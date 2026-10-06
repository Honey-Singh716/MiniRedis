#include "../include/RedisStore.h"
#include <chrono>



RedisStore::RedisStore(int capacity)
    : store(capacity),
    lruCache(capacity){
}

void RedisStore::set(string key, string value) {
    store.set(key, value);
    lruCache.put(key, value);
    expiryMap.erase(key);
    getNextVersion(key);
}

void RedisStore::set(string key, string value, long long ttl){
    
    store.set(key, value);
    
    lruCache.put(key, value);
    
    long long currentTime = chrono::duration_cast<chrono::seconds>(
            chrono::system_clock::now().time_since_epoch()
            ).count();
        
    long long expiryTime = currentTime + ttl;

    expiryMap[key] = expiryTime;

    long long version = getNextVersion(key);

    ttlManager.addExpiry(expiryTime, key, version);

}

string RedisStore::get(string key) {

    processExpiredKeys();

    string value = lruCache.get(key);

    if(value != "(nil)") {
        return value;
    }

    value = store.get(key);

    if(value != "(nil)") {
        lruCache.put(key, value);
    }

    return value;
}

bool RedisStore::del(string key) {
    bool deleted = store.del(key);

    if(deleted) {
        versionMap[key]++;
        lruCache.remove(key);
    }
    
    expiryMap.erase(key);
    return deleted;
}

long long RedisStore::getNextVersion(string key){
    versionMap[key]++;
    return versionMap[key];
}


void RedisStore::processExpiredKeys() {

    long long currentTime =
        chrono::duration_cast<chrono::seconds>(
            chrono::system_clock::now().time_since_epoch()
        ).count();

    while(!ttlManager.isExpiryEmpty()) {

        Expiry topExpiry = ttlManager.getExpiry();

        if(topExpiry.expiry > currentTime) {
            break;
        }

        auto it = versionMap.find(topExpiry.key);

        if(it != versionMap.end() &&
           it->second == topExpiry.version) {

            store.del(topExpiry.key);
            versionMap.erase(it);
            lruCache.remove(topExpiry.key);
            expiryMap.erase(topExpiry.key);
        }

        ttlManager.removeExpiry();
    }
}

bool RedisStore::exists(string key) {
    processExpiredKeys();
    return store.exists(key);
}

long long RedisStore::get_ttl(string key) {
    processExpiredKeys();

    if(!store.exists(key)) {
        return -2;
    }
    
    auto it = expiryMap.find(key);

    if(it == expiryMap.end()) {
        return -1;
    }

    long long currentTime =
        chrono::duration_cast<chrono::seconds>(
            chrono::system_clock::now().time_since_epoch()
        ).count();

    long long expiryTime = it->second;

    return expiryTime - currentTime;
  
}

void RedisStore::save(const string& filename) {

    processExpiredKeys();

    vector<pair<string, string>> data = store.getAll();

    vector<PersistentEntry> persistentData;

    for(const auto& entry : data) {

        PersistentEntry persistentEntry;

        persistentEntry.key = entry.first;
        persistentEntry.value = entry.second;

        auto it = expiryMap.find(entry.first);

        if(it == expiryMap.end()) {
            persistentEntry.expiryTime = -1;
        }
        else {
            persistentEntry.expiryTime = it->second;
        }

        persistentData.push_back(persistentEntry);
    }

    persistence.save(persistentData, filename);
}

void RedisStore::load(const string& filename) {

    vector<PersistentEntry> data = persistence.load(filename);

    long long currentTime =
        chrono::duration_cast<chrono::seconds>(
            chrono::system_clock::now().time_since_epoch()
        ).count();

    for(const auto& entry : data) {

        // Already expired while Redis was offline
        if(entry.expiryTime != -1 &&
           entry.expiryTime <= currentTime) {
            continue;
        }

        // Restore actual data
        store.set(entry.key, entry.value);

        // Restore cache
        lruCache.put(entry.key, entry.value);

        // Create a fresh version
        long long version = getNextVersion(entry.key);

        // No TTL
        if(entry.expiryTime == -1) {
            continue;
        }

        // Restore TTL metadata
        expiryMap[entry.key] = entry.expiryTime;

        // Restore expiry into min-heap
        ttlManager.addExpiry(
            entry.expiryTime,
            entry.key,
            version
        );
    }
}






