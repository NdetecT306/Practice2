#include<vector>
#include<string>
#include<math.h>
#include<iostream>
#include<algorithm>
using namespace std;
vector<int> continued_fraction(double x, int iterations);
vector<int> primeFactors(int n);
string Decrypt(const vector<pair<int, int>>& ciphertext, int x, int p);
int LineFrac();
bool Primitive(int g, int p);
