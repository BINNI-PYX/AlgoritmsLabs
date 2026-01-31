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
    int index;
};

bool Greater(Key a, Key b) {
    if (a.key_1.year != b.key_1.year){
        return a.key_1.year > b.key_1.year;
    };
    if (a.key_1.month != b.key_1.month){
        return a.key_1.month > b.key_1.month;
    };
    if (a.key_1.day != b.key_1.day){
        return a.key_1.day > b.key_1.day;
    };
    if (a.key_2.F != b.key_2.F){
        return a.key_2.F < b.key_2.F;
    };
    if (a.key_2.I != b.key_2.I){
        return a.key_2.I < b.key_2.I;
    };
    return a.key_2.O <= b.key_2.O;
}

void Shakeup_sort(Key* arr, int n){
    int left = 0;
    int right = n - 1;
    while(left < right){
        int last_sort = left;
        for(int i = left; i < right; i++){
            if(!Greater(arr[i],arr[i+1])){
                Key temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                last_sort = i;
            }
        }
        right = last_sort;
        if(last_sort == 0 && left == 0){
            break;
        }
        last_sort = 0;
        for(int i = right; i > left; i--){
            if(!Greater(arr[i-1],arr[i])){
                Key temp = arr[i-1];
                arr[i-1] = arr[i];
                arr[i] = temp;
                last_sort = i;
            }
        }
        left = last_sort;
        if(last_sort == 0){
            break;
        }
    }
}

void Shella_sort(Key* arr,int n){
    int h = 1;
    while(h < n/3){
        h = 3*h+1;
    }
    while(h >= 1){
        for(int i = h; i < n; i++){
            Key temp = arr[i];
            int j = i;
            while(j >= h && Greater(temp,arr[j - h])){
              arr[j] = arr[j-h];
              j-=h;  
            }
            arr[j] = temp;
        }
        h = h/3;
    }
}

int main(){
    Key* arr1 = new Key[10000];
    Key* arr2 = new Key[10000]; 
    ifstream list1("list1.txt");
    if(!list1.is_open()) {
        return 1;
    }

    int i = 0;
    string line;
    
    while(getline(list1, line) && i < 10000){
        istringstream iss(line);
        iss >> arr1[i].key_1.day >> arr1[i].key_1.month >> arr1[i].key_1.year;
        iss >> arr1[i].key_2.F >> arr1[i].key_2.I >> arr1[i].key_2.O;
        arr1[i].index = i + 1;
        arr2[i] = arr1[i];
        i++;
    }
    
    list1.close();
    int size = i;


    auto start1 = chrono::steady_clock::now();
    Shakeup_sort(arr1, size);
    auto end1 = chrono::steady_clock::now();
    chrono::duration<double> elapsed_shaker = end1 - start1;

    auto start2 = chrono::steady_clock::now();
    Shella_sort(arr2, size);
    auto end2 = chrono::steady_clock::now();
    chrono::duration<double> elapsed_shell = end2 - start2;

    cout << "Время Shake: " << elapsed_shaker.count() << " сек" << endl;
    cout << "Время Shella: " << elapsed_shell.count() << " сек" << endl;

    ofstream output1("output_shaker.txt");
    ofstream output2("output_shell.txt");
    
    for(int j = 0; j < size; j++){  
        output1 << arr1[j].key_1.day << " " << arr1[j].key_1.month << " " << arr1[j].key_1.year << "\t" 
                << arr1[j].key_2.F << " " << arr1[j].key_2.I << " " << arr1[j].key_2.O << "\t" 
                << arr1[j].index << endl;  
    }
    
    for(int j = 0; j < size; j++){  
        output2 << arr2[j].key_1.day << " " << arr2[j].key_1.month << " " << arr2[j].key_1.year << "\t" 
                << arr2[j].key_2.F << " " << arr2[j].key_2.I << " " << arr2[j].key_2.O << "\t" 
                << arr2[j].index << endl;  
    }
    
    output1.close();
    output2.close();
    
    delete[] arr1;
    delete[] arr2;
    
    return 0;
}