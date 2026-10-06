#include <iostream>
#include <string>
#include <vector>
#include "../include/CommandParser.h"

using namespace std;

vector<string> CommandParser::tokenize(string command){
    vector<string> tokens;
    string token;

    for(char c : command){
        if(c == ' '){
            if(token.length() > 0){
                tokens.push_back(token);
                token.clear();
            }
        }
        else{
            token += c;
        }
    }

    if(token.length() > 0){
        tokens.push_back(token);
    }
    return tokens;
}

bool CommandParser::isValidCommand(vector<string> tokens){

    if(tokens.empty()) {
        return false;
    }

    string command = tokens[0];

    if(command != "SET" &&
       command != "GET" &&
       command != "DEL" &&
       command != "EXISTS" &&
       command != "TTL") {
        return false;
    }

    if(command == "SET") {

        if(tokens.size() == 3) {
            return true;
        }

        if(tokens.size() == 5) {
            if(tokens[3] != "EX") {
                return false;
            }

            if(tokens[4].find_first_not_of("0123456789") != string::npos) {
                return false;
            }

            return true;
        }

        return false;
    }

    if(command == "GET" ||
       command == "DEL" ||
       command == "EXISTS" ||
       command == "TTL") {

        return tokens.size() == 2;
    }

    return false;
}

void CommandParser::executeCommand(vector<string> tokens, RedisStore& store) {

    if(!isValidCommand(tokens)) {
        cout << "Invalid command" << endl;
        return;
    }

    string command = tokens[0];

    if(command == "SET") {

        string key = tokens[1];
        string value = tokens[2];

        if(tokens.size() == 3) {
            store.set(key, value);
            cout << "OK" << endl;
        }
        else if(tokens.size() == 5) {
            long long ttl = stoll(tokens[4]);

            store.set(key, value, ttl);
            cout << "OK" << endl;
        }
    }

    if(command == "GET") {

        string key = tokens[1];
        string value = store.get(key);

        cout << "Value = " << value << endl;
    }


    if(command == "DEL") {

        string key = tokens[1];
        bool deleted = store.del(key);

        if(deleted) {
            cout << "Deleted" << endl;
        }
        else {
            cout << "Key not found" << endl;
        }
    }

    if(command == "EXISTS") {

        string key = tokens[1];
        bool exists = store.exists(key);

        if(exists) {
            cout << "Key exists" << endl;
        }
        else {
            cout << "Key does not exist" << endl;
        }
    }

    if(command == "TTL") {

        string key = tokens[1];
        long long ttl = store.get_ttl(key);

        if(ttl == -1) {
            cout << "Key does not exist" << endl;
        }
        else if(ttl == -2) {
            cout << "Key exists but has no associated TTL" << endl;
        }
        else {
            cout << "TTL = " << ttl << " seconds" << endl;
        }
    }
}