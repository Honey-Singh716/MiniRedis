#include "../include/Persistence.h"
#include <fstream>

void Persistence::save(const vector<PersistentEntry>& data, const string& filename){

    ofstream file(filename);

    if(!file.is_open()) {
        return;
    }

    for(const auto& entry : data) {
        file << entry.key << "|"
             << entry.value << "|"
             << entry.expiryTime << "\n";
    }

    file.close();
}


vector<PersistentEntry> Persistence::load(const string& filename) {

    vector<PersistentEntry> data;

    ifstream file(filename);

    if(!file.is_open()) {
        return data;
    }

    string line;

    while(getline(file, line)) {

        size_t firstSeparator = line.find('|');

        if(firstSeparator == string::npos) {
            continue;
        }

        size_t secondSeparator =
            line.find('|', firstSeparator + 1);

        if(secondSeparator == string::npos) {
            continue;
        }

        string key =
            line.substr(0, firstSeparator);

        string value =
            line.substr(
                firstSeparator + 1,
                secondSeparator - firstSeparator - 1
            );

        string expiryString =
            line.substr(secondSeparator + 1);

       long long expiryTime;

        try {
            expiryTime = stoll(expiryString);
        }
        catch(...) {
            continue;
        }

        data.push_back({
            key,
            value,
            expiryTime
        });
    }

    file.close();

    return data;
}