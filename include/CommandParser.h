#pragma once

#include <string>
#include <vector>

#include "RedisStore.h"

using namespace std;

class CommandParser {
public:
    vector<string> tokenize(string command);
    bool isValidCommand(vector<string> tokens);
    void executeCommand(vector<string> tokens, RedisStore& store);
};