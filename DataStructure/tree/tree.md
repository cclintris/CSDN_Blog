@[TOC](C++ 完整理解與實現二叉樹)

# 樹是甚麼 ?
在計算機的世界中，樹是一個非常經典且變化多，應用性高的一種 ADT(Abstract Data Type)，簡單的說，是由 n (n > 0) 個有限節點組成的一個有層次的集合。其實叫它樹是因為畫起來其實像一棵倒掛的樹，也就是說看起來是根朝上，葉子朝下的。根據 wiki，一棵樹擁有以下特徵:
1. 一個節點有一個或多個子節點(child)
2. 沒有父節點(father)的節點就是根結點(root)
3. 每一個非根結點都只有一個父節點
4. 除了根結點，每個子節點都可以分為多個不相交的子樹

有了對樹的一點基本的初步認識，要想完整的實現出一棵樹還得了解一些術語，以下就來看看更多樹的概念吧!

# Tree 術語

- Root 根
頂端節點，一棵樹只有一個

- Child 子節點
一個由父節點連接的點都是 child

- Father 父節點
連接子節點的節點

- Siblings 兄弟節點
一群擁有相同父節點的同級節點都叫 siblings

- Descendant 後裔節點
由一個父節點不斷往下延伸都能到達的節點都是該父節點的 descendant

- Ancestor 祖先
由一個子節點不斷往上延伸都能到達的節點都是該子節點的 ancestor

- Leaf 葉節點
沒有子節點的節點，也常稱為外節點(external node)

- Branch 分支
至少有一個子節點的節點(internal node)

- Degree 度
度的概念有分為出度和入度。因為樹是由節點連接的，出度就是說從該節點向外有多少個連接，反之則為入度。應用到樹的概念上，像是 father -> son 的父子節點結構，那麼就說 father 的出度 = 1。

- Edge 連接
節點之間的聯繫

- Path 路徑
一節點和另一節點相連的節點序列

- Level 層次
level = 根到目標節點的連接 + 1，根結點默認 level = 1

- Depth 深度
深度就是某節點的層次

- Height of node 節點高度
節點高度就是某節點到葉節點最長路徑

- Height of tree 樹高度
樹高就是從根到某葉節點的最長路徑

有了以上所有的概念，可以來認識下到底有哪幾種樹，以及各自的實現吧，比較長，可以挑著需要的自己看，因為本人希望就整理一篇就好，免得過於繁瑣細碎。

# 二叉樹 Binary Tree
二叉樹其實是以遞規形式的一些節點集合給出的，這個集合如果為空，那就是一棵空樹，否則，就是一個由根節點加上兩棵左子樹和右子樹組成的，而左子樹和右子樹又各自都是二叉樹，如此遞規下去。二又樹的特點是每個節點最多有兩個子女，分別稱為該節點的左子女和右子女。在二又樹中不存在 degree > 2 的節點，並且二又樹的子樹有左、右之分，其子樹的次序也不能顛倒。

## 二叉樹存儲方式

### 數組
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-1.png)

### 鏈表
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-2.png)

## 二叉樹節點定義
```cpp
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
```

## 二叉樹整體框架
```cpp
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

private:
    BinaryTreeNode<T> *root; // 根節點
    T stopFlag; // 數據停止輸入的標誌
};

```

## 創建二叉樹
首先，當然要來看看如何創建二叉樹，以及有哪些方式，這邊主要關注廣義表的創建方式。

### 廣義表創建
由廣義表 <b>A（B（D，E（G，）），C（，F））#</b> 建立起來的二叉樹如下圖，應該不是很困難:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-3.png)

算法思路:
1. 如果遇到 "(" 則表示子樹的開始，將標記 k 置為 1，若遇到 ")" 則表示子樹的結束。
2. 如果遇到 "，"，則表示以左子女為根的子樹處理完畢，應該要接著處理以右子女為根的子樹，這時將 k 置為 2。
3. 如果遇到字母，則表示是樹的節點值，為它新建一個節點，並把該節點作為左子女(k = 1)或右子女(k = 2)連接到父節點上。
4. 遇到 "#" 就停止

```cpp
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
```

## 遍歷二叉樹
遍歷二叉樹主要可以分為遞規以及非遞規兩種，其中又分為先序，中序，後序。此外還有層次遍歷。

### 遞規遍歷
- 先序
==根 → 左 → 右==
```cpp
// 根左右
void Preorder(BinaryTreeNode<T>* &subtree) {
    if(subtree != NULL) {
        cout << subtree->data << " ";
        Preorder(subtree->leftChild);
        Preorder(subtree->rightChild);
    }
}
```

- 中序
==左 → 根 → 右==
```cpp
// 左根右
void Inorder(BinaryTreeNode<T>* &subtree) {
    if(subtree != NULL) {
        Inorder(subtree->leftChild);
        cout << subtree->data << " ";
        Inorder(subtree->rightChild);
    }
}
```

- 後序
==左 → 右 → 根==
```cpp
// 左右根
void Postorder(BinaryTreeNode<T>* &subtree) {
    if(subtree != NULL) {
        Postorder(subtree->leftChild);
        Postorder(subtree->rightChild);
        cout << subtree->data << " ";
    }
}
```

### 非遞規遍歷
- 先序
==根 → 左 → 右==
這邊為了將遞歸過程改成非遞規的，我們要使用棧，以便紀錄回退路徑。看看下圖非常清晰:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-4.png)
每次訪問一個節點後，在向左子樹遍歷下去之前，我們要先用棧紀錄右子樹的節點(如果有右子樹)，這樣當我們遍歷完左子樹時，我們才能直接從棧中取得右子樹的根結點，繼續右子樹的先序遍歷。

```cpp
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
```

- 中序
==左 → 根 → 右==
下圖是非遞規的中序遍歷過程，這邊還是使用棧來實現。具體思路是，在一棵子樹中，首先訪問的節點位於從根開始沿 leftChild 走到最左下角的節點，而該節點的 leftChild 為 NULL。訪問完他的數據後，接著再訪問它的右子樹，而該右子樹又是個二叉樹，於是重複上述步驟直到遍歷完成。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-5.png)
如果某節點的右子樹遍歷完或是右子樹為空，那就說明以該節點為根的二叉樹遍歷完成，此時就要從棧中退出更上層的節點並訪問它，再繼續遍歷它的右子樹。

```cpp
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
```

- 後序
==左 → 右 → 根==
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-6.png)
後序遍歷的非遞規主要思想如下，以上圖為例:
1. 如果棧頂元素非 NULL 且左節點存在，將其與其左節點都壓入棧，重複該過程直到對於某節點不存在左節點。
2. 判斷上一次出棧的節點是否是當前棧頂節點的右節點(比如 g,f 節點)，或是當前棧頂節點不存在右節點(比如 g,f,a 節點)，則將當前節點輸出，並出棧。否則，將當前棧頂節點的右節點壓棧，再進入第 1 步

```cpp
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
```
### 層次遍歷
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/tree-7.png)
按層次遍歷需要用到隊列。在訪問二叉樹某一層的節點時，把下一層節點預先記憶在隊列中，利用隊列安排逐層訪問的次序。因此，每當訪問一個節點時，要把他的子女依序加到隊列的隊尾，然後訪問隊頭的節點。
```cpp
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
```

## 二叉樹節點個數
```cpp
int size(BinaryTreeNode<T> *subTree) {
    if(subTree == NULL) {
        return 0;
    }
    return 1 + Size(subTree->leftChild) + Size(subTree->rightChild);
}
```
## 二叉樹高度
```cpp
int height(BinaryTreeNode<T> *subTree) {
    if(subTree == NULL) {
        return 0;
    }
    int leftHeight = Height(subTree->leftChild);
    int rightHeight = Height(subTree->rightChild);
    return (leftHeight < rightHeight) ? rightHeight + 1 : leftHeight + 1;
}
```

## 二叉樹打印(廣義表形式)
```cpp
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
```

## 獲取二叉樹某一節點的父節點
```cpp
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
```

## 判斷二叉樹是否為空
```cpp
// 判斷二叉樹是否為空
int isEmpty() { return (root == NULL); }
```

## 判斷兩棵二叉樹是否相等
```cpp
bool equal(BinaryTreeNode<T>* a, BinaryTreeNode<T>* b) {
    if(a == NULL && b == NULL) {
        return true;
    }
    if((a != NULL && b != NULL) && (a->data == b->data) && (equal(a->leftChild, b->leftChild)) && (equal(a->rightChild, b->rightChild))) {
        return true;
    }
    return false;
}
```

## 二叉樹銷毀
```cpp
void Destroy(BinaryTreeNode<T>* &subTree) {
    if(subTree != NULL) {
        Destroy(subTree->leftChild);
        Destroy(subTree->rightChild);
        delete subTree;
        subTree = NULL;
    }
}
```

## 完整代碼
```cpp
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
```
