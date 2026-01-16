#include "RBTree.h"

namespace snow{
    template<class K>
    class set{
    public: 
        struct SetKeyOfT{
            const K& operator()(const K& k){
                return k;
            }
        };

        bool insert(const K& key){
            return _t.Insert(key);
        }
    private:
        RBTree<K, K, SetKeyOfT> _t;
    };
}