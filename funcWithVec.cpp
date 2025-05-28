#include<vector>
#include<math.h>
#include<iostream>
#include<algorithm>
#include "header.h"
#include "headvec.h"
#include "headrand.h"
vector<int> continued_fraction(double x, int iterations) {
    vector<int> result;
    double remainder = x;
    for (int i = 0; i < iterations; ++i) {
        int integer_part = floor(remainder);
        result.push_back(integer_part);
        remainder -= integer_part;
        if (remainder == 0) break; 
        remainder = 1 / remainder;
    }
    return result;
}
vector<int> primeFactors(int n) {
    vector<int> factors;
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }
    for (int i = 3; i * i <= n; i = i + 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 2){ 
        factors.push_back(n);
    }
    vector<int> uniqueFactors;
    for(int factor : factors) {
        if (find(uniqueFactors.begin(), uniqueFactors.end(), factor) == uniqueFactors.end()){ 
            uniqueFactors.push_back(factor);
        }
    }
    return uniqueFactors;
}
string Decrypt(const vector<pair<int, int>>& ciphertext, int x, int p) {
    string decryptedText = "";
    for (const auto& pair : ciphertext) {
        int a = pair.first;
        int b = pair.second;
        int s = BinCheck(a, x, p);
        int s_inv = BinCheck(s, p - 2, p); 
        int m = (b * s_inv) % p;
        decryptedText += static_cast<char>(m);
    }
    return decryptedText;
}
int LineFrac(){
	double x;
    int iterations;
    cout << "Введите вещественное число: ";
    cin >> x;
    cout << "Введите количество итераций: ";
    cin >> iterations;
    vector<int> cf = continued_fraction(x, iterations);
    cout << "Цепная дробь:";
    if (cf.empty()){ 
        cout << "Нет такой.";
    }
    else {
        for (const auto i : cf) {
            cout << i << " "; 
        }
    }
    cout << endl;
    return 0;
}
bool Primitive(int g, int p){
    if (g < 1 || g >= p) {
        return false;
    }
    int per = p - 1;
    vector<int> prime_factors = primeFactors(per);
    for (int e : prime_factors) {
        if (BinCheck(g, per / e, p) == 1){
            return false;
        }
    }
    return true;
}
