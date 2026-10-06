#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Node{
    public:
    string key;
    string value;
    Node* next;

    Node(string key, string value){
        this->key = key;
        this->value = value;
        this->next = NULL;
    }
};


class HashTable{
private:
    int size;
    int capacity;
    vector<Node*> table;

    double loadFactor();

    int hashKey(string key);
    void rehash();
public:
    HashTable(int capacity){
        this->capacity = capacity;
        this->size = 0;
        this->table.resize(capacity, NULL);
    }

    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    int getSize();
    int getBucket(string key);
    void set(string key, string value);
    string get(string key);
    bool del(string key);
    bool exists(string key);
};

