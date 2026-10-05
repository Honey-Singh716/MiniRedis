#pragma once

#ifndef REDIS_STORE_H
#define REDIS_STORE_H

#include <string>
#include "HashTable.h"

using namespace std;

class RedisStore {
private:
    HashTable store;

public:
    RedisStore(int capacity);

    void set(string key, string value);
    string get(string key);
    bool del(string key);
};

#endif