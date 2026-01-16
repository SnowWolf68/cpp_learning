#include<iostream>
#include"string.h"
using namespace std;

void test01(){
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

    cout << "=====push_back, append========" << endl;
    cout << str.c_str() << endl;
    cout << "push_back(a) " << str.size() << " " << str.capacity() << endl;
    str.push_back('a');
    cout << "push_back(a) " << str.size() << " " << str.capacity() << endl;
    str.push_back('b');
    cout << "push_back(a) " << str.size() << " " << str.capacity() << endl;
    str.push_back('c');
    cout << "push_back(a) " << str.size() << " " << str.capacity() << endl;

    str.append("qqqq111111111111111");
    cout << "append " << str.size() << " " << str.capacity() << endl;

    cout << str.c_str() << endl;

    cout << "=====重载+= ========" << endl;
    str += '#';
    (str += '#')+= '^';
    str += "&&&&";
    cout << str.c_str() << endl;

    cout << "===== 空串的capacity为0, 有bug ========" << endl;
    snow::string str2;
    cout << "capacity: " << str2.capacity() << endl;
    str2.push_back('1');
    str2 += "%%%%%%%%%%%%%%%%%%%%";
    cout << str2.c_str() << endl;
    cout << "capacity: " << str2.capacity() << endl;

    cout << "npos: " << snow::string::npos << endl;
}

void test02(){
    snow::string str("string");
    str.insert(5, 'a');
    cout << str.c_str() << endl;
    str.insert(str.size(), 'a');
    cout << str.c_str() << endl;
    str.insert(0, 'a');
    cout << str.c_str() << endl;

    str.insert(0, "qqqq");
    cout << str.c_str() << endl;


    // str.erase(2, snow::string::npos);
    str.erase(2, 3);
    cout << str.c_str() << endl;

    str.erase(0, 2);
    cout << str.c_str() << endl;

    str.erase(3, snow::string::npos);
    cout << str.c_str() << endl;
}

void test03(){
    snow::string s("aaa");
    cout << s << endl;
    cin >> s;
    cout << s << endl;

    s.resize(10, 'a');
    cout << s << endl;
    s.resize(3, 'a');
    cout << s << endl;
}

void test04() {
    // g++允许修改非法位置, vs2022会报错
    char s[] = "hello";
    s[5] = '1';
    s[6] = '2';
    s[7] = '\0';
    cout << s << endl;
}

void test05(){
    snow::string s;
    cin >> s;
    cout << s << endl;
}

int main(){
    
    test03();
    // test04();

    // test05();
    
    

    return 0;
}
// g++ test.cpp string.cpp -o test ; ./test.exe