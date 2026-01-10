#include "RBTree.h"

namespace snow{
    template<class K, class V>
    class map{
    public:
        struct MapKeyOfT{
            const K& operator()(const pair<K, V>& kv){
                return kv.first;
            }
        };
        bool insert(const pair<K, V>& kv){
            _t.Insert(kv);
        }
    private:
        RBTree<K, pair<K, V>, MapKeyOfT> _t;
    };
}