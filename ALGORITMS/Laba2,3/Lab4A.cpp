#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

struct FIO
{
    string F;
    string I;
    string O;
};

struct Key{
    Date key_1;
    FIO key_2;
    int key_num;
    int index;
    string description;
};


int simple_hash(string& s) {
    int sum = 0;
    for (int i = 0; i < s.size(); i++) {
        sum += s[i];
    }
    return sum;
}

int rabin_karp_count(string& text, string& pattern) {
    int n = text.size();
    int m = pattern.size();
     if (m == 0 || n < m) 
        return 0;

    int pattern_hash = simple_hash(pattern);
    string first_window = text.substr(0, m);
    int window_hash = simple_hash(first_window);
    int count = 0;

    for (int i = 0; i <= n - m; i++) {
        if (window_hash == pattern_hash) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) 
                count++;
        }

        if (i < n - m) {
            window_hash = window_hash - text[i] + text[i + m];
        }
    }

    return count;
}

int bmh_count(string& text, string& pattern) { 
    int m = pattern.size();
    int n = text.size();
     if (m == 0 || n < m) 
        return 0;

    int skip[256];
    for (int i = 0; i < 256; i++) 
        skip[i] = m;

    for (int i = 0; i < m - 1; i++) 
        skip[(unsigned char)pattern[i]] = m - 1 - i;

    int count = 0;
    int i = 0;

    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0 && text[i + j] == pattern[j]) 
            j--;

        if (j < 0) {
            count++;
            i += 1;
        } else {
            i += skip[(unsigned char)text[i + m - 1]];
        }
    }

    return count;
}

bool Choise(Key& Struct, string& pattern, int min_total, bool flag) {
    string full_fio = Struct.key_2.F + " " + Struct.key_2.I + " " + Struct.key_2.O;

    int fio_count;      
    int desc_count;     
    if (flag) {
        fio_count = rabin_karp_count(full_fio, pattern);
        desc_count = rabin_karp_count(Struct.description, pattern);
    } else {
        fio_count = bmh_count(full_fio, pattern);
        desc_count = bmh_count(Struct.description, pattern);
    }

    int total = fio_count;
    if (total < min_total) {
        total += desc_count;
    }

    return (total >= min_total);
}


void run_search(Key* arr, int size, string& pattern, int min_total, bool use_rk) {
    for (int j = 0; j < size; j++) {
        Choise(arr[j], pattern, min_total, use_rk);
    }
}

int main(){
    Key* arr1 = new Key[1000000];

    string pattern;
    int min_total;

    cout << "Введите шаблон для поиска: " << endl;
    getline(cin, pattern);
    cout << "Введите общее число вхождений: " << endl;
    cin >> min_total;
    cin.ignore();

    ifstream list1("list1.txt");
    if(!list1.is_open()) {
        return 1;
    }

    
    int i = 0;
    string line;
    
    while(getline(list1, line) && i < 1000000){
        istringstream iss(line);
        iss >> arr1[i].key_1.day >> arr1[i].key_1.month >> arr1[i].key_1.year;
        iss >> arr1[i].key_2.F >> arr1[i].key_2.I >> arr1[i].key_2.O;
        iss >> arr1[i].key_num;
        string temporary;
        getline(iss, temporary);
        arr1[i].description = temporary.substr(1);
        arr1[i].index = i + 1;
        i++;
    }
    
    list1.close();
    int size = i;

    chrono::steady_clock::time_point start1 = chrono::steady_clock::now();
    run_search(arr1, size, pattern, min_total, true); 
    chrono::steady_clock::time_point end1 = chrono::steady_clock::now();
    chrono::duration<double> time_rk = end1 - start1;


    chrono::steady_clock::time_point start2 = chrono::steady_clock::now();
    run_search(arr1, size, pattern, min_total, false);
    chrono::steady_clock::time_point end2 = chrono::steady_clock::now();
    chrono::duration<double> time_bmx = end2 - start2;

    ofstream output1("RK.txt");
    int rk_count = 0;
    for (int j = 0; j < size; j++) {
        if (Choise(arr1[j], pattern, min_total, true)) {
            output1 << arr1[j].key_1.day << " " << arr1[j].key_1.month << " " << arr1[j].key_1.year << "\t" 
                    << arr1[j].key_2.F << " " << arr1[j].key_2.I << " " << arr1[j].key_2.O << "\t"
                    << arr1[j].key_num << "\t"
                    << arr1[j].description << "\t"
                    << arr1[j].index << endl;
            rk_count++;
        }
    }
    if (rk_count > 0) {
        output1 << "Время выполнения РК: " << time_rk.count() << " сек" << endl;
    }
    output1.close();

    ofstream output2("BMX.txt");
    int bmx_count = 0;
    for (int j = 0; j < size; j++) {
        if (Choise(arr1[j], pattern, min_total, false)) {
            output2 << arr1[j].key_1.day << " " << arr1[j].key_1.month << " " << arr1[j].key_1.year << "\t" 
                    << arr1[j].key_2.F << " " << arr1[j].key_2.I << " " << arr1[j].key_2.O << "\t"
                    << arr1[j].key_num << "\t"
                    << arr1[j].description << "\t"
                    << arr1[j].index << endl;
            bmx_count++;
        }
    }
    if (bmx_count > 0) {
        output2 << "Время выполнения БМХ: " << time_bmx.count() << " сек" << endl;
    }
    output2.close();

    delete[] arr1;
    return 0;
}