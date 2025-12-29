#include <iostream>
#include <assert.h>
#include "vector.h"

using namespace std;

template <class T>
void print(snow::vector<T> v){     // 如果值传递, 那么由于memcpy是浅拷贝, 那么v中的string会被析构两次, 导致崩溃
    for(auto x : v){
        cout << x << " ";
    }
    cout << endl;
}


void test01(){
    snow::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);

    for(int i = 0;i < vec.size();i++){
        cout << vec[i] << " ";
    }
    cout << endl;

    snow::vector<int>::iterator it = vec.begin();
    while(it != vec.end()){
        *it *= 10;
        cout << *it << " ";
        it++;
    }
    cout << endl;

    for(int &x : vec){
        cout << x << " ";
    }
    cout << endl;

    // const
    cout << "---------const---------" << endl;

    const snow::vector<int> v2 = vec;

    for(const int &x : v2){
        // x++;
        cout << x << " ";
    }
    cout << endl;

    for(int i = 0;i < v2.size();i++){
        // v2[i]++;
        cout << v2[i] << " ";
    }
    cout << endl;

    cout << "--------- resize ---------" << endl;

    snow::vector<int> v3;
    v3.push_back(1);
    v3.push_back(2);
    v3.push_back(3);
    v3.push_back(4);
    v3.push_back(5);

    v3.resize(3);
    print(v3);

    v3.resize(10, 1);
    print(v3);

    snow::vector<string> v4;
    v4.resize(10, "xxx");
    print(v4);

    snow::vector<int*> v5;
    v5.resize(5);
    print(v5);
}


void test02(){
    int i = int();
    double d = double();
    cout << i << endl;
    cout << d << endl;
    int j;
    cout << j << endl;
}

void test03(){
    snow::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);

    // v1.insert(v1.end(), 30);
    // print(v1);

    v1.push_back(22);
    print(v1);
    
    // v1.erase(v1.end());
    // print(v1);
    v1.erase(v1.end() - 1);
    print(v1);
}

void test04(){
    snow::vector<int> v;

    // v.push_back(1);
    // v.push_back(2);
    // v.push_back(3);
    // v.push_back(4);
    // v.push_back(5);
    // v.push_back(6);

    v.push_back(2);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    print(v);

    auto it = v.begin();
    while(it < v.end()){
        while(it < v.end() && *it % 2 == 0){
            v.erase(it);
        }
        it++;
    }
    print(v);
}

void test05(){
    snow::vector<int> v1;

    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);
    v1.push_back(6);

    snow::vector<int> v2(v1.begin(), v1.end());

    print(v2);
    
    // string str = "hello vector";
    // snow::vector<int> v3(str.begin(), str.end());
    
    // print(v3);
}

void test06(){
    snow::vector<string> v1(10, "xx");
    snow::vector<int> v2(10, 0);

    print(v1);
    print(v2);
}

int main(){

    // test01();

    // test02();

    test06();

    return 0;
}
