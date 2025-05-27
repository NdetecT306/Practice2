#include <iostream>
#include <algorithm>
#include <limits>
#include "header.h"
#include "headvec.h"
#include "headrand.h"
#include "headshif.h"
using namespace std;

enum class Task {
    task1 = 1,
    task2,
    task3,
    task4,
    task5,
    task6,
    task7,
    invalid
};
Task getTaskFromInput() {
    int numberOper;
    cout << "Выберите номер задания: ";
    cin >> numberOper;
    switch (numberOper) {
    case 1: return Task::task1;
    case 2: return Task::task2;
    case 3: return Task::task3;
    case 4: return Task::task4;
    case 5: return Task::task5;
    case 6: return Task::task6;
    case 7: return Task::task7;
    default: return Task::invalid;
    }
}
int main() {
    setlocale(LC_ALL, "rus");
    Task selectedTask = getTaskFromInput();
    switch (selectedTask) {
    case Task::task1: {
        int a, x, p;
        try {
            cout << "Введите основание a: ";
            if (!(cin >> a)) throw runtime_error("a");
            cout << "Введите степень x: ";
            if (!(cin >> x)) throw runtime_error("x");
            cout << "Введите p: ";
            if (!(cin >> p)) throw runtime_error("p");
        } catch (const runtime_error& error) {
            cerr << "Ошибка ввода: " << error.what() << " не число." << endl;
            cin.clear();
            return 1;
        }
        (!prostota(p)) ? cout << "p - простое\n" : cout << "p - составное\n";
        if (a >= 1 && (a % p != 0) && (x == p - 1) && prostota(p)) cout << "Теорема Ферма выполняется" << endl;
        else cout << "Теорема Ферма НЕ выполняется" << endl;
        cout << "Проверка бинарным способом: " << BinCheck(a, x, p) << endl;
        break;
    }
    case Task::task2: {
        int c, m;
        int d = FindAnswer(c, m);
        if (d != -1) cout << c << "*" << d << " mod " << m << " =  1" << endl;
        break;
    }
    case Task::task3: {
        int c, m;
        int d = FindAnswer(c, m);
        if (d != -1) cout << c << "^(-1) mod " << m << " = " << d << endl;
        break;
    }
    case Task::task4: {
        ElGamal();
        cout << "Загляните в input.txt и output.txt" <<endl;
        break;
    }
    case Task::task5: {
        LineFrac();
        int a = 1256, b = 847, d = 119;
        cout << "Решение уравнения " << a << "a + " << b << "b = " << d << ":" << endl;
        int u = 0, v = 0;
        int res = Euclidean(a, b, u, v);
        if (d % res != 0) cout << "Уравнение не имеет решений в целых числах." << endl;
        else cout << "x = " << u * (d / res) << ", y = " << v * (d / res) << endl;
        break;
    }
    case Task::task6: {
        YouNotInSafe();
        MITM();
        break;
    }
    case Task::task7: {
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
