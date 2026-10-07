#include "include/HashTable.h"

#include <iostream>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

int main() {

    cout << "\n";
    cout << "==================================================\n";
    cout << "             MiniRedis Rehash Benchmark\n";
    cout << "==================================================\n\n";

    const int N = 100000;

    HashTable table(10);

    auto start = high_resolution_clock::now();

    for(int i = 0; i < N; i++) {

        table.set(
            "key_" + to_string(i),
            "value_" + to_string(i)
        );
    }

    auto end = high_resolution_clock::now();

    long long totalTime =
        duration_cast<microseconds>(
            end - start
        ).count();

    cout << "Initial Capacity : 10\n";
    cout << "Total Keys       : " << N << "\n";
    cout << "Final Size       : " << table.getSize() << "\n";

    cout << "\nTotal SET time including rehashing: "
         << totalTime
         << " us\n";

    cout << "\nAverage SET time: "
         << static_cast<double>(totalTime) / N
         << " us/key\n";

    cout << "\n==================================================\n";
    cout << "Rehash Benchmark Complete\n";
    cout << "==================================================\n";

    return 0;
}