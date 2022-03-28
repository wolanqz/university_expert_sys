#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <stdlib.h> // для atoi()
using namespace std;

const int subnum = 6;
const char* subs[subnum] = {"математика", "русский", "информатика", "обществознание", "география", "физика"};
const int citynum = 2;
const char* cities[citynum] = { "Томск", "Москва"};

/* класс направление подготовки */
class direct {
public:
    const char* name;       // наименование направления
    const char* subject[3]; // комбинация предметов ЕГЭ для поступления
    int price;              // стоимость платного обучения
    int paidScore;          // проходной балл ЕГЭ на ПЛАТНОЕ по предметам в прошлом году
    int freeScore;          // проходной балл ЕГЭ на БЮДЖЕТ по предметам в прошлом году
    direct();
};

direct::direct() {

}

/* класс ВУЗ */
class univer {
public:
    const char* name;       // наименование ВУЗа
    const char* city;       // город
    bool dormitory;         // наличие общежития
    bool military;          // наличие военной кафедры
    int stipend;            // минимальный размер стипендии
    int n;                  // кол-во направлений подготовки ВУЗа
    direct* uniDirects;     // массив объектов класса направление подготовки
    univer();
    friend istream& operator>>(istream& stream, univer& a);
    friend ostream& operator<<(ostream& stream, univer& a);
};

univer::univer() {

}

istream& operator>>(istream& stream, univer& a) {
    
    /* Ввод данных о ВУЗе */
    char* buffn = new char[2];
    stream >> buffn; int n = atoi(buffn);
    char** buff = new char* [n + 5];
    delete[] buffn;
    for (int i = 0; i < n + 5; i++) {
        buff[i] = new char[100];
        stream >> buff[i];
        if (i != 0 and i < n) {
            strcat(buff[0], " ");
            strcat(buff[0], buff[i]);
        }
    }
    a.name = buff[0];
    a.city = buff[n];
    if (atoi(buff[n + 1]) == 1) a.dormitory = true;
    else a.dormitory = false;
    if (atoi(buff[n + 2]) == 1) a.military = true;
    else a.military = false;
    a.stipend = atoi(buff[n + 3]);
    a.n = atoi(buff[n + 4]);
    delete[] buff;

    /* Ввод данных о направлениях подготовки ВУЗа */
    a.uniDirects = new direct[a.n];
    for (int i = 0; i < a.n; i++) {
        char* buffn = new char[2];
        stream >> buffn; int n = atoi(buffn);
        char** buff = new char* [n + 6];
        delete[] buffn;
        for (int j = 0; j < n + 6; j++) {
            buff[j] = new char[100];
            stream >> buff[j];
            if (j != 0 and j < n) {
                strcat(buff[0], " ");
                strcat(buff[0], buff[j]);
            }
        }
        a.uniDirects[i].name = buff[0];
        for (int j = 0; j < 3; j++) a.uniDirects[i].subject[j] = buff[n + j];
        a.uniDirects[i].price = atoi(buff[n + 3]);
        a.uniDirects[i].paidScore = atoi(buff[n + 4]);
        a.uniDirects[i].freeScore = atoi(buff[n + 5]);
        delete[] buff;
    }
    
    return stream;
}

ostream& operator<<(ostream& stream, univer& a) {
    stream << a.name << ' ';
    stream << a.city << ' ';
    stream << a.dormitory << ' ';
    stream << a.military << ' ';
    stream << a.stipend << ' ';
    stream << a.n << '\n';
    for (int i = 0; i < a.n; i++) {
        stream << a.uniDirects[i].name << ' ';
        for (int j = 0; j < 3; j++) stream << a.uniDirects[i].subject[j] << ' ';
        stream << a.uniDirects[i].price << ' ';
        stream << a.uniDirects[i].freeScore << ' ';
        stream << a.uniDirects[i].paidScore << '\n';
    }
    return stream;
}

int main()
{
    setlocale(LC_ALL, "rus");   // корректное отображение кириллицы
    
    /* Заполнение массива данных из файла */
    ifstream fin("bd.txt");
    int unisnum;
    fin >> unisnum;
    univer* unis = new univer[unisnum];
    for (int i = 0; i < unisnum; i++) fin >> unis[i];
    fin.close();

    for (int i = 0; i < unisnum; i++) cout << unis[i];

    /* Алгоритм подбора подходящих направлений и ВУЗов с консольным интерфейсом пользователя */

    int subcode[3]; // номера (+1) интересующих абитуриента предметов из глобального массива
    int score;      // кол-во баллов абитуриента
    int military;   // важно ли наличие военной кафедры
    int city;       // номер города

    cout << "Выберите предметы ЕГЭ из списка: ";
    for (int i = 0; i < subnum; i++) {
        cout << i + 1 << ". " << subs[i] << " ";
    }
    cout << "\nи введите последовательно через пробел номера 3-х предметов\n";
    cin >> subcode[0] >> subcode[1] >> subcode[2];

    cout << "Введите сумму баллов ЕГЭ\n";
    cin >> score;

    cout << "Вам важно наличие военной кафедры в ВУЗе? (0/1)\n";
    cin >> military;

    cout << "Выберите город обучения из списка: ";
    for (int i = 0; i < citynum; i++) {
        cout << i + 1 << ". " << cities[i] << " ";
    }
    cout << "\nи введите его номер, либо 0, если город неважен\n";
    cin >> city;

    /* Подбор направлений с бюджетными местами */
    cout << "Список подходящих направлений обучения:\n";
    int count = 1;
    for (int i = 0; i < unisnum; i++) {
        if (city == 0 || strcmp(unis[i].city, cities[city - 1]) == 0) {
            if ((military == 1 && unis[i].military == true) || military == 0) {
                /* Цикл по массиву направлений подготовки */
                for (int j = 0; j < unis[i].n; j++) {
                    /* Проверка предметов */
                    bool flag1 = true;
                    for (int k = 0; k < 3; k++) {
                        bool flag2 = false;
                        for (int h = 0; h < 3; h++) {
                            if (strcmp(unis->uniDirects[j].subject[h], subs[subcode[k] - 1]) == 0) {
                                flag2 = true;
                                break;
                            }
                        }
                        if (flag2 == false) {
                            flag1 = false;
                            break;
                        }
                    }

                    if (score >= unis[i].uniDirects[j].freeScore) {
                        cout << count << ". " << unis[i].name << " " << unis[i].uniDirects[j].name << " на бюджете\n";
                        count++;
                    }
                    else if (score >= unis[i].uniDirects[j].paidScore) {
                        cout << count << ". " << unis[i].name << " " << unis[i].uniDirects[j].name << " платно за " << unis[i].uniDirects[j].price << "руб/год" << "\n";
                        count++;
                    }
                }
            }
        }
    }
}