namespace snow
{
    class string
    {
        public:
        typedef char* iterator;

        iterator it;

        // string();
        string(const char* str = "");
        ~string();
        const char *c_str();
        size_t size() const;
        size_t capacity() const;
        char& operator[](size_t pos);
        const char& operator[](size_t pos) const;   // 用于const对象调用
        iterator begin();  // 字符指针模拟迭代器
        iterator end();

    private:
        char *str;
        size_t _size;
        size_t _capacity;
    };
}