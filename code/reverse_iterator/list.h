#include "reverse_iterator.h"
namespace snow {
    template <class T>
    struct list_node {
        T _data;
        list_node<T>* _next;
        list_node<T>* _prev;

        list_node(const T& x = T())
            :_data(x)
            , _next(nullptr)
            , _prev(nullptr)
        {}
    };


    // 封装迭代器
    template <class T, class Ref, class Ptr>
    struct __list_iterator {
        typedef list_node<T> Node;
        Node* _node;

        __list_iterator(Node* node) {
            _node = node;
        }

        // 前置++
        __list_iterator& operator++() {
            _node = _node->_next;
            return *this;
        }

        // 后置++
        // 注意要返回++之前的值
        __list_iterator operator++(int) {
            __list_iterator old = *this;
            _node = _node->_next;
            return old;
        }

        __list_iterator& operator--() {
            _node = _node->_prev;
            return *this;
        }

        __list_iterator operator--(int) {
            __list_iterator old = *this;
            _node = _node->_prev;
            return old;
        }

        Ref operator*() {
            return _node->_data;
        }

        Ptr operator->() {
            return &(_node->_data);
        }

        bool operator!=(const __list_iterator& it) {
            return _node != it._node;
        }

        bool operator==(const __list_iterator& it) {
            return _node == it._node;
        }
    };

    // template <class T>
    // struct __list_const_iterator{
    //     typedef list_node<T> Node;
    //     Node* _node;

    //     __list_const_iterator(Node* node){
    //         _node = node;
    //     }

    //     // 前置++
    //     __list_const_iterator& operator++(){
    //         _node = _node->_next;
    //         return *this;
    //     }

    //     // 后置++
    //     // 注意要返回++之前的值
    //     __list_const_iterator operator++(int){
    //         __list_const_iterator old = *this;
    //         _node = _node->_next;
    //         return old;
    //     }

    //     __list_const_iterator& operator--(){
    //         _node = _node->_prev;
    //         return *this;
    //     }

    //     __list_const_iterator operator--(int){
    //         __list_const_iterator old = *this;
    //         _node = _node->_prev;
    //         return old;
    //     }

    //     const T& operator*(){
    //         return _node->_data;
    //     }

    //     const T* operator->(){
    //         return &(_node->_data);
    //     }

    //     bool operator!=(const __list_const_iterator& it){
    //         return _node != it._node;
    //     }

    //     bool operator==(const __list_const_iterator& it){
    //         return _node == it._node;
    //     }
    // };


    template <class T>
    class list {
    public:
        typedef list_node<T> Node;
        // typedef __list_iterator<T> iterator;
        // typedef __list_const_iterator<T> const_iterator;

        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, const T&, const T*> const_iterator;

        typedef __reverse_iterator<iterator, T&, T*> reverse_iterator;
        typedef __reverse_iterator<const_iterator, const T&, const T*> const_reverse_iterator;
        // typedef __reverse_iterator<iterator, const T&, const T*> const_reverse_iterator;


        void empty_init() {
            _head = new Node();
            _head->_next = _head;
            _head->_prev = _head;
            _size = 0;
        }

        list() {
            empty_init();
        }

        void push_back(const T& x) {
            // Node* newNode = new Node(x);
            // Node* tail = _head->_prev;
            // tail->_next = newNode;
            // newNode->_prev = tail;
            // newNode->_next = _head;
            // _head->_prev = newNode;
            insert(end(), x);
        }

        void push_front(const T& x) {
            insert(begin(), x);
        }

        void pop_front() {
            erase(begin());
        }

        void pop_back() {
            erase(end()--);
        }

        const_iterator begin() const {
            return _head->_next;    // 隐式类型转换
        }

        const_iterator end() const {
            return _head;
        }

        iterator begin() {
            // return iterator(_head->_next);
            return _head->_next;    // 隐式类型转换
        }

        iterator end() {
            // return iterator(_head);
            return _head;
        }

        reverse_iterator rbegin() {
            return --end();
        }

        reverse_iterator rend() {
            return --begin();
        }

        const_reverse_iterator rbegin() const {
            return --end();
        }

        const_reverse_iterator rend() const {
            return --begin();
        }

        iterator insert(iterator pos, const T& val) {
            Node* newNode = new Node(val);
            newNode->_prev = pos._node->_prev;
            pos._node->_prev = newNode;
            newNode->_prev->_next = newNode;
            newNode->_next = pos._node;
            _size++;
            return newNode;
        }

        iterator erase(iterator pos) {
            pos._node->_prev->_next = pos._node->_next;
            pos._node->_next->_prev = pos._node->_prev;
            Node* next = pos._node->_next;
            delete pos._node;
            _size--;
            return next;
        }

        void clear() {
            iterator it = begin();
            while (it != end()) {
                it = erase(it);
            }
            _size = 0;
        }

        ~list() {
            clear();
            delete _head;
            _head = nullptr;
        }

        size_t size() {
            return _size;
        }

        // 拷贝构造
        // list(const list& lt){
        list(list<T>& lt) {
            empty_init();
            for (auto x : lt) {
                push_back(x);
            }
            _size = lt.size();
        }

        // 赋值重载
        // list& operator=(const list& lt){
        list<T>& operator=(list<T>& lt) {
            if (this != &lt) {
                clear();
                for (auto x : lt) {
                    push_back(x);
                }
                _size = lt.size();
            }
            return *this;
        }


    private:
        Node* _head;
        size_t _size;
    };
}