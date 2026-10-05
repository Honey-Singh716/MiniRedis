#include <iostream>
#include <string>
#include <vector>
#include <queue>


using namespace std;

class Expiry{
public:
    long long expiry;
    string key;
    long long version;

};


struct CompareExpiry {
    bool operator()(const Expiry& a, const Expiry& b) {
        return a.expiry > b.expiry;
    }
};



class TTLManager{
private:
    priority_queue<Expiry, vector<Expiry>, CompareExpiry> expiryQueue;
public:
    TTLManager();
    void addExpiry(long long expiry, string key, long long version);
    Expiry getExpiry();
    bool isExpiryEmpty();
    void removeExpiry();

};