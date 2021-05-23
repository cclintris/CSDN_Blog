@[TOC](C++ 理解與實現 linkedlist)

# 什麼是 linkedlist ?
鏈表示一種很常見，也算是最基本的數據結構，在物理上是不連續的，數據元素的邏輯順序是通過每個節點的指針鏈接次序實現的。通常，每個節點包括兩個部分，數據區以及指針。數據區存放的就是當前節點的數據，而指針就是指向的下一個節點。相比於線性表，鏈表更方便插入，刪除。

# 創建 head
當然，一個鏈表要有一個頭節點，這邊稱為 head。一開始總是要手動 new 一個節點作為 head 的。而一開始 head 的 next 指針置為 null 就好。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/ll-1.png)

# 從 head 插入一個新節點
手動 new 一個節點 p，使 p.next 指向本來 head.next 指向的節點，然後把 head.next 指向 p。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/ll-2.png)

# 刪除指定節點
先遍歷到指定節點的前一個節點，然後將該節點指向指定節點指向的下一個節點，然後刪除指定節點就好了。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/ll-3.png)

# 修改指定節點
遍歷到指定節點，然後修改其 data 就好了。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/ll-4.png)

# 反轉 linkedlist
反轉的操作也是鏈表中很常見的要求，具體代碼看下面實現，應該不是很難理解的!

還有一些鏈表包含的操作，因為比較基本，在此暫不用文字詳述，直接來看看代碼的實現!

# C++ 代碼實現
```cpp
#include <iostream>

using namespace std;

typedef int DataType;

struct Node {
    DataType data;
    Node *next;
};

class Linkedlist {
public:
    Linkedlist();
    ~Linkedlist();
    void createLinkedlist(int size);
    void clear();
    DataType get(int pos);
    void insert(Node *p, int pos);
    void erase(int pos);
    void update(int pos, int data);
    int getSize() const;
    bool isEmpty() const;
    void reverse();
    void printList() const;

private:
    Node *head;
    int size;
};

Linkedlist::Linkedlist() {
    head = new Node;
    head->data = 0;
    head->next = nullptr;
    this->size = 0;
}

Linkedlist::~Linkedlist() {
    delete head;
}

void Linkedlist::createLinkedlist(int size) {
    if(size < 0) {
        cout << "Error!" << endl;
        return;
    }
    Node *listptr = this->head;
    Node *newNode;
    this->size = size;
    for(int i = 0; i < size; i++) {
        cout << "Input node" << i + 1 << "'s data:";
        cin >> listptr->data;
        newNode = new Node;
        newNode->next = nullptr;
        newNode->data = 0;
        if(i == size - 1) {
            newNode = nullptr;
        }
        listptr->next = newNode;
        listptr = newNode;
    }
    cout << endl;
    cout << "create size " << size << " linkedlist completed!" << endl;
    cout << endl;
}

void Linkedlist::clear() {
    if(this->head == nullptr) {
        cout << "linkedlist is empty!" << endl;
    }
    Node *tmp;
    while(this->head) {
        tmp = head->next;
        free(this->head);
        this->head = tmp;
        this->size--;
    }
}

DataType Linkedlist::get(int pos) {
    if(pos < 1) {
        cout << "Index too small, linkedlist start from 1, cannot get!" << endl;
    }
    if(pos > this->size) {
        cout << "Index too big, cannot get!" << endl;
    }
    Node *posNode = this->head;
    if(pos == 1) {
        return posNode->data;
    }
    while(pos > 1) {
        posNode = posNode->next;
        pos--;
    }
    return posNode->data;
}

void Linkedlist::insert(Node *p, int pos) {
    if(this->head == nullptr) {
        cout << "linkedlist empty, cannot insert!" << endl;
    }
    if(p == nullptr) {
        cout << "insert node is nullptr, cannot insert!" << endl;
    }

    // insert head
    if(pos == 1) {
        Node *tmp = this->head;
        this->head = p;
        p->next = tmp;
        this->size++;
        return;
    }
    // insert tail
    else if(pos == this->size + 1) {
        Node *last = this->head;
        while(last->next != nullptr) {
            last = last->next;
        }
        last->next = p;
        p->next = nullptr;
        this->size++;
        return;
    }
    // insert middle
    else{
        Node *preposNode = this->head;
        while(pos > 2) {
            preposNode = preposNode->next;
            pos--;
        }
        Node *posNode = preposNode->next;
        preposNode->next = p;
        p->next = posNode;
        posNode = p;
        this->size++;
        return;
    }
}

void Linkedlist::erase(int pos) {
    if(pos < 1) {
        cout << "Index too small, linkedlist start from 1, cannot erase!" << endl;
    }

    if(pos > this->size) {
        cout << "Index too big, cannot erase!" << endl;
    }

    // erase head
    if(pos == 1) {
        Node *tmp = this->head;
        this->head = this->head->next;
        free(tmp);
        this->size--;
        return;
    }
    // erase tail
    if(pos == this->size) {
        Node *prelast = this->head;
        for(int i = 0; i < this->size - 2; i++) {
            prelast = prelast->next;
        }
        Node *last = prelast->next;
        free(last);
        prelast->next = nullptr;
        this->size--;
        return;
    }
    // erase middle
    else{
        Node *preposNode = this->head;
        while(pos > 2) {
            preposNode = preposNode->next;
            pos--;
        }
        Node *posNode = preposNode->next;
        Node *nextposNode = posNode->next;
        preposNode->next = nextposNode;
        free(posNode);
        this->size--;
        return;
    }
}

void Linkedlist::update(int pos, int data) {
    if(pos < 1) {
        cout << "Index too small, linkedlist start from 1, cannot update!" << endl;
        return;
    }
    if(pos > this->size) {
        cout << "Index too big, cannot update!" << endl;
        return;
    }

    Node *listptr = this->head;
    for(int i = 1; i < pos; i++) {
        listptr = listptr->next;
    }
    listptr->data = data;
}

int Linkedlist::getSize() const {
    return this->size;
}

bool Linkedlist::isEmpty() const {
    return this->size == 0;
}

void Linkedlist::reverse() {
    Node *cur = this->head;
    Node *tmp = nullptr;
    this->head = nullptr;
    while(cur) {
        tmp = cur;
        cur = cur->next;
        tmp->next = head;
        head = tmp;
    }
}

void Linkedlist::printList() const {
    if(this->size < 0) {
        cout << "linkedlist size error!" << endl;
    }
    if(this->size == 0) {
        cout << "linkedlist is empty, cannot be printed!" << endl;
    }
    Node *listptr = this->head;
    for(int i = 0; i < this->size; i++) {
        if(i == this->size - 1) {
            cout << listptr->data << endl;
        }else {
            cout << listptr->data << " -> ";
        }
        listptr = listptr->next;
    }
}

int main() {
    auto *linkedlist = new Linkedlist();
    linkedlist->createLinkedlist(5);
    Node *node1 = new Node;
    node1->data = 100;
    cout << "Initial linkedlist" << endl;
    linkedlist->printList();
    cout << "Get operation" << endl;
    DataType data = linkedlist->get(5);
    cout << "Node data get:" << data << endl;
    cout << "Insert operation" << endl;
    linkedlist->insert(node1, 5);
    linkedlist->printList();
    cout << "Erase operation" << endl;
    linkedlist->erase(1);
    linkedlist->printList();
    cout << "Update operation" << endl;
    linkedlist->update(4, 70);
    linkedlist->printList();
    cout << "getSize operation" << endl;
    cout << "Current linkedlist size : " << linkedlist->getSize() << endl;
    cout << "isEmpty operation (0 : NO | 1 : YES)" << endl;
    cout << "Linkedlist empty ? : " << linkedlist->isEmpty() << endl;
    cout << "Reverse operation" << endl;
    linkedlist->reverse();
    linkedlist->printList();
    cout << "Clear operation" << endl;
    linkedlist->clear();
    linkedlist->printList();
    return 0;
}
```
