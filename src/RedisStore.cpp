#include "../include/RedisStore.h"
#include <chrono>



RedisStore::RedisStore(int capacity)
    : store(capacity) {
}

void RedisStore::set(string key, string value) {
    store.set(key, value);
    expiryMap.erase(key);
    getNextVersion(key);
}

void RedisStore::set(string key, string value, long long ttl){
    
    store.set(key, value);
    
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
    return store.get(key);
}

bool RedisStore::del(string key) {
    bool deleted = store.del(key);

    if(deleted) {
        versionMap[key]++;
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