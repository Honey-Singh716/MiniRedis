#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class LRUNode {
public:
    string key;
    string value;

    LRUNode* prev;
    LRUNode* next;

    LRUNode(string key, string value);
};


class LRUCache {
private:
    int capacity;
    unordered_map<string, LRUNode*> cache;

    LRUNode* head;
    LRUNode* tail;


    void removeNode(LRUNode* node);
    void insertAtHead(LRUNode* node);
    void moveToHead(LRUNode* node);
    void removeLRU();

public:
    LRUCache(int capacity);
    
    ~LRUCache();
    
    void remove(string key);
    string get(string key);
    void put(string key, string value);
};