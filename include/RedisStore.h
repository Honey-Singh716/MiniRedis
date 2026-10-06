#pragma once

#ifndef REDIS_STORE_H
#define REDIS_STORE_H

#include <string>
#include <unordered_map>

#include "HashTable.h"
#include "TTLManager.h"
#include "LRUCache.h"
#include "Persistence.h"

using namespace std;

class RedisStore {
private:
    HashTable store;
    TTLManager ttlManager;
    LRUCache lruCache;
    Persistence persistence;

    unordered_map<string, long long> versionMap;
    unordered_map<string, long long> expiryMap;

    long long getNextVersion(string key);

public:
    RedisStore(int capacity);

    void load(const string& filename);
    void save(const string& filename);
    void processExpiredKeys();
    bool exists(string key);
    long long get_ttl(string key);
    void set(string key, string value);
    void set(string key, string value, long long ttl);
    string get(string key);
    bool del(string key);
};

#endif