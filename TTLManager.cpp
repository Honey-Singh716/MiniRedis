#include "TTLManager.h"

TTLManager::TTLManager() {
}

void TTLManager::addExpiry(long long expiry, string key, long long version){
    Expiry newExpiry;

    newExpiry.expiry = expiry;
    newExpiry.key = key;
    newExpiry.version = version;

    expiryQueue.push(newExpiry);
   
}

Expiry TTLManager::getExpiry(){
    
    if(!expiryQueue.empty()){
        Expiry topExpiry = expiryQueue.top();
        return topExpiry;
    }

    Expiry emptyExpiry;
    emptyExpiry.expiry = -1;
    emptyExpiry.key = "";
    emptyExpiry.version = -1;

    return emptyExpiry;
}

bool TTLManager::isExpiryEmpty(){
    return expiryQueue.empty();
}

void TTLManager::removeExpiry(){
    if(!expiryQueue.empty()){
        expiryQueue.pop();
    }
}