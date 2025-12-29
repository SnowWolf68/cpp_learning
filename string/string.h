#include<iostream>
namespace snow
{
    class string
    {
        public:
        typedef char* iterator;
        typedef const char* const_iterator;


        // string();
        string(const char* str = "");
        ~string();

        void swap(string& s);
        // 拷贝构造
        string(const string& s);
        // 赋值重载
        // string& operator=(const string& s);
        string& operator=(string s);
        const char *c_str();
        size_t size() const;
        size_t capacity() const;
        char& operator[](size_t pos);
        const char& operator[](size_t pos) const;   // 用于const对象调用
        iterator begin();  // 字符指针模拟迭代器
        iterator end();
        const_iterator begin() const;  // 字符指针模拟迭代器
        const_iterator end() const;
        void reserve(int n);
        void push_back(char ch);
        void append(const char* str);
        string& operator+=(char ch);
        string& operator+=(const char* str);
        void insert(size_t pos, char ch);
        void insert(size_t pos, const char* str);
        void erase(size_t pos, size_t len);
        
        const static size_t npos;   // 声明

        bool operator<(const string& s) const ;
        bool operator==(const string& s) const ;
        bool operator<=(const string& s) const ;
        bool operator>(const string& s) const ;
        bool operator>=(const string& s) const ;
        
        void clear();
        void resize(int n, char ch = '\0');
        
        
        private:
        char *str;
        size_t _size;   // _size, _capacity 均不包含'\0'
        size_t _capacity;
        
    };

    std::ostream& operator<<(std::ostream& out, const snow::string &s);
    std::istream& operator>>(std::istream& in, snow::string &s);
}

