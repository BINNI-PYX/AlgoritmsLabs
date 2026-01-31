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
     if (a.key_2.O != b.key_2.O){
        return a.key_2.O < b.key_2.O;
    };
    return a.key_num < b.key_num;
}
bool Greater_App(Key a,Key b){
    return a.key_num < b.key_num;
}
void Application_sort(Key* arr, int n){
    int left = 0;
    int right = n - 1;
    while(left < right){
        int last_sort = left;
        for(int i = left; i < right; i++){
            if(!Greater_App(arr[i],arr[i+1])){
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
            if(!Greater_App(arr[i-1],arr[i])){
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

void Line_Search(Key* arr, int n, int num_app) {
    Key* temp = new Key[n + 1];
    for (int i = 0; i < n; i++) {
        temp[i] = arr[i];
    }
    temp[n].key_num = num_app;

    int i = 0;
    int steps = 0;
    while (temp[i].key_num != num_app) {
        i++;
        steps++;
    }
    delete[] temp;

    if (i == n) {
        cout << "Заявка не найдена" << endl;
    } else {
        cout << i+1 << " строка - первое вхождение" << endl;
        cout << arr[i].key_1.day << " " << arr[i].key_1.month << " " << arr[i].key_1.year << "\t" 
                << arr[i].key_2.F << " " << arr[i].key_2.I << " " << arr[i].key_2.O << "\t"
                << arr[i].key_num << "\t"
                << i + 1<< endl;  
    }

    cout << "Кол-во шагов: " << steps << endl;
}

void Bin_Search(Key* arr, int n,int num_app){
    const int Max_Step = 32;
    int Steps[Max_Step];
    int step_count = 0;

    for (int s = 1;s <= n; s= s * 2){
        Steps[step_count] = s;
        step_count = step_count + 1;
    }
    
    int pos = 0;
    int move_counts = 0;
    for(int k = step_count - 1; k>=0; --k){
        int next_pos = pos + Steps[k];
        if(next_pos < n){
            move_counts++;
            if(arr[next_pos].key_num <= num_app){
            pos = next_pos;
            }
        }
    }
    if (pos < n && arr[pos].key_num == num_app) {
        while (pos > 0 && arr[pos - 1].key_num == num_app) {
            pos--;
        }
        cout << pos + 1<< " строка -первое вхождение" << endl;
        cout << arr[pos].key_1.day << " " << arr[pos].key_1.month << " " << arr[pos].key_1.year << "\t" 
                << arr[pos].key_2.F << " " << arr[pos].key_2.I << " " << arr[pos].key_2.O << "\t"
                << arr[pos].key_num << "\t"
                << pos + 1<< endl;  
    }
    else{
        cout << "Заявка не найдена" << endl;
    }
    cout << "Кол-во шагов: " << move_counts << endl;
}

int main(){
    Key* arr1 = new Key[10000];
    ifstream list1("list1.txt");
    if(!list1.is_open()) {
        return 1;
    }
    int num_app;
    cout << "Введите номер заявки: ";
    cin >> num_app;

    
    int i = 0;
    string line;
    
    while(getline(list1, line) && i < 10000){
        istringstream iss(line);
        iss >> arr1[i].key_1.day >> arr1[i].key_1.month >> arr1[i].key_1.year;
        iss >> arr1[i].key_2.F >> arr1[i].key_2.I >> arr1[i].key_2.O;
        iss >> arr1[i].key_num;
        arr1[i].index = i + 1;
        i++;
    }
    
    list1.close();
    int size = i;

    auto start1 = chrono::steady_clock::now();
    Application_sort(arr1,size);
    auto end1 = chrono::steady_clock::now();
    chrono::duration<double> elapsed_shaker = end1 - start1;

    cout << "Время сортировки: " << elapsed_shaker.count() << " сек" << endl;

    ofstream output1("output_application.txt");
    for(int j = 0; j < size; j++){  
        output1 << arr1[j].key_1.day << " " << arr1[j].key_1.month << " " << arr1[j].key_1.year << "\t" 
                << arr1[j].key_2.F << " " << arr1[j].key_2.I << " " << arr1[j].key_2.O << "\t"
                << arr1[j].key_num << "\t"
                << arr1[j].index << endl;  
    }
    
    auto start2 = chrono::steady_clock::now();
    Bin_Search(arr1,i,num_app);
    auto end2 = chrono::steady_clock::now();
    chrono::duration<double> search_num_key = end2 - start2;

    cout << "Время поиска: " << search_num_key.count() << " сек" << endl;

    auto start3 = chrono::steady_clock::now();
    Line_Search(arr1,i,num_app);
    auto end3 = chrono::steady_clock::now();
    chrono::duration<double> search_all_key = end3- start3;

    cout << "Время поиска: " << search_all_key.count() << " сек" << endl;

    

    output1.close();

    delete[] arr1;
    
    return 0;
}