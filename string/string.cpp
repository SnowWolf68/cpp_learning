#include<string.h>
#include<assert.h>
#include<iostream>
#include"string.h"

using std::cout;
using std::endl;

// snow::string::string()
//     :str(new char[1])
//     ,size(0)
//     ,capacity(0) {
//     str[0] = '\0';
// }

// snow::string::string()
//     :str(nullptr)
//     ,size(0)
//     ,capacity(0) {

// }

snow::string::string(const char* str)
    :_size(strlen(str))
    ,_capacity(strlen(str))
{
    this->str = new char[_capacity + 1];
    strcpy(this->str, str);
}

snow::string::~string(){
    delete[] str;
    str = nullptr;
    _size = 0;
    _capacity = 0;
}

void snow::string::swap(string& tmp){
    std::swap(str, tmp.str);
    std::swap(_size, tmp._size);
    std::swap(_capacity, tmp._capacity);
}

// 拷贝构造
// snow::string::string(const string& s){
//     str = new char[s._capacity + 1];
//     strcpy(str, s.str);
//     _size = s._size;
//     _capacity = s._capacity;
// }

snow::string::string(const string& s)
    :_size(0)
    ,_capacity(0){
    string tmp(s.str);
    swap(tmp);
    // std::swap(str, tmp.str);
    // std::swap(_size, tmp._size);
    // std::swap(_capacity, tmp._capacity);
}

// 赋值重载
// snow::string& snow::string::operator=(const string& s){
//     if(this != &s){     // 避免自己给自己赋值
//         char* tmp = new char[s.capacity() + 1];
//         strcpy(tmp, s.str);
//         delete[] str;
//         str = tmp;
//         _size = s._size;
//         _capacity = s._capacity;
//     }
    
//     return *this;
// }

// snow::string& snow::string::operator=(const string& s){
//     if(this != &s){     // 避免自己给自己赋值
//         string tmp(s.str);
//         // std::swap(str, tmp.str);
//         // std::swap(_size, tmp._size);
//         // std::swap(_capacity, tmp._capacity);
//     }
//     return *this;
// }

snow::string& snow::string::operator=(string s){
    swap(s);
    return *this;
}

const char* snow::string::c_str() {
    return this->str;
}

size_t snow::string::size() const {
    return _size;
}

size_t snow::string::capacity() const{
    return _capacity;
}

char& snow::string::operator[](size_t pos){
    assert(pos < _size);
    return str[pos];
}

const char& snow::string::operator[](size_t pos) const{
    assert(pos < _size);
    return str[pos];
}

snow::string::iterator snow::string::begin(){
    return str;
}

snow::string::iterator snow::string::end(){
    return str + _size;
}

snow::string::const_iterator snow::string::begin() const {
    return str;
}

snow::string::const_iterator snow::string::end() const {
    return str + _size;
}

void snow::string::reserve(int n){
    if(n > _capacity){
        char* new_str = new char[n + 1];    // '\0'
        /*
        strcpy: 
        源字符串必须以 '\0' 结束。
        • 会将源字符串中的 '\0' 拷⻉到⽬标空间。
        • ⽬标空间必须⾜够⼤，以确保能存放源字符串。
        • ⽬标空间必须可修改。
        */
        strcpy(new_str, str);
        delete[] str;
        str = new_str;
        _capacity = n;
    }
}

void snow::string::push_back(char ch){
    if(_size == _capacity){
        reserve(_capacity == 0 ? 4 : 2 * _capacity);
        // reserve(2 * _capacity);
    }
    str[_size] = ch;
    _size++;
    str[_size] = '\0';
}
void snow::string::append(const char* str){
    int len = strlen(str);
    if(_size + len > _capacity){
        reserve(_size + len);
    }
    /*
    strcpy: 
     源字符串必须以 '\0' 结束。
    • 会将源字符串中的 '\0' 拷⻉到⽬标空间。
    • ⽬标空间必须⾜够⼤，以确保能存放源字符串。
    • ⽬标空间必须可修改。
    */
    strcpy(this->str + _size, str);
    _size += len;
}

snow::string& snow::string::operator+=(char ch){
    this->push_back(ch);
    return *this;
}

snow::string& snow::string::operator+=(const char* str){
    this->append(str);
    return *this;
}

void snow::string::insert(size_t pos, char ch){
    assert(pos <= _size);
    if(_size == _capacity){
        reserve(_capacity == 0 ? 4 : 2 * _capacity);
    }
    for(int i = _size;i >= (int)pos;i--){
        str[i + 1] = str[i];
    }
    str[pos] = ch;
    _size++;
}

void snow::string::insert(size_t pos, const char* str){
    assert(pos <= _size);
    int len = strlen(str);
    if(_size + len > _capacity){
        reserve(_size + len);
    }
    for(int i = _size;i >= (int)pos;i--){
        this->str[i + len] = this->str[i];
    }
    for(int i = pos;i < pos + len;i++){
        this->str[i] = str[i - pos];
    }
    _size += strlen(str);
}

void snow::string::erase(size_t pos, size_t len){
    if(len == npos) len = _size - pos;
    for(int i = pos;i < _size - len;i++){
        str[i] = str[i + len];
    }
    _size -= len;
    str[_size] = '\0';
}

const size_t snow::string::npos = -1;   // 定义, 只能在.cpp中定义, 如果在.h中定义则会导致多次包含时出现重定义

bool snow::string::operator<(const string& s) const { 
    return strcmp(this->str, s.str) < 0;
}
bool snow::string::operator==(const string& s) const {
    return strcmp(this->str, s.str) == 0;
}
bool snow::string::operator<=(const string& s) const { 
    return *this < s || *this == s;
}
bool snow::string::operator>(const string& s) const {
    return !(*this <= s);
}
bool snow::string::operator>=(const string& s) const {
    return !(*this < s);
}

std::ostream& snow::operator<<(std::ostream& out, const snow::string &s){
    for(auto ch : s){   // 由于参数s是const引用, 因此如果想用范围for, 必须提供const类型的迭代器
        out << ch;
    }
    return out;
}

// std::istream& snow::operator>>(std::istream& in, snow::string &s){
//     s.clear();
//     char ch;
//     // in >> ch;    // in拿不到空格
//     ch = in.get();
//     while(ch != ' ' && ch != '\n'){
//         s += ch;
//         ch = in.get();
//     }
//     return in;
// }

// 改进cin, 避免多次扩容
// 使用缓冲区思想
std::istream& snow::operator>>(std::istream& in, snow::string &s){
    s.clear();

    char buf[129];
    int idx = 0;

    char ch;
    ch = in.get();
    while(ch != ' ' && ch != '\n'){
        buf[idx++] = ch;
        if(idx == 128){
            buf[idx] = '\0';
            s += buf;
            idx = 0;
        }
        ch = in.get();
    }
    if(idx != 0){
        buf[idx] = '\0';
        s += buf;
    }
    return in;
}

void snow::string::clear(){
    _size = 0;
    str[0] = '\0';
}

void snow::string::resize(int n, char ch){
    if(n <= _size){
        str[n] = '\0';
    }else{
        reserve(n);
        for(int i = _size;i < n;i++){
            str[i] = ch;
        }
        str[n] = '\0';
    }
    _size = n;
}