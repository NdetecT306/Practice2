#include <math.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <gmp.h>
#include <gmpxx.h>
#include "header.h"
#include "headGMP.h"
using namespace std;
void RandomNumbers_gmp(mpz_t result, mpz_t start, mpz_t end) {
    mpz_t range;
    mpz_init(range);
    mpz_sub(range, end, start);
    mpz_add_ui(range, range, 1); 
    gmp_randstate_t randstate;
    gmp_randinit_mt(randstate);  
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    gmp_randseed_ui(randstate, seed);
    mpz_urandomm(result, randstate, range);
    mpz_add(result, result, start);  
    mpz_clear(range);
    gmp_randclear(randstate);
}
void findPrimitiveRoot(mpz_t result, mpz_t p) {
    mpz_t phi;
    mpz_init(phi);
    mpz_sub_ui(phi, p, 1); 
    mpz_t n;
    mpz_init_set(n, phi);
    mpz_t i;
    mpz_init(i);
    mpz_t factors[100]; 
    int factorCount = 0;
    for (mpz_set_ui(i, 2); mpz_cmp(i, n) <= 0; mpz_add_ui(i, i, 1)) {
        mpz_t temp;
        mpz_init(temp);
        mpz_set(temp, n);
        if(prostota_gmp(p)){
            while (mpz_divisible_p(temp, i)) {
                mpz_tdiv_q(temp, temp, i);
                if (factorCount < 100) {
                    mpz_init(factors[factorCount]);
                    mpz_set(factors[factorCount], i);
                    factorCount++;
                } else {
                    cerr << "Слишком много простых делителей phi." << endl;
                    exit(1);
                }
                mpz_set(n,temp);
            }
        }
        mpz_clear(temp);
        if (mpz_cmp(i, n) > 0)
            break;

    }
    mpz_t a;
    mpz_init_set_ui(a, 2);
    mpz_t limit;
    mpz_init(limit);
    mpz_sub_ui(limit,p,1);
    bool found = false;
    while(mpz_cmp(a,limit)<0 && !found){
       bool isPrimitiveRoot = true;
        for (int j = 0; j < factorCount; j++) {
            mpz_t exp;
            mpz_init(exp);
            mpz_tdiv_q(exp, phi, factors[j]);  
            mpz_t result_pow;
            mpz_init(result_pow);
            Bin(result_pow, a, exp, p);
            if (mpz_cmp_ui(result_pow, 1) == 0) {
                isPrimitiveRoot = false;
                mpz_clear(result_pow);
                mpz_clear(exp);
                break;
            }
            mpz_clear(result_pow);
            mpz_clear(exp);
        }
        if (isPrimitiveRoot) {
            mpz_set(result, a);
            found = true;
            break;
        }
       mpz_add_ui(a,a,1);
    }
    if (!found) {
        cout << "Примитивный корень не найден." << endl;
        mpz_set_ui(result, 0);
    }
    mpz_clear(phi);
    mpz_clear(n);
    mpz_clear(i);
    mpz_clear(a);
    mpz_clear(limit);
    for (int j = 0; j < factorCount; j++) {
        mpz_clear(factors[j]);
    }
}
string Decrypt(const vector<pair<mpz_class, mpz_class>>& ciphertext, mpz_t x, mpz_t p) {
    string decryptedText = "";
    mpz_t s, s_inv, m, a_temp; 
    mpz_init(s);
    mpz_init(s_inv);
    mpz_init(m);
    mpz_init(a_temp);
    for (const auto& pair : ciphertext) {
        mpz_set(a_temp, pair.first.get_mpz_t());
        Bin(s, a_temp, x, p);
        mpz_t p_minus_2;
        mpz_init(p_minus_2);
        mpz_sub_ui(p_minus_2, p, 2);
        Bin(s_inv, s, p_minus_2, p);
        mpz_clear(p_minus_2);
        mpz_mul(m, pair.second.get_mpz_t(), s_inv);
        mpz_mod(m, m, p);
        decryptedText += static_cast<char>(mpz_get_ui(m));
    }
    mpz_clear(s);
    mpz_clear(s_inv);
    mpz_clear(m);
    mpz_clear(a_temp);  
    return decryptedText;
}

int ElGamal_gmp() {
    mpz_t p, g, bob_x, bob_y;
    mpz_init(p);
    mpz_init(g);
    mpz_init(bob_x);
    mpz_init(bob_y);

    mpz_t temp_min, temp_max;
    mpz_init_set_ui(temp_min, 100000);
    mpz_init_set_ui(temp_max, 50000000);
    RandomNumbers_gmp(p, temp_min, temp_max);
    while (!prostota_gmp(p)) {
        RandomNumbers_gmp(p, temp_min, temp_max);
    }
    mpz_clear(temp_min);
    mpz_clear(temp_max);
    findPrimitiveRoot(g, p);
    string myText;
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Не удалось открыть файл input.txt" << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line)) {
        myText += line + "\n";
    }
    inputFile.close();
    if (!myText.empty() && myText.back() == '\n') {
        myText.pop_back();
    }
    mpz_t temp_p_minus_1;
    mpz_init(temp_p_minus_1);
    mpz_sub_ui(temp_p_minus_1, p, 1);
    mpz_t temp_two;
    mpz_init_set_ui(temp_two, 2);
    RandomNumbers_gmp(bob_x, temp_two, temp_p_minus_1);
    mpz_clear(temp_two);
    mpz_clear(temp_p_minus_1);
    Bin(bob_y, g, bob_x, p);
    vector<pair<mpz_class, mpz_class>> ciphertext;
    string encryptedTextString;
    for (unsigned char c : myText) {
        mpz_t m;
        mpz_init_set_ui(m, static_cast<int>(c));
        mpz_t k, a, b;
        mpz_init(k);
        mpz_init(a);
        mpz_init(b);
        mpz_t temp_p_minus_2;
        mpz_init(temp_p_minus_2);
        mpz_sub_ui(temp_p_minus_2, p, 2);
        mpz_t temp_two_2;
        mpz_init_set_ui(temp_two_2, 2);
        RandomNumbers_gmp(k, temp_two_2, temp_p_minus_2);
        mpz_clear(temp_two_2);
        mpz_clear(temp_p_minus_2);
        Bin(a, g, k, p);
        mpz_t temp_bob_y_k;
        mpz_init(temp_bob_y_k);
        Bin(temp_bob_y_k, bob_y, k, p);
        mpz_mul(b, m, temp_bob_y_k);
        mpz_mod(b, b, p);
        mpz_clear(temp_bob_y_k);
        mpz_class a_class, b_class;
        mpz_set(a_class.get_mpz_t(), a);
        mpz_set(b_class.get_mpz_t(), b);
        encryptedTextString += "(" + a_class.get_str() + ", " + b_class.get_str() + ") ";
        ciphertext.push_back({ a_class, b_class });
        mpz_clear(m);
        mpz_clear(k);
        mpz_clear(a);
        mpz_clear(b);
    }
    mpz_class bob_x_class, p_class;
    mpz_set(bob_x_class.get_mpz_t(), bob_x);
    mpz_set(p_class.get_mpz_t(), p);
    string decryptedText = Decrypt(ciphertext, bob_x, p);
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Не удалось открыть файл output.txt для записи" << endl;
        return 1;
    }
    outputFile << "Зашифрованный текст: " << endl << encryptedTextString << endl;
    outputFile << "Расшифрованный текст: " << endl << decryptedText << endl;
    outputFile.close();
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(bob_x);
    mpz_clear(bob_y);
    return 0;
}
bool prostota_gmp(mpz_t p) {
    int reps = 100; 
    if (mpz_cmp_ui(p, 2) < 0) {
      return false; 
    }
    if (mpz_cmp_ui(p, 2) == 0 || mpz_cmp_ui(p, 3) == 0) {
      return true; 
    }
    if (mpz_odd_p(p) == 0) {
      return false; 
    }
    int result = mpz_probab_prime_p(p, reps);
    return (result > 0);
}
void Bin(mpz_t result, mpz_t a, mpz_t x, mpz_t p) {
    mpz_t b;
    mpz_init(b);
    mpz_mod(b, a, p);  
    mpz_set_ui(result, 1); 
    mpz_t temp_x;
    mpz_init(temp_x);
    mpz_set(temp_x, x);
    while (mpz_cmp_ui(temp_x, 0) > 0) { 
        if (mpz_odd_p(temp_x)) { 
            mpz_mul(result, result, b); 
            mpz_mod(result, result, p);  
        }
        mpz_tdiv_q_2exp(temp_x, temp_x, 1); 
        mpz_mul(b, b, b);  
        mpz_mod(b, b, p);  
    }
}

