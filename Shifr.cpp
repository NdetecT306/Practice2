#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "header.h"
#include "headvec.h"
#include "headshif.h"
#include "headrand.h"
pair<int, int> elGamalEncrypt(int message, int g, int mitm_y, int p) {
    int k = RandomNumbers(2, p - 2);
    int a = BinCheck(g, k, p);
    int b = (message * BinCheck(mitm_y, k, p)) % p;
    return {a, b};
}
int ElGamal() {
    int p = RandomNumbers(100000, 5000000);
    while (!prostota(p)) {
        p = RandomNumbers(10000, 5000000);
    }
    int g = generatePrimitiveRoot(p);
    while(g==-1){ 
        g = generatePrimitiveRoot(p);
    }
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
    int bob_x = RandomNumbers(2, p - 1); 
    int bob_y = BinCheck(g, bob_x, p); 
    int alice_x = RandomNumbers(2, p - 1); 
    vector<pair<int, int>> ciphertext;
    string encryptedTextString;
    for (unsigned char c : myText) {
        int m = c;
        int k = RandomNumbers(2, p - 2);
        int a = BinCheck(g, k, p);
        int b = (m * BinCheck(bob_y, k, p)) % p;
        encryptedTextString += "(" + to_string(a) + ", " + to_string(b) + ") ";
        ciphertext.push_back({ a, b });
    }
    cout << endl;
    string decryptedText = Decrypt(ciphertext, bob_x, p);
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Не удалось открыть файл output.txt для записи" << endl;
        return 1;
    }
    outputFile << "Зашифрованный текст: " << endl << encryptedTextString << endl;
    outputFile << "Расшифрованный текст: " << endl << decryptedText << endl;
    outputFile.close();
    return 0;
}
int meetInTheMiddleAttack(int p, int g, int y){
    map<int, int> allX;
    for (int i = 1; i < p; ++i){
        allX[BinCheck(g, i, p)] = i;
    }
    int realX = -1;
    int perY = BinCheck(y, 1, p);
    for (int k = 1; k < p; ++k){
        int C1 = BinCheck(g, k, p);
        int gI = BinCheck(C1, p - 2, p);
        int right_side_val = (perY * gI) % p;
        if (allX.count(right_side_val)) {
            int j = allX[right_side_val];
            realX = (k + j) % (p - 1);
            break;
        }
    }
    if (realX != -1) cout << "Закрытый ключ x найден: " << realX << endl;
    else cout << "Атака Meet in the Middle не удалась." << endl;
    return realX;
}
int YouNotInSafe(){
    string plaintext;
    int p = RandomNumbers(100000, 5000000);
    while (!prostota(p)) {
        p = RandomNumbers(100000, 5000000);
    }
    int g = generatePrimitiveRoot(p);
    while(g==-1) {
        g = generatePrimitiveRoot(p);
    }
    cout << "p: " << p << endl;
    cout << "g: " << g << endl;
    int x = RandomNumbers(2, p - 1);
    cout << "x = " << x << endl;
    int y = BinCheck(g, x, p);
    int findX = meetInTheMiddleAttack(p, g, y);
    if (x == findX) {
        cout << "Ключи совпадают, значит атака пройдет успешно." << endl;
    }
	return 0;
}
int MITM() {
    int p = RandomNumbers(100000, 5000000);
    while (!prostota(p)){
        p = RandomNumbers(100000, 5000000);
    }
    int g = generatePrimitiveRoot(p);
    while(g==-1){
        g = generatePrimitiveRoot(p);
    } 
    string myText;
    ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        cerr << "Не удалось открыть файл input.txt" << endl;
        return 1;
    }
    string line;
    while (getline(inputFile, line)) 
    {
        myText += line + "\n";
    }
    inputFile.close();
    if (!myText.empty() && myText.back() == '\n') {
        myText.pop_back();
    }
    int bob_x = RandomNumbers(2, p - 1);
    int bob_y = BinCheck(g, bob_x, p);
    int alice_x = RandomNumbers(2, p - 1);
    int alice_y = BinCheck(g, alice_x, p);
    cout << "p: " << p << endl;
    cout << "g: " << g << endl;
    cout << "Публичный ключ Боба: " << bob_y << endl;
    cout << "Публичный ключ Алисы: " << alice_y << endl;
    int mitm_x = RandomNumbers(2, p - 1);
    int mitm_y = BinCheck(g, mitm_x, p);
    cout << "Публичный ключ Евы: " << mitm_y << endl;
    int alice_shared_key = BinCheck(mitm_y, alice_x, p);
    cout << "Серетный ключ Алисы-Евы: " << alice_shared_key << endl;
    int bob_shared_key = BinCheck(mitm_y, bob_x, p);
    cout << "Серетный ключ Боба-Евы: " << bob_shared_key << endl;
    vector<pair<int, int>> ciphertext;
    string encryptedTextString;
    for (unsigned char c : myText) {
        int m = c;
        pair<int, int> encryptedPair = elGamalEncrypt(m, g, mitm_y, p); 
        ciphertext.push_back(encryptedPair);
        encryptedTextString += "(" + to_string(encryptedPair.first) + ", " + to_string(encryptedPair.second) + ") ";
    }
    cout << endl;
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Не удалось открыть файл output.txt для записи" << endl;
        return 1;
    }
    outputFile << "Зашифрованный текст: " << endl << encryptedTextString << endl;
    return 0;
}


