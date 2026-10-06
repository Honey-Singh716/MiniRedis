#include "../include/LRUCache.h"

LRUNode::LRUNode(string key,string value){
    this->key = key;
    this->value = value;
    this->prev = nullptr;
    this->next = nullptr;
}

LRUCache::LRUCache(int capacity){
    
    this->capacity = capacity;
    this->head = nullptr;
    this->tail = nullptr;
}