@[TOC](C++ 實現 Queue)

# Queue 是什麼?
Queue 的中文就是隊列，其實就跟大家熟悉的棧一樣，是一種線性存儲的數據結構，其應用也十分廣泛。隊列有以下特點:
1. 隊列中的數據是按照 FIFO(First In First Out) 先進先出的規則出入的。
2. 隊列只允許在「隊首」進行刪除操作，只允許在「隊尾」進行添加操作。

所以其實實現隊列也相對簡單，最主要就是出隊和入隊兩個操作。
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/queue1.jpg)

# 代碼實現
```cpp
#include <iostream>
using namespace std;

template <typename T>
class Queue {
public:
    Queue(int size);
    ~Queue();

    void add(T data);
    T pop();
    T front();
    T tail();
    int getSize();
    void isEmpty();
    void printQueue();

private:
    T *arr;
    int ptr = 0; // arr數組位置紀錄

};

// 創建隊列，大小為size
template <typename T>
Queue<T>::Queue(int size) {
    arr = new T[size];
    if(!arr) {
        cout << "allocate memory bug!";
    }
}

// 銷毀隊列
template <typename T>
Queue<T>::~Queue<T>() {
    if(arr) {
        delete []arr;
        arr = nullptr;
    }
}

// 添加數據
template <typename T>
void Queue<T>::add(T data) {
    arr[ptr++] = data;
}

// 返回並刪除隊頭數據
template <typename T>
T Queue<T>::pop() {
    int i = 0;
    T data = arr[0];

    ptr--;
    while(i++ < ptr) {
        arr[i - 1] = arr[i];
    }

    return data;
}

// 返回隊頭數據
template <typename T>
T Queue<T>::front() {
    return arr[0];
}

// 返回隊尾數據
template <typename T>
T Queue<T>::tail() {
    return arr[ptr - 1];
}

// 返回隊列大小
template <typename T>
int Queue<T>::getSize() {
    return ptr;
}

// 返回隊列是否為空
template <typename T>
void Queue<T>::isEmpty() {
    if(ptr == 0) {
        cout << "Queue is empty" << endl;
    }else {
        cout << "Queue is not empty" << endl;
    }
}

// 打印隊列數據
template <typename T>
void Queue<T>::printQueue() {
    cout << "IN -> ";
    for(int i = 0; i < ptr; i++) {
        if(i == ptr - 1) {
            cout << arr[ptr - 1 - i];
        }else {
            cout << arr[ptr - 1 - i] << " -> ";
        }
    }
    cout << " -> OUT";
    cout << endl;
}

int main() {
    auto *queue = new Queue<int>(10);
    queue->isEmpty();
    queue->add(1);
    queue->add(2);
    queue->add(3);
    queue->add(4);
    queue->add(5);
    queue->printQueue();
    cout << "Queue size : " << queue->getSize() << endl;
    // queue->printPtr();
    cout << "Queue front : " << queue->front() << endl;
    cout << "Queue tail : " << queue->tail() << endl;
    int pop = queue->pop();
    cout << "Queue pop : " << pop << endl;
    cout << "Queue size : " << queue->getSize() << endl;
    queue->printQueue();
    queue->isEmpty();
    int pop1 = queue->pop();
    cout << "Queue pop : " << pop1 << endl;
    cout << "Queue size : " << queue->getSize() << endl;
    queue->printQueue();
}
```
