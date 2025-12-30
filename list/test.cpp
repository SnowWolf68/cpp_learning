#include <iostream>
#include "list.h"

using namespace std;

void test01(){
    snow::list<int> lt;

    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    lt.push_front(6);
    lt.push_front(6);
    lt.push_front(6);

    snow::list<int>::iterator it = lt.begin();
    while(it != lt.end()){
        cout << *it << " ";
        // *it += 20;
        // cout << *it << " ";
        it++;
    }
    cout << endl;

    for(int& x : lt){
        x++;
        cout << x << " ";
    }
    cout << endl;


    cout << "========== clear ==========" << endl;
    lt.clear();

    for(int& x : lt){
        cout << x << " ";
    }
    cout << endl;

}

template <class T>
void print(snow::list<T> lt){
    for(auto x : lt){
        cout << x << " ";
    }
    cout << endl;
}

void test02(){
    snow::list<int> lt1;

    lt1.push_back(1);
    lt1.push_back(2);
    lt1.push_back(3);
    lt1.push_back(4);
    lt1.push_back(5);

    lt1.push_front(6);
    lt1.push_front(6);
    lt1.push_front(6);

    print(lt1);
    
    snow::list<int> lt2(lt1);
    print(lt2);
    
    snow::list<int> lt3;
    lt3 = lt1;
    print(lt3);
}

void test03(){
    
}

int main(){

    test02();

    return 0;
}