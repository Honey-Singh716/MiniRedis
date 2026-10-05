#include "RedisStore.h"

RedisStore::RedisStore(int capacity)
    : store(capacity) {
}

void RedisStore::set(string key, string value) {
    store.set(key, value);
}

string RedisStore::get(string key) {
    return store.get(key);
}

bool RedisStore::del(string key) {
    return store.del(key);
}