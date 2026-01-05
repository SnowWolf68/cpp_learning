#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

#include "AVLTree.h"
// 测试函数
void testAVLTree() {
    cout << "========== 开始测试 AVL 树 ==========" << endl;
    
    // 测试1：基础插入测试
    {
        cout << "\n测试1：基础插入和平衡性测试" << endl;
        AVLTree<int, int> tree;
        
        // 插入一些数据
        vector<int> data = {10, 20, 30, 40, 50, 25};
        for(int num : data){
            tree.insert({num, num * 10});
        }
        
        // 验证所有元素都存在
        for(int num : data){
            if(!tree.contains(num)){
                cout << "错误：找不到元素 " << num << endl;
                return;
            }
        }
        
        // 验证树是平衡的
        if(tree.isAVLTree()){
            cout << "✓ 树是平衡的" << endl;
        }else{
            cout << "✗ 树不平衡" << endl;
            return;
        }
        
        // 验证排序性质
        vector<pair<int, int>> inorder = tree.inorder();
        for(size_t i = 1; i < inorder.size(); i++){
            if(inorder[i].first <= inorder[i-1].first){
                cout << "错误：中序遍历结果不是升序" << endl;
                return;
            }
        }
        cout << "✓ 中序遍历是升序的" << endl;
    }
    
    // 测试2：左旋测试
    {
        cout << "\n测试2：左旋测试（右子树过高）" << endl;
        AVLTree<int, int> tree;
        
        // 插入导致左旋的序列
        tree.insert({10, 100});
        tree.insert({20, 200});
        tree.insert({30, 300});  // 应该触发左旋
        
        if(tree.isAVLTree()){
            cout << "✓ 左旋后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
        }else{
            cout << "✗ 左旋后树不平衡" << endl;
            return;
        }
    }
    
    // 测试3：右旋测试
    {
        cout << "\n测试3：右旋测试（左子树过高）" << endl;
        AVLTree<int, int> tree;
        
        // 插入导致右旋的序列
        tree.insert({30, 300});
        tree.insert({20, 200});
        tree.insert({10, 100});  // 应该触发右旋
        
        if(tree.isAVLTree()){
            cout << "✓ 右旋后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
        }else{
            cout << "✗ 右旋后树不平衡" << endl;
            return;
        }
    }
    
    // 测试4：左右双旋测试
    {
        cout << "\n测试4：左右双旋测试" << endl;
        AVLTree<int, int> tree;
        
        // 插入导致左右双旋的序列
        tree.insert({30, 300});
        tree.insert({10, 100});
        tree.insert({20, 200});  // 应该触发左右双旋
        
        if(tree.isAVLTree()){
            cout << "✓ 左右双旋后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
        }else{
            cout << "✗ 左右双旋后树不平衡" << endl;
            return;
        }
    }
    
    // 测试5：右左双旋测试
    {
        cout << "\n测试5：右左双旋测试" << endl;
        AVLTree<int, int> tree;
        
        // 插入导致右左双旋的序列
        tree.insert({10, 100});
        tree.insert({30, 300});
        tree.insert({20, 200});  // 应该触发右左双旋
        
        if(tree.isAVLTree()){
            cout << "✓ 右左双旋后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
        }else{
            cout << "✗ 右左双旋后树不平衡" << endl;
            return;
        }
    }
    
    // 测试6：随机插入测试
    {
        cout << "\n测试6：随机插入测试" << endl;
        AVLTree<int, int> tree;
        
        srand(time(nullptr));
        vector<int> randomData;
        
        // 生成100个随机数
        for(int i = 0; i < 100; i++){
            int num = rand() % 1000;
            if(tree.insert({num, num * 10})){
                randomData.push_back(num);
            }
        }
        
        // 验证树是平衡的
        if(tree.isAVLTree()){
            cout << "✓ 随机插入后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
            cout << "  插入的唯一元素数量: " << randomData.size() << endl;
            
            // 验证排序性质
            vector<pair<int, int>> inorder = tree.inorder();
            bool sorted = true;
            for(size_t i = 1; i < inorder.size(); i++){
                if(inorder[i].first <= inorder[i-1].first){
                    sorted = false;
                    break;
                }
            }
            
            if(sorted){
                cout << "✓ 随机插入后中序遍历是升序的" << endl;
            }else{
                cout << "✗ 随机插入后中序遍历不是升序的" << endl;
            }
        }else{
            cout << "✗ 随机插入后树不平衡" << endl;
            return;
        }
    }
    
    // 测试7：有序插入测试（最坏情况）
    {
        cout << "\n测试7：有序插入测试（最坏情况）" << endl;
        AVLTree<int, int> tree;
        
        // 有序插入100个元素
        for(int i = 0; i < 100; i++){
            tree.insert({i, i * 10});
        }
        
        if(tree.isAVLTree()){
            cout << "✓ 有序插入后树是平衡的" << endl;
            cout << "  树的高度: " << tree.height() << endl;
            
            // AVL树的理论最大高度约为 1.44 * log2(n+2) - 1.328
            // 对于n=100，理论最大高度约为 1.44 * log2(102) - 1.328 ≈ 1.44 * 6.67 - 1.328 ≈ 9.6 - 1.328 ≈ 8.27
            // 实际高度应该不超过9
            if(tree.height() <= 9){
                cout << "✓ 树的高度在合理范围内" << endl;
            }else{
                cout << "警告：树的高度可能过高" << endl;
            }
        }else{
            cout << "✗ 有序插入后树不平衡" << endl;
            return;
        }
    }
    
    // 测试8：重复插入测试
    {
        cout << "\n测试8：重复插入测试" << endl;
        AVLTree<int, int> tree;
        
        // 插入重复元素
        bool firstInsert = tree.insert({10, 100});
        bool secondInsert = tree.insert({10, 200});  // 应该失败
        
        if(firstInsert && !secondInsert){
            cout << "✓ 重复插入被正确处理" << endl;
        }else{
            cout << "✗ 重复插入处理错误" << endl;
        }
    }
    
    cout << "\n========== 所有测试通过！ ==========" << endl;
}

// 可视化输出树结构（辅助调试）
template <class K, class V>
void printTree(AVLTreeNode<K, V>* root, int depth = 0) {
    if(root == nullptr) return;
    
    // 先打印右子树
    printTree(root->_right, depth + 1);
    
    // 打印当前节点
    for(int i = 0; i < depth; i++) cout << "   ";
    cout << root->_kv.first << "(" << root->_bf << ")" << endl;
    
    // 打印左子树
    printTree(root->_left, depth + 1);
}


int main() {
    // 运行自动测试
    testAVLTree();
    
    
    return 0;
}