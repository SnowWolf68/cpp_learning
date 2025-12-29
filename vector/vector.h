namespace snow{
    template<class T>
    class vector{
    public: 
        typedef T* iterator;
        typedef const T* const_iterator;

        vector()
        {}

        vector(size_t n, const T& val = T()){
            reserve(n);
            for(int i = 0;i < n;i++){
                push_back(val);
            }
        }

        vector(int n, const T& val = T()){
            reserve(n);
            for(int i = 0;i < n;i++){
                push_back(val);
            }
        }

        template <class InputIterator>
        vector(InputIterator first, InputIterator last){
            while(first != last){
                push_back(*first);
                first++;
            }
        }

        ~vector(){
            delete[] _start;
            _start = _finish = _endofstorage = nullptr;
        }

        vector(const vector& v){    // 类内不需要加<T>, 类外需要加<T>
            int sz = v.size(), cap = v.capacity();
            _start = new T[cap];
            // memcpy(_start, v._start, sz * sizeof(T));
            for(int i = 0;i < sz;i++){
                _start[i] = v._start[i];
            }
            _finish = _start + sz;
            _endofstorage = _start + cap;
        }

        vector& operator=(const vector& v){
            int sz = v.size(), cap = v.capacity();
            delete[] _start;
            _start = new T[cap];
            // memcpy(_start, v._start, sz * sizeof(T));
            for(int i = 0;i < sz;i++){
                _start[i] = v._start[i];
            }
            _finish = _start + sz;
            _endofstorage = _start + cap;
        }

        void reserve(int n){
            if(n > capacity()){
                int sz = size();
                T* tmp = new T[n];
                if(_start != nullptr){
                    // memcpy(tmp, _start, sizeof(T) * sz);
                    for(int i = 0;i < sz;i++){
                        tmp[i] = _start[i];
                    }
                }
                delete[] _start;
                _start = tmp;
                _finish = _start + sz;
                _endofstorage = _start + n;
            }
        }

        void push_back(const T& x){
            // if(_finish == _endofstorage){
            //     reserve(_start == _finish ? 4 : 2 * capacity());
            // }
            // *_finish = x;
            // ++_finish;

            insert(end(), x);   // 好像也能用????
        }

        void insert(size_t pos, const T& x){
            assert(pos <= size());
            if(_finish == _endofstorage){
                reserve(_start == _finish ? 4 : 2 * capacity());
            }
            iterator it = _finish;
            while(it != _start + pos){
                *(it + 1) = *it;
                --it;
            }
            *it = x;
            _finish++;
        }

        // 1. insert内部: 小心扩容后pos失效 (迭代器失效)
        // 2. insert外部: insert后的迭代器有可能失效, 不能再使用pos
        //      也不能尝试使用iterator& pos传引用, 因为有可能通过insert(v.begin(), 1)调用, 
        //          这里的v.begin()是临时对象, 具有常性
        //      但是我这里的实现由于内部没有修改pos引用的对象, 所以其实可以改成const iterator& pos
        // 不过标准库里的实现没有使用const引用的形式, 而是规定insert可能导致迭代器失效, 
        //      insert后不能再使用同一个迭代器
        void insert(const iterator& pos, const T& x){
            assert(pos >= _start);
            assert(pos <= _finish);
            insert(pos - _start, x);
        }

        void erase(iterator pos){
            assert(pos >= _start);
            assert(pos <= _finish);
            while(pos != _finish){
                *pos = *(pos + 1);
                pos++;
            }
            _finish--;
        }

        // void resize(int n, T val = T()){
        void resize(int n, const T& val = T()){
            int sz = size(), cap = capacity();
            if(n < sz){
                _finish -= (sz - n);
            }else{
                if(n > cap){
                    reserve(n);
                }
                iterator end = _finish + (n - sz);
                while(_finish != end){
                    *_finish = val;
                    _finish++;
                }
            }
        }

        T& operator[](size_t pos){
            assert(pos < size());
            return _start[pos];
        }

        const T& operator[](size_t pos) const {
            assert(pos < size());
            return _start[pos];
        }

        int capacity() const {
            return _endofstorage - _start;
        }

        int size() const {
            return _finish - _start;
        }

        iterator begin(){
            return _start;
        }

        iterator end(){
            return _finish;
        }

        const_iterator begin() const {
            return _start;
        }

        const_iterator end() const {
            return _finish;
        }


    private:
        iterator _start = nullptr;
        iterator _finish = nullptr;
        iterator _endofstorage = nullptr;

    };
}