#include "header.h"
#include "headrand.h"
#include "headvec.h"
using namespace std;
bool prostota(int p){
    if (p < 2){
        return false;
    }
    for (int i = 2; i <= sqrt(p); i++){
        if (p % i == 0) {
            return false;
        }
    }
    return true;
}
int BinCheck(int a, int x, int p){
    int result = 1;
    int b = a % p;
    while (x > 0){
        if (x % 2 == 1){ 
            result = (result * b) % p;
        }
        x = x >> 1;
        b = (b * b) % p;
    }
    return result;
}
int Euclidean(int a, int b, int &u, int &v) {
    if (b == 0) { 
      u = 1;
      v = 0;
      return a;
    }
    int x1, y1;
    int d = Euclidean(b, a % b, x1, y1); 
    u = y1;
    v = x1 - (a / b) * y1;
    return d;
}
int FindAnswer(int &c, int &m){
    try {
        cout << "Введите c: ";
        if (!(cin >> c)) throw runtime_error("c");
        cout << "Введите m: ";
        if (!(cin >> m)) throw runtime_error("m");
    } catch (const runtime_error& error) {
        cerr << "Ошибка ввода: " << error.what()  << " не число."<< endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        return -1;
    }
    if ((c % m == 0)){
        cout << "c и m не взаимно простые." << endl;
        return -1;
    }
    int u = 0, v = 0;
    int result = Euclidean(c, m, u, v);
    return ((u % m + m) % m);
}
int generatePrimitiveRoot(int p) {
    int g, maxIter = 1000;
    for (int i = 0; i < maxIter; ++i) {
        g = RandomNumbers(2, p - 1); 
        if (Primitive(g, p)) return g; 
    }
    return -1;
}
