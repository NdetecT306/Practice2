#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "headrand.h"
using namespace std;
int RandomNumbers(int start, int end){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    ranlux24_base generation(seed);
    uniform_int_distribution<int> distribution(start, end);
    return distribution(generation);
}
