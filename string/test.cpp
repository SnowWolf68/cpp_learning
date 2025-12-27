#include<iostream>
#include"string.h"
using namespace std;

int main(){
    const char* s = "string";
    snow::string str(s);
    cout << "=====构造函数========" << endl;
    cout << str.c_str() << endl;

    cout << "=====重载[]========" << endl;

    cout << str[0] << endl;
    str[0] = '1';
    cout << str.c_str() << endl;

    for(int i = 0;i < str.size();i++){
        cout << str[i] << " ";
    }
    cout << endl;

    const snow::string str_const(s);
    cout << str_const[0] << endl;

    cout << "=====迭代器========" << endl;
    snow::string::iterator it = str.begin();
    while(it != str.end()){
        cout << *it << " ";
        it++;
    }
    cout << endl;
    it = str.begin();
    while(it != str.end()){
        (*it)++;
        cout << *it << " ";
        it++;
    }
    cout << endl;

    cout << "=====范围for========" << endl;     // 底层用迭代器实现, 迭代器的两个接口必须是begin(), end()才能实现
    for(auto ch : str){
        cout << ch << " ";
    }
    cout << endl;

    return 0;
}
// g++ test.cpp string.cpp -o test ; ./test.exe