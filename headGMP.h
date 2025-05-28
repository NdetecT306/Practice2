#include <gmp.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <gmpxx.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
void RandomNumbers_gmp(mpz_t result, mpz_t start, mpz_t end);
void findPrimitiveRoot(mpz_t result, mpz_t p);
string Decrypt(const vector<pair<mpz_class, mpz_class>>& ciphertext, mpz_t x, mpz_t p);
int ElGamal_gmp();
void Bin(mpz_t result, mpz_t a, mpz_t x, mpz_t p);
bool prostota_gmp(mpz_t p);
