#include <iostream>
#include <random>

using namespace std;
class Node{
public:
    int data;
    Node* next;
    Node* prev;

    Node(int value){//Конструктор
        data = value;
        next = nullptr;
        prev = nullptr;
    }
};
struct AdressInventory{
        Node* adress;
        AdressInventory* next;
    };
class Spisok{
private:
    Node* head;
    Node* tail;
public:
    Spisok(){
        head = nullptr;
        tail = nullptr;
    }

    void InsertElem(int value){
        Node* newNode = new Node(value);
        if (!head){
            head = tail = newNode;
            return;
        }
        Node* current = head;
        while(current && current->data < value){//бежим бегунком по списку и сравниваем новый элемент c другими 
            current = current->next;
        }
        if (!current){
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else if (current == head){
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else{
            Node* prevNode = current->prev; // Сохраняем указатель на предыдущий узел чтобы вставить новый элемент перед текущим.
            prevNode->next = newNode;
            newNode->prev = prevNode;
            newNode->next = current;
            current->prev = newNode;
        }
    }

    void DeletePrevElem(int value){
        Node* current = head;
        while(current){
            if (current->data == value && current->prev){
                Node* DeleteElem = current->prev; // связь с предыдущим элементом 
                if (DeleteElem->prev){
                    DeleteElem->prev->next = current;// элемент который стоит перед удаляемым элементом, связываем с элементом стоящим после удаляемого элемента
                    current->prev = DeleteElem->prev;
                }
                else{
                    head = current;
                    current->prev = nullptr;
                }
                delete DeleteElem;
            }
            current = current->next;
        }
    }

    void DeleteAllKey(int value){
        Node* current = head;

        while(current){
            if (current->data == value){
                Node* DeleteElem = current;
                if(DeleteElem->prev){
                    DeleteElem->prev->next = DeleteElem->next;
                }
                else{
                    head = DeleteElem->next;
                }
                if (DeleteElem->next){
                    DeleteElem->next->prev = DeleteElem->prev;
                }
                else{
                    tail = DeleteElem->prev;
                }
                current = DeleteElem->next;
                delete DeleteElem;
            }
            else{
                current = current->next;
            }
        }
    }
    AdressInventory* Search(int value) {
        Node* current = head;
        AdressInventory* result = new AdressInventory; // Создаём новый список
        AdressInventory* current_result = result; // Создаём указатель в нём
        while(current){
            if(current->data == value){
                current_result->adress = current;
                AdressInventory* temp = new AdressInventory;// Создаём новую память для нового
                current_result->next = temp;
                current_result = current_result->next;
            }
            current = current->next;
        }
        current_result = nullptr;
        delete current_result;
        return result;
    }

    void PrintSpisok(){
        Node* current = head;
        if (!current) {
            cout << "Список пуст" << endl;
            return;
        }
        cout << "Список: ";
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void DifferenceSpisok(Spisok& other) {
    Node* curA = head;
    Node* curB = other.head;

    while (curA && curB) {
        if (curA->data < curB->data) {
            curA = curA->next;
        }
        else if (curA->data == curB->data) {
            Node* DeleteElem = curA;
            curA = curA->next;

            if (DeleteElem->prev)
                DeleteElem->prev->next = DeleteElem->next;
            else
                head = DeleteElem->next;

            if (DeleteElem->next)
                DeleteElem->next->prev = DeleteElem->prev;
            else
                tail = DeleteElem->prev;

            delete DeleteElem;
        }
        else {
            curB = curB->next;
        }
    }
}


    ~Spisok(){
        Node* current = head;
        while(current){
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};

int main(){
    Spisok a;
    Spisok listB;
    for(int i = 0; i < 10; ++i){
        a.InsertElem(5);
    }
    a.PrintSpisok();
    AdressInventory* result = new AdressInventory;
    result = a.Search(5);
    for(int i = 0; i < 10; ++i){
        cout << result->adress << " ";
        cout << result->adress->data << endl;
        AdressInventory* temp = result;
        result = result->next;
        delete temp;
    }
    return 0;
}