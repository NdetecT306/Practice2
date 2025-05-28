#include <iostream>
#include <limits>
#include "headGMP.h"
#include "header.h"
#include "headvec.h"
#include "headrand.h"
#include "headshif.h"
using namespace std;
enum class Task {
    FermaCheck = 1,
    Eucklid_1,
    Eucklid_d,
    ShifrElGamal,
    Equation,
    Errors,
    Letter,
    invalid
};
Task getTaskFromInput() {
    int numberOper;
    cout << "Выберите номер задания: ";
    cin >> numberOper;
    switch (numberOper) {
    case 1: return Task::FermaCheck;
    case 2: return Task::Eucklid_1;
    case 3: return Task::Eucklid_d;
    case 4: return Task::ShifrElGamal;
    case 5: return Task::Equation;
    case 6: return Task::Errors;
    case 7: return Task::Letter;
    default: return Task::invalid;
    }
}
int main() {
    setlocale(LC_ALL, "rus");
    Task selectedTask = getTaskFromInput();
    switch (selectedTask) {
    case Task::FermaCheck: {
        mpz_t a, x, p, result;
        mpz_init(a);
        mpz_init(x);
        mpz_init(p);
        mpz_init(result);
        try {
            cin.ignore();
            cout << "Введите основание a: ";
            string a_str;
            if (!getline(cin, a_str)) throw runtime_error("a");
            if (mpz_set_str(a, a_str.c_str(), 10) != 0) throw runtime_error("a");
            cout << "Введите степень x: ";
            string x_str;
            if (!getline(cin, x_str)) throw runtime_error("x");
            if (mpz_set_str(x, x_str.c_str(), 10) != 0) throw runtime_error("x");
            cout << "Введите p: ";
            string p_str;
            if (!getline(cin, p_str)) throw runtime_error("p");
            if (mpz_set_str(p, p_str.c_str(), 10) != 0) throw runtime_error("p");
        } catch (const runtime_error& error) {
            cerr << "Ошибка ввода: " << error.what() << " не число." << endl;
            cin.clear();
            mpz_clear(a);
            mpz_clear(x);
            mpz_clear(p);
            mpz_clear(result);
            return 1;
        }
        if (!prostota_gmp(p)) cout << "p - составное\n";
        else cout << "p - простое\n";
        mpz_t p_minus_1, a_mod_p, one;
        mpz_init(p_minus_1);
        mpz_sub_ui(p_minus_1, p, 1); 
        mpz_init(a_mod_p);
        mpz_mod(a_mod_p, a, p); 
        mpz_init_set_ui(one, 1);
        if (mpz_cmp(a, one)>=0 && mpz_cmp_ui(a_mod_p, 0)!=0 && mpz_cmp(x, p_minus_1)==0 && prostota_gmp(p)) cout << "Теорема Ферма выполняется" << endl;
        else cout << "Теорема Ферма НЕ выполняется" << endl;
        mpz_clear(one);
        Bin(result, a, x, p);
        cout << "Проверка бинарным способом: ";
        gmp_printf("%Zd\n", result);
        mpz_clear(a);mpz_clear(x);
        mpz_clear(p);
        mpz_clear(result);
        mpz_clear(p_minus_1);
        mpz_clear(a_mod_p);
        break;
    }
    case Task::Eucklid_1: {
        int c, m;
        int d = FindAnswer(c, m);
        if (d != -1) cout << c << "*" << d << " mod " << m << " =  1" << endl;
        break;
    }
    case Task::Eucklid_d: {
        int c, m;
        int d = FindAnswer(c, m);
        if (d != -1) cout << c << "^(-1) mod " << m << " = " << d << endl;
        break;
    }
    case Task::ShifrElGamal: {
        ElGamal();
        //ElGamal_gmp();
        cout << "Взгляните в input.txt и output.txt" << endl;
        break;
    }
    case Task::Equation: {
        LineFrac();
        int a = 1256, b = 847, d = 119;
        cout << "Решение уравнения " << a << "a + " << b << "b = " << d << ":" << endl;
        int u = 0, v = 0;
        int res = Euclidean(a, b, u, v);
        if (d % res != 0) cout << "Уравнение не имеет решений в целых числах." << endl;
        else cout << "x = " << u * (d / res) << ", y = " << v * (d / res) << endl;
        break;
    }
    case Task::Errors: {
        YouNotInSafe();
        MITM();
        break;
    }
    case Task::Letter: {
        cout << "Сообщение лежит в отчете." << endl;
        break;
    }
    case Task::invalid: {
        cout << "Неверный номер задания." << endl;
        break;
    }
    }
    return 0;
}
