#include<deque>
namespace snow{

    template <class T, class container = std::deque<T>>
    class stack{
    public: 
        void push(const T& x){
            con.push_back(x);
        }

        void pop(){
            con.pop_back();
        }

        const T& top(){
            return con.top();
        }

        bool empty(){
            return con.empty();
        }

        size_t size(){
            return con.size();
        }
    private:
        container con;
    };

}