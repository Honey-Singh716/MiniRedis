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

LRUCache::~LRUCache() {

    LRUNode* current = head;

    while(current != nullptr) {

        LRUNode* next = current->next;

        delete current;

        current = next;
    }
}

void LRUCache::insertAtHead(LRUNode* node) {

    node->prev = nullptr;
    node->next = head;

    if(head != nullptr) {
        head->prev = node;
    }
    else {
        tail = node;
    }

    head = node;
}


void LRUCache::removeNode(LRUNode* node) {

    if(node->prev != nullptr) {
        node->prev->next = node->next;
    }
    else {
        head = node->next;
    }

    if(node->next != nullptr) {
        node->next->prev = node->prev;
    }
    else {
        tail = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
}

void LRUCache::moveToHead(LRUNode* node) {

    if(node == head) {
        return;
    }

    removeNode(node);
    insertAtHead(node);
}


void LRUCache::removeLRU() {

    if(tail == nullptr) {
        return;
    }

    LRUNode* node = tail;

    cache.erase(node->key);

    removeNode(node);

    delete node;
}


void LRUCache::put(string key, string value) {

    auto it = cache.find(key);

    // Key already exists
    if(it != cache.end()) {

        LRUNode* node = it->second;

        node->value = value;

        moveToHead(node);

        return;
    }

    // New key
    LRUNode* newNode = new LRUNode(key, value);

    // Cache full
    if(cache.size() >= capacity) {
        removeLRU();
    }

    insertAtHead(newNode);

    cache[key] = newNode;
}


string LRUCache::get(string key) {

    auto it = cache.find(key);

    if(it == cache.end()) {
        return "(nil)";
    }

    LRUNode* node = it->second;

    moveToHead(node);

    return node->value;
}

void LRUCache::remove(string key){
    auto it = cache.find(key);
   
    if(it != cache.end()){
        LRUNode* node = it->second;

        removeNode(node);
        cache.erase(it);
        delete node;
    }
}

