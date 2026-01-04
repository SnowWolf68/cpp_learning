template <class K, class V>
struct AVLTreeNode{
    AVLTreeNode<K, V>* _left;
    AVLTreeNode<K, V>* _right;
    AVLTreeNode<K, V>* _parent;

    pair<K, V> _kv;

    int _bf;    // balance factor = right - left

    AVLTreeNode(const pair<K, V>& kv)
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_kv(kv)
        ,_bf(0)
    {}

};

template <class K, class V>
struct AVLTree{
public:
    bool insert(const pair<K, V>& kv){
        if(_root == nullptr){
            _root = new AVLTreeNode<K, V>(kv);
            return true;
        }
        AVLTreeNode<K, V>* cur = _root;
        AVLTreeNode<K, V>* parent = nullptr;
        while(cur != nullptr){
            parent = cur;
            if(kv.first > cur->_kv.first){
                cur = cur->_right;
            }else if(kv.first < cur->_kv.first){
                cur = cur->_left;
            }else{
                return false;
            }
        }
        cur = new AVLTreeNode<K, V>(kv);
        if(cur->_kv.first > parent->_kv.first){
            parent->_right = cur;
            parent->_bf++;
        }else{
            parent->_left = cur;
            parent->_bf--;
        }
        cur->_parent = parent;

        /*
        1) 刚插入时: (cur为刚插入节点, parent为cur的父节点)
            1. parent的bf = 0, 说明parent所在子树的bf插入前是1/-1, 插入后变成了0, 即插入之后填补上了树高较小的那一棵子树, 
                    parent所在子树的树高插入后不会变化, 不需要继续往上更新
            2. parent的bf = 1/-1, 说明parent所在子树的bf插入前是0, 插入后变成了1/-1, 即插入后parent所在子树的树高+1, 需要继续往上更新bf (继续找最小不平衡子树)
        2) 继续向上调整的过程中: (parent为当前有可能调整的节点, cur为parent的两个子树中, 含有新插入节点的那个子树)
            1.(同上) parent的bf = 0, 说明parent所在子树的bf插入前是1/-1, 插入后变成了0, 即插入之后填补上了树高较小的那一棵子树, 
                    parent所在子树的树高插入后不会变化, 不需要继续往上更新
            2.(同上) parent的bf = 1/-1, 说明parent所在子树的bf插入前是0, 插入后变成了1/-1, 即插入后parent所在子树的树高+1, 需要继续往上更新bf (继续找最小不平衡子树)
            3.(额外) parent的bf = 2/-2, 说明parent就是最小不平衡子树的根节点, 先旋转, 然后是否要继续向上调整需要视情况而定
        */
        
        while(parent != nullptr){
            if(parent->_bf == 0){
                break;
            }else if(parent->_bf == 1 || parent->_bf == -1){
                // 继续向上, 先向上走一步, 再更新parent的bf
                cur = parent;
                parent = cur->_parent;
                if(parent != nullptr){
                    if(cur = parent->_left) parent->_bf--;
                    else parent->_bf++;
                }
            }else{  // 2/-2
                // parent为根的子树就是当前找到的最小不平衡子树

            }
        }
    }

    /*
        parent  - cur的父节点
        cur     - 最小不平衡子树的根节点
        curR    - cur的右孩子
        curRL   - cur的右孩子的左孩子

        -- 调整结果: 根节点由 cur -> curR 之后, 树高-1
        -- 调整后的根节点: curR
    */
    void rotateL(AVLTreeNode<K, V>* cur){
        AVLTreeNode<K, V>* curR = cur->_right;
        AVLTreeNode<K, V>* curRL = curR->_left;    // 可能为nullptr
        AVLTreeNode<K, V>* parent = cur->_parent;

        curR->_parent = cur->_parent;
        cur->_parent = curR;
        cur->_right = curRL;
        curR->_left = cur;
        if(curRL != nullptr) curRL->_parent = cur;
        cur->_bf = curR->_bf = 0;


        // parent左/右子树在插入前及插入+调整后的高度不变, 因此parent的bf不变, 也不需要继续向上调整
        if(parent == nullptr){
            if(cur = parent->_left){
                parent->_left = curR;
            }else{
                parent->_right = curR;
            }
        }
    }

    /*
        -- 调整后的根节点: curL
    */
    void rotateR(AVLTreeNode<K, V>* cur){
        AVLTreeNode<K, V>* parent = cur->_parent;
        AVLTreeNode<K, V>* curL = cur->_left;
        AVLTreeNode<K, V>* curLR = curL->_right;    // 可能为nullptr

        cur->_parent = curL;
        cur->_left = curLR;
        curL->_parent = parent;
        curL->_right = cur;
        if(curLR != nullptr) curLR->_parent = cur;
        cur->_bf = curL->_bf = 0;

        if(parent != nullptr){
            if(cur = parent->_left) {
                parent->_left = curL;
            }
            else {
                parent->_right = curL;
            }
        }
    }

    void rorateLR(AVLTreeNode<K, V>* cur){
        
    }





private:
    AVLTreeNode<K, V>* _root = nullptr;
};
