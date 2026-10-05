#include <iostream>
#include "RedisStore.h"


using namespace std;

int main() {

    RedisStore redis(4);

    redis.set("name", "Honey");
    redis.set("age", "18");

    cout << redis.get("name") << endl;
    cout << redis.get("age") << endl;

    redis.set("name", "Singh");

    cout << redis.get("name") << endl;

    redis.del("age");

    cout << redis.get("age") << endl;  

    return 0;
}