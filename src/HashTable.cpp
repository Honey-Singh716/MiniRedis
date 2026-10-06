#include <iostream>
#include "../include/HashTable.h"



HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
}

int HashTable::hashKey(string key){
    int hash = 0;

    for(int i = 0;i<key.length();i++){
        hash = hash*31 + key[i];
    }
    return hash % capacity;
}


double HashTable::loadFactor(){
    return (double)size/capacity;
}


void HashTable::rehash(){
    cout<<"Rehashing..."<<endl;
    
    int oldcapacity = capacity;
    capacity *= 2;

    vector<Node*> oldTable = table;
    table.clear();
    table.resize(capacity, nullptr);


    for(int i = 0;i<oldcapacity;i++){
        Node* current = oldTable[i];

        while(current != nullptr){
            Node* next = current->next;
            
            int newIndex = hashKey(current->key);
            
            current->next = table[newIndex];
            table[newIndex] = current;

            current =  next;
        }
    }

}


void HashTable::set(string key,string value){
    int index = hashKey(key);
  
    Node* current = table[index];
        
    while(current != NULL){
        if(current->key == key){
            current->value = value;
            return;
        }
        current = current->next;
    }
        
    Node*newNode = new Node(key,value);
    newNode->next = table[index];
    table[index] = newNode;

    size++;
    
    if(loadFactor() >= 0.75){
        rehash();
    }
}

bool HashTable::del(string key){
    int index = hashKey(key);

    if(table[index] == NULL){
        return false;
    }

    Node* current = table[index];
    Node* prev = NULL;

    while(current != NULL){
        if(current->key == key){
            if(prev == NULL){
                table[index] = current->next;
            }
            else{
                prev->next = current->next;
            }
            delete current;
            size--;
            return true;
        }

        else{
            prev = current;
            current = current->next;
        }
    }

    return false;
}

int HashTable::getBucket(string key) {
    return hashKey(key);
}

int HashTable::getSize(){
    return size;
}

string HashTable::get(string key){
    int index = hashKey(key);

    Node* current = table[index];

    while(current != NULL){
        if(current->key == key){
            return current->value;
        }
       
        current = current->next;
        
    }

    return "(nil)";
}


bool HashTable::exists(string key){
    int index = hashKey(key);
    
    Node* current = table[index];

    while(current != NULL){
        if(current->key == key){
            return true;
        }
       
        current = current->next;
    }
    return false;
}

vector<pair<string, string>> HashTable::getAll() {

    vector<pair<string, string>> data;

    for(int i = 0; i < capacity; i++) {

        Node* current = table[i];

        while(current != nullptr) {

            data.push_back({
                current->key,
                current->value
            });

            current = current->next;
        }
    }

    return data;
}