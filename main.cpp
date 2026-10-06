#include "include/RedisStore.h"
#include "include/CommandParser.h"

#include <iostream>
#include <string>

using namespace std;

int main() {

    RedisStore store(10);
    CommandParser parser;

    string command;

    while(true) {

        cout << "MiniRedis> ";

        getline(cin, command);

        if(command == "EXIT") {
            break;
        }

        vector<string> tokens =
            parser.tokenize(command);

        parser.executeCommand(tokens, store);
    }

    return 0;
}