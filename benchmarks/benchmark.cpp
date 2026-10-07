#include "include/HashTable.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

const int RUNS = 3;


// ============================================================
// Generate Keys
// ============================================================

vector<string> generateKeys(int N) {

    vector<string> keys;
    keys.reserve(N);

    for(int i = 0; i < N; i++) {
        keys.push_back("key_" + to_string(i));
    }

    return keys;
}


// ============================================================
// Custom HashTable
// ============================================================

void benchmarkCustomHashTable(
    const vector<string>& keys
) {

    int N = static_cast<int>(keys.size());

    long long setTotal = 0;
    long long getTotal = 0;
    long long delTotal = 0;


    for(int run = 0; run < RUNS; run++) {

        // Large capacity so this benchmark does NOT trigger rehashing.
        HashTable table(N * 2);


        // ---------------- SET ----------------

        auto start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            table.set(
                keys[i],
                "value_" + to_string(i)
            );
        }

        auto end = high_resolution_clock::now();

        setTotal += duration_cast<microseconds>(
            end - start
        ).count();


        // ---------------- GET ----------------

        volatile size_t checksum = 0;

        start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            string value = table.get(keys[i]);

            checksum += value.length();
        }

        end = high_resolution_clock::now();

        getTotal += duration_cast<microseconds>(
            end - start
        ).count();


        // ---------------- DELETE ----------------

        start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            table.del(keys[i]);
        }

        end = high_resolution_clock::now();

        delTotal += duration_cast<microseconds>(
            end - start
        ).count();
    }


    cout << left
         << setw(20) << "Custom HashTable"
         << setw(15) << setTotal / RUNS
         << setw(15) << getTotal / RUNS
         << setw(15) << delTotal / RUNS
         << endl;
}


// ============================================================
// std::unordered_map
// ============================================================

void benchmarkUnorderedMap(
    const vector<string>& keys
) {

    int N = static_cast<int>(keys.size());

    long long setTotal = 0;
    long long getTotal = 0;
    long long delTotal = 0;


    for(int run = 0; run < RUNS; run++) {

        unordered_map<string, string> table;

        table.reserve(N * 2);


        // ---------------- SET ----------------

        auto start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            table[keys[i]] =
                "value_" + to_string(i);
        }

        auto end = high_resolution_clock::now();

        setTotal += duration_cast<microseconds>(
            end - start
        ).count();


        // ---------------- GET ----------------

        volatile size_t checksum = 0;

        start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            auto it = table.find(keys[i]);

            if(it != table.end()) {
                checksum += it->second.length();
            }
        }

        end = high_resolution_clock::now();

        getTotal += duration_cast<microseconds>(
            end - start
        ).count();


        // ---------------- DELETE ----------------

        start = high_resolution_clock::now();

        for(int i = 0; i < N; i++) {

            table.erase(keys[i]);
        }

        end = high_resolution_clock::now();

        delTotal += duration_cast<microseconds>(
            end - start
        ).count();
    }


    cout << left
         << setw(20) << "unordered_map"
         << setw(15) << setTotal / RUNS
         << setw(15) << getTotal / RUNS
         << setw(15) << delTotal / RUNS
         << endl;
}


// ============================================================
// Main
// ============================================================

int main() {

    cout << "\n";
    cout << "============================================================\n";
    cout << "              MiniRedis HashTable Benchmark\n";
    cout << "============================================================\n";

    cout << "\nAverage of " << RUNS << " runs\n";
    cout << "Rehashing excluded from this benchmark.\n";


    vector<int> sizes = {
        1000,
        10000,
        100000
    };


    for(int N : sizes) {

        cout << "\n";
        cout << "==================== N = "
             << N
             << " ====================\n\n";

        vector<string> keys =
            generateKeys(N);


        cout << left
             << setw(20) << "Implementation"
             << setw(15) << "SET(us)"
             << setw(15) << "GET(us)"
             << setw(15) << "DEL(us)"
             << endl;

        cout << "------------------------------------------------------------\n";


        benchmarkCustomHashTable(keys);

        benchmarkUnorderedMap(keys);
    }


    cout << "\n";
    cout << "============================================================\n";
    cout << "                 Benchmark Complete\n";
    cout << "============================================================\n";

    return 0;
}