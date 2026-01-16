#include <iostream>
#include<string>
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

class A{
public:
    A(int a1 = 0, int a2 = 0)
        :_a1(a1)
        ,_a2(a2)
    {}

    int _a1;
    int _a2;
};

void test03(){
    snow::list<A> lt;
    lt.push_back(A(1, 1));
    lt.push_back(A(2, 2));
    lt.push_back(A(3, 3));
    lt.push_back(A(4, 4));

    snow::list<A>::iterator it = lt.begin();
    while(it != lt.end()){
        cout << it->_a1 << " " << it->_a2 << endl;
        cout << it.operator->()->_a1 << " " << it.operator->()->_a2 << endl;
        it++;
    }
}

// // template <class T>
// template <typename T>
// void print_list(const snow::list<T> &lt){
//     typename snow::list<T>::const_iterator cit = lt.begin();
//     // auto cit = lt.begin();
//     while(cit != lt.end()){
//         // (*cit)++;
//         cout << *cit << " ";
//         cit++;
//     }
//     cout << endl;
//     for(auto x : lt){
//         cout << x << " ";
//     }
//     cout << endl;
// }


template <class container>
void print_container(const container& con){
    typename container::const_iterator it = con.begin();
    // auto cit = lt.begin();
    while(it != con.end()){
        // (*cit)++;
        cout << *it << " ";
        it++;
    }
    cout << endl;
    for(auto x : con){
        cout << x << " ";
    }
    cout << endl;
}

void test04(){
    snow::list<int> lt;

    lt.push_back(1);
    lt.push_back(2);
    lt.push_back(3);
    lt.push_back(4);
    lt.push_back(5);

    print_container(lt);

    vector<string> lt1;
    lt1.push_back("11111111111");
    lt1.push_back("11111111111");
    lt1.push_back("11111111111");
    lt1.push_back("11111111111");
    lt1.push_back("11111111111");

    print_container(lt1);
}

int main(){

    test04();

    return 0;
}