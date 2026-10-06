#pragma once

#include <string>
#include <vector>


using namespace std;

struct PersistentEntry {
    string key;
    string value;
    long long expiryTime;
};


class Persistence {
public:
    void save(const vector<PersistentEntry>& data,
              const string& filename);

    vector<PersistentEntry> load(const string& filename);
};