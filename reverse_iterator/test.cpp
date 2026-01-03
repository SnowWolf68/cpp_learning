#include<iostream>
#include"list.h"

using namespace std;

template <class T>
void print_list(const snow::list<T> lt){
    typename snow::list<T>::const_iterator it = lt.begin();
    while(it != lt.end()){
        cout << *it << " ";
        it++;
    }
    cout << endl;

    typename snow::list<T>::const_reverse_iterator rit = lt.rbegin();
    while(rit != lt.rend()){
        cout << *rit << " ";
        rit++;
    }
    cout << endl;
}

void test01() {
    snow::list<int> lt;
    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    snow::list<int>::iterator it = lt.begin();
    while (it != lt.end()) {
        cout << *it << " ";
        it++;
    }
    cout << endl;

    snow::list<int>::reverse_iterator rit = lt.rbegin();
    while (rit != lt.rend()) {
        cout << *rit << " ";
        rit++;
    }
    cout << endl;

    print_list(lt);

}

int main() {

    test01();

    return 0;
}