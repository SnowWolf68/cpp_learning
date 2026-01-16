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
                    if(cur == parent->_left) parent->_bf--;
                    else parent->_bf++;
                }
            }else{  // 2/-2
                // parent为根的子树就是当前找到的最小不平衡子树
                if(parent->_bf == 2 && cur->_bf == 1) {
                    // 左单旋
                    rotateL(parent);
                } else if(parent->_bf == 2 && cur->_bf == -1) {
                    // 右左双旋
                    rotateRL(parent);
                } else if(parent->_bf == -2 && cur->_bf == -1) {
                    // 右单旋
                    rotateR(parent);
                } else if(parent->_bf == -2 && cur->_bf == 1) {
                    // 左右双旋
                    rotateLR(parent);
                }
                
                // 旋转后，以parent为根的子树高度恢复到插入前的高度
                // 不需要继续向上调整
                break;
            }
        }
        return true;
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
        AVLTreeNode<K, V>* parent = cur->_parent;  // 可能为nullptr

        curR->_parent = cur->_parent;
        cur->_parent = curR;
        cur->_right = curRL;
        curR->_left = cur;
        if(curRL != nullptr) curRL->_parent = cur;
        cur->_bf = curR->_bf = 0;


        // parent左/右子树在插入前及插入+调整后的高度不变, 因此parent的bf不变, 也不需要继续向上调整
        if(parent != nullptr){
            if(cur == parent->_left){
                parent->_left = curR;
            }else{
                parent->_right = curR;
            }
        }else{
            _root = curR;
        }
    }

    /*
        -- 调整后的根节点: curL
    */
    void rotateR(AVLTreeNode<K, V>* cur){
        AVLTreeNode<K, V>* parent = cur->_parent;   // 可能为nullptr
        AVLTreeNode<K, V>* curL = cur->_left;
        AVLTreeNode<K, V>* curLR = curL->_right;    // 可能为nullptr

        cur->_parent = curL;
        cur->_left = curLR;
        curL->_parent = parent;
        curL->_right = cur;
        if(curLR != nullptr) curLR->_parent = cur;
        cur->_bf = curL->_bf = 0;

        if(parent != nullptr){
            if(cur == parent->_left) {
                parent->_left = curL;
            }
            else {
                parent->_right = curL;
            }
        }else{
            _root = curL;
        }
    }

    void rotateLR(AVLTreeNode<K, V>* cur){
        AVLTreeNode<K, V>* curL = cur->_left;
        AVLTreeNode<K, V>* curLR = curL->_right;

        int bf = curLR->_bf;

        rotateL(curL);
        rotateR(cur);

        // 根据旋转前curLR的平衡因子重新调整最终平衡因子    (单旋中调整bf的方式对于双旋不适用)
        if(bf == 1) {
            curL->_bf = -1;
            cur->_bf = 0;
            curLR->_bf = 0;
        } else if(bf == -1) {
            curL->_bf = 0;
            cur->_bf = 1;
            curLR->_bf = 0;
        } else { // bf == 0
            curL->_bf = 0;
            cur->_bf = 0;
            curLR->_bf = 0;
        }
    }

    void rotateRL(AVLTreeNode<K, V>* cur) {
        AVLTreeNode<K, V>* curR = cur->_right;
        AVLTreeNode<K, V>* curRL = curR->_left;
        int bf = curRL->_bf;  // 保存旋转前的平衡因子
        
        // 先对cur的右子树进行右旋
        rotateR(curR);
        // 再对cur进行左旋
        rotateL(cur);
        
        // 根据旋转前curRL的平衡因子调整最终平衡因子
        if(bf == 1) {
            cur->_bf = -1;
            curR->_bf = 0;
            curRL->_bf = 0;
        } else if(bf == -1) {
            cur->_bf = 0;
            curR->_bf = 1;
            curRL->_bf = 0;
        } else { // bf == 0
            cur->_bf = 0;
            curR->_bf = 0;
            curRL->_bf = 0;
        }
    }

    // 计算树的高度
    int height(AVLTreeNode<K, V>* node) {
        if(node == nullptr) return 0;
        return max(height(node->_left), height(node->_right)) + 1;
    }

    // 验证是否是AVL树
    bool isAVLTree(AVLTreeNode<K, V>* node) {
        if(node == nullptr) return true;
        
        int leftHeight = height(node->_left);
        int rightHeight = height(node->_right);
        
        // 检查平衡因子是否正确
        if(node->_bf != (rightHeight - leftHeight)) {
            return false;
        }
        
        // 检查平衡因子是否在[-1, 1]范围内
        if(abs(node->_bf) > 1) {
            return false;
        }
        
        // 递归检查左右子树
        return isAVLTree(node->_left) && isAVLTree(node->_right);
    }


     // 新增：测试用的辅助函数
    bool contains(const K& key) const {
        AVLTreeNode<K, V>* cur = _root;
        while(cur != nullptr){
            if(key > cur->_kv.first){
                cur = cur->_right;
            }else if(key < cur->_kv.first){
                cur = cur->_left;
            }else{
                return true;
            }
        }
        return false;
    }

    // 计算树的高度
    int height() const {
        return _height(_root);
    }

    // 中序遍历（验证排序性质）
    vector<pair<K, V>> inorder() const {
        vector<pair<K, V>> result;
        _inorder(_root, result);
        return result;
    }

    // 验证是否是AVL树
    bool isAVLTree() const {
        return _isAVLTree(_root);
    }

    // 清空树
    void clear() {
        _clear(_root);
        _root = nullptr;
    }

    ~AVLTree() {
        clear();
    }




private:
    AVLTreeNode<K, V>* _root = nullptr;

    // 私有辅助函数
    int _height(AVLTreeNode<K, V>* node) const {
        if(node == nullptr) return 0;
        return max(_height(node->_left), _height(node->_right)) + 1;
    }

    void _inorder(AVLTreeNode<K, V>* node, vector<pair<K, V>>& result) const {
        if(node == nullptr) return;
        _inorder(node->_left, result);
        result.push_back(node->_kv);
        _inorder(node->_right, result);
    }

    bool _isAVLTree(AVLTreeNode<K, V>* node) const {
        if(node == nullptr) return true;
        
        int leftHeight = _height(node->_left);
        int rightHeight = _height(node->_right);
        
        // 检查平衡因子是否正确计算
        int calculatedBF = rightHeight - leftHeight;
        if(node->_bf != calculatedBF){
            cout << "错误：节点 " << node->_kv.first << " 的平衡因子不正确" << endl;
            cout << "  存储的平衡因子: " << node->_bf << endl;
            cout << "  计算的平衡因子: " << calculatedBF << endl;
            return false;
        }
        
        // 检查平衡因子是否在允许范围内
        if(abs(node->_bf) > 1){
            cout << "错误：节点 " << node->_kv.first << " 的平衡因子超出范围: " << node->_bf << endl;
            return false;
        }
        
        // 递归检查子树
        return _isAVLTree(node->_left) && _isAVLTree(node->_right);
    }

    void _clear(AVLTreeNode<K, V>* node){
        if(node == nullptr) return;
        _clear(node->_left);
        _clear(node->_right);
        delete node;
    }
};
