namespace snow {
    template <class iterator, class Ref, class Ptr>
    class __reverse_iterator {
    public:
        __reverse_iterator(iterator it)
            :_it(it)
        {}

        __reverse_iterator(const __reverse_iterator& rit) 
            :_it(rit._it)
        {}

        __reverse_iterator& operator++() {
            _it--;
            return *this;
        }
        __reverse_iterator operator++(int) {
            __reverse_iterator tmp(*this);
            _it--;
            return tmp;
        }
        __reverse_iterator& operator--() {
            _it++;
            return *this;
        }
        __reverse_iterator operator--(int) {
            __reverse_iterator tmp(*this);
            _it++;
            return tmp;
        }
        bool operator!=(const __reverse_iterator& rit) {
            return this->_it != rit._it;
        }
        bool operator==(const __reverse_iterator& rit) {
            return this->_it == rit._it;
        }
        Ref operator*(){
            return *_it;
        }

        Ptr operator->(){
            return _it.operator->();
        }
    private:
        iterator _it;
    };
}