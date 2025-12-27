#include<string.h>
#include<assert.h>
#include"string.h"

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

