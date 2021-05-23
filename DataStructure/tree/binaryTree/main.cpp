#include <iostream>
#include <stack>
#include <queue>

using namespace std;

/*
 * 二叉樹節點
 * */
template <typename T>
struct BinaryTreeNode {
    T data; // 節點中存儲的數據
    BinaryTreeNode<T> *leftChild, *rightChild; // 左右子樹
    // 無參構造函數
    BinaryTreeNode() :leftChild(NULL), rightChild(NULL) {}
    // 帶默認值構造函數
    BinaryTreeNode(T x, BinaryTreeNode<T> *l = NULL, BinaryTreeNode<T> *r = NULL) :data(x), leftChild(l), rightChild(r) {}
};

/*
 * 二叉樹
 * */
template <typename T>
class BinaryTree {
public:
    /*
     * 二叉樹構造與析構
     * */

    // 一般構造函數
    BinaryTree() :root(NULL) {}
    // 指定結束標誌的構造函數
    BinaryTree(T stop) :root(NULL), stopFlag(stop) {}
    // 析構函數
    ~BinaryTree() { Destroy(); }

    /*
     * 創建二叉樹
     * */

    // 使用廣義表創建二叉樹，以 '#' 表示結束標誌
    void createBinTree() { createBinTree(root); }

    /*
     * 遍歷二叉樹
     * */

    // 先序遍歷(遞規)
    void Preorder() { Preorder(root); }

    // 中序遍歷(遞規)
    void Inorder() { Inorder(root); }

    // 後序遍歷(遞規)
    void Postorder() { Postorder(root); }

    // 先序遍歷(非遞規)
    void Preorder_nonRecursive() { Preorder_nonRecursive(root); }

    // 中序遍歷(非遞規)
    void Inorder_nonRecursive() { Inorder_nonRecursive(root); }

    // 後序遍歷(非遞規)
    void Postorder_nonRecursive() { Postorder_nonRecursive(root); }

    // 層次遍歷
    void Levelorder() { Levelorder(root); }

    /*
     * 獲取節點
     * */

    // 獲取根節點
    BinaryTreeNode<T> *getRoot() const{
        return root;
    }

    // 獲取current節點的父節點
    BinaryTreeNode<T> *getFather(BinaryTreeNode<T> *current) const{
        return (root == NULL || current == root) ? NULL : getFather(root, current);
    }

    // 獲取current節點的左節點
    BinaryTreeNode<T> *get_leftChild(BinaryTreeNode<T> *current) const{
        return (current != NULL) ? current->leftChild : NULL;
    }

    // 獲取current節點的右節點
    BinaryTreeNode<T> *get_rightChild(BinaryTreeNode<T> *current) const{
        return (current != NULL) ? current->rightChild : NULL;
    }

    /*
     * 成員函數
     * */

    // 銷毀二叉樹
    void Destroy() { Destroy(root); };

    // 判斷兩棵二叉樹是否相同
    bool operator==(BinaryTreeNode<T> &s) {
        return equal(this->getRoot(), s.getRoot());
    }

    // 獲取二叉樹節點個數
    int size() { return size(root); }

    // 獲取二叉樹高度
    int height() { return height(root); }

    // 判斷二叉樹是否為空
    int isEmpty() { return (root == NULL); }

    // 以廣義表形式打印二叉樹
    void printTree() { printTree(root); }


protected:
    void createBinTree(BinaryTreeNode<T>* &BT) {
        stack<BinaryTreeNode<T>*> s;
        BT = NULL;
        BinaryTreeNode<T> *p, *t; // p 用來記住當前創建的節點，t 用來記住棧頂元素
        int k; // 處理左/右子樹的標記
        T ch;
        cin >> ch;

        while (ch != stopFlag) {
            switch (ch) {
                case '(':
                    s.push(p);
                    k = 1;
                    break;
                case ')':
                    s.pop();
                    break;
                case ',':
                    k = 2;
                    break;
                default:
                    p = new BinaryTreeNode<T>(ch);
                    if(BT == NULL) { // 若頭節點為空
                        BT = p;
                    }
                    else if(k == 1) {
                        t = s.top();
                        t->leftChild = p;
                    }
                    else {
                        t = s.top();
                        t->rightChild = p;
                    }
            }
            cin >> ch;
        }
    }

    // 根左右
    void Preorder(BinaryTreeNode<T>* &subtree) {
        if(subtree != NULL) {
            cout << subtree->data << " ";
            Preorder(subtree->leftChild);
            Preorder(subtree->rightChild);
        }
    }

    // 左根右
    void Inorder(BinaryTreeNode<T>* &subtree) {
        if(subtree != NULL) {
            Inorder(subtree->leftChild);
            cout << subtree->data << " ";
            Inorder(subtree->rightChild);
        }
    }

    // 左右根
    void Postorder(BinaryTreeNode<T>* &subtree) {
        if(subtree != NULL) {
            Postorder(subtree->leftChild);
            Postorder(subtree->rightChild);
            cout << subtree->data << " ";
        }
    }

    // 根左右(非遞規)
    void Preorder_nonRecursive(BinaryTreeNode<T>* p) {
        stack<BinaryTreeNode<T>*> s;
        s.push(NULL); // 先push一個NULL，當遍歷到最後一個節點再沒左右子樹，棧裡只剩一個NULL，p被置為NULL，再判斷就會結束遍歷
        while (p != NULL) {
            cout << p->data << " ";
            // 預留右子樹節點以便回退
            if(p->rightChild != NULL) {
                s.push(p->rightChild);
            }

            // 遍歷左子樹
            if(p->leftChild != NULL) {
                p = p->leftChild;
            }else {
                p = s.top();
                s.pop();
            }
        }
    }

    // 左根右(非遞規)
    void Inorder_nonRecursive(BinaryTreeNode<T>* p) {
        stack<BinaryTreeNode<T>*> s;
        do {
            while (p != NULL) {
                s.push(p);
                p = p->leftChild;
            }
            if (!s.empty()) {
                p = s.top();
                s.pop();
                cout << p->data << " ";
                p = p->rightChild;
            }
        }while (p != NULL || s.empty());
    }

    // 左右根(非遞規)
    void Postorder_nonRecursive(BinaryTreeNode<T>* p) {
        if(root == NULL) {
            return;
        }
        stack<BinaryTreeNode<T>*> s;
        s.push(p);
        BinaryTreeNode<T> *lastPop = NULL;

        while(!s.empty()) {
            while (s.top()->leftChild != NULL) {
                s.push(s.top()->leftChild);
            }
            while (!s.empty()) {
                // 右葉子節點 || 無右節點
                if(lastPop == s.top()->rightChild || s.top()->rightChild == NULL) {
                    cout <<  s.top()->data << " ";
                    lastPop = s.top();
                    s.pop();
                }else if(s.top()->rightChild != NULL) {
                    s.push(s.top()->rightChild);
                    break;
                }
            }
        }
    }

    void Levelorder(BinaryTreeNode<T>* p) {
        queue<BinaryTreeNode<T>*> q;
        q.push(p); // 根節點先進隊
        BinaryTreeNode<T> *t;

        while (!q.empty()) {
            t = q.front();
            q.pop();
            cout << t->data << " ";

            if(t->leftChild != NULL) {
                q.push(t->leftChild);
            }
            if(t->rightChild != NULL) {
                q.push(t->rightChild);
            }
        }
    }

    int size(BinaryTreeNode<T> *subTree) {
        if(subTree == NULL) {
            return 0;
        }
        return 1 + Size(subTree->leftChild) + Size(subTree->rightChild);
    }

    int height(BinaryTreeNode<T> *subTree) {
        if(subTree == NULL) {
            return 0;
        }
        int leftHeight = Height(subTree->leftChild);
        int rightHeight = Height(subTree->rightChild);
        return (leftHeight < rightHeight) ? rightHeight + 1 : leftHeight + 1;
    }

    void printTree(BinaryTreeNode<T> *BT) {
        if(BT != NULL) {
            cout << BT->data;
            if(BT->leftChild != NULL || BT->rightChild != NULL) {
                cout << "(";
                if(BT->leftChild != NULL) {
                    printTree(BT->leftChild);
                }
                cout << ",";
                if(BT->rightChild != NULL) {
                    printTree(BT->rightChild);
                }
                cout << ")";
            }
        }
    }

    BinaryTreeNode<T>* getFather(BinaryTreeNode<T>* subTree, BinaryTreeNode<T>* current) {
        if(subTree == NULL) {
            return NULL;
        }
        if(subTree->leftChild == current || subTree->rightChild == current) {
            return subTree;
        }
        if(getFather(subTree->leftChild, current) != NULL) { // 遞規在左子樹中尋找
            return getFather(subTree->leftChild, current);
        }
        else { // 遞規在右子樹中尋找
            return getFather(subTree->rightChild, current);
        }
    }

    void Destroy(BinaryTreeNode<T>* &subTree) {
        if(subTree != NULL) {
            Destroy(subTree->leftChild);
            Destroy(subTree->rightChild);
            delete subTree;
            subTree = NULL;
        }
    }

    bool equal(BinaryTreeNode<T>* a, BinaryTreeNode<T>* b) {
        if(a == NULL && b == NULL) {
            return true;
        }
        if((a != NULL && b != NULL) && (a->data == b->data) && (equal(a->leftChild, b->leftChild)) && (equal(a->rightChild, b->rightChild))) {
            return true;
        }
        return false;
    }


private:
    BinaryTreeNode<T> *root; // 根節點
    T stopFlag; // 數據停止輸入的標誌
};
