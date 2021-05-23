@[TOC](C++ 理解與實現堆和二叉堆)

# 堆的定義
堆的英文是 heap，通常可以看作是一棵樹，且滿足下列要求:
1. 堆中任意節點的值總是不大(小)於任意子節點的值
2. 堆總是一棵完全樹

其中還有分最小堆以及最大堆。對任意節點不大於其子節點的就是最小堆，相反的，對任意節點不小於其子節點的就是最大堆。

# 二叉堆
二叉堆其實就是完全二叉樹。一樣也分為最小堆以及最大堆。如下圖:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-1.png)

而通常二叉堆的實現是通過數組來實現的。數組實現的二叉堆，父子節點之間會存在一定的關係。這邊我們將二叉堆的第一個元素放在數組的第 0 個位置。那麼父子節點就會有以下關係:
1. 索引為 i 的左子節點索引為 (2*i + 1)
2. 索引為 i 的右子節點索引為 (2*i + 2)
3. 索引為 i 的父節點索引為 (floor((i-1) / 2))
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-2.png)

有時候我們會把二叉堆的第一個元素放在數組的第 1 個位置，其實也大同小異，只是有一點小小的區別而已:
1. 索引為 i 的左子節點索引為 (2*i)
2. 索引為 i 的右子節點索引為 (2*i + 1)
3. 索引為 i 的父節點索引為 floor(i/2)
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-3.png)

本文都是採用將二叉堆第一個元素放在第 0 個位置實現的。接下來要討論的是關於二叉堆的操作以及實現，而因為最大堆其實和最小堆基本相同，所以下面都用最大堆進行介紹。

## 基本框架
```cpp
template <typename T>
class MaxHeap {
public:
    MaxHeap(int capacity);
    ~MaxHeap();

    // 返回 data 在二叉堆中的索引
    int getIndex(T data);

    // 刪除最大堆中的 data
    int remove(T data);

    // 將 data 添加到二叉堆中
    int insert(T data);

    // 打印二叉堆
    void print();

private:
    T *data; // 數據
    int capacity; // 總的容量
    int size; // 實際容量

    // 最大堆向下調整
    void filterDown(int start, int end);

    // 最大堆向上調整(從 start 開始直到 0)
    void filterUp(int start);
};
```
二叉堆的核心其實就是添加和刪除元素，所以接著就來看看這兩個操作如何實現!

## 添加
假設要在二叉堆 [90,80,70,60,40,30,20,10,50] 中添加 85，過程如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-2.jpg)
如上圖所示，添加的過程首先就是將數據添加到二叉堆的最後，接著盡可能的把元素往上移動，直到移不動為止。以這個例子來說，添加後的二叉堆變成 [90,85,70,60,80,30,20,10,50,40]。

- 代碼實現:
```cpp
template <typename T>
void MaxHeap<T>::filterUp(int start) {
    int current = start; // 當前節點位置
    int parent = (current-1)/2; // 當前節點的父節點位置
    T tmp = mHeap[current]; // 當前節點數據

    while (current > 0) {
        if(mHeap[parent] > tmp) {
            break;
        }else {
            mHeap[current] = mHeap[parent];
            current = parent;
            parent = (parent-1)/2;
        }
    }
    mHeap[current] = tmp;
}

/*
 * @return 0:成功 / -1:失敗
 * */
template <typename T>
int MaxHeap<T>::insert(T data) {
    if(size == capacity) {
        return -1;
    }
    mHeap[size] = data;
    filterUp(size);
    size++;

    return 0;
}
```

## 刪除
假設要從二叉堆 [90,85,70,60,80,30,20,10,50,40] 中刪除 90，過程如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-3.jpg)
如上圖所示，當要從二叉堆中刪除數據時，首先用最大堆的最後一個元素替換這個空位，接著，對二叉堆往下做調整，直到二叉堆二叉堆變成一個最大堆。以這個例子來說，刪除 90 後的二叉堆變成 [85,80,70,60,40,30,20,10,50]。

- 代碼實現:
```cpp
/*
 * 向下調整通常從被替換的元素索引，到最後一個元素索引
 * @param start:被替換的元素索引, end:最後一個元素索引
 * */
template <typename T>
void MaxHeap<T>::filterDown(int start, int end) {
    int current = start; // 當前節點位置
    int l = 2*current+1; // 當前節點的左子節點位置
    T tmp = mHeap[current]; // 當前節點數據

    while (l <= end) {
        // l 為左子節點，l+1 為右子節點
        if(l < end && mHeap[l] < mHeap[l+1])
            l++;
        if(tmp >= mHeap[l])
            break;
        else {
            mHeap[current] = mHeap[l];
            current = l;
            l = 2*l + 1;
        }
    }
    mHeap[current] = tmp;
}

/*
 * @return 1:成功 / -1:失敗
 * */
template <typename T>
int MaxHeap<T>::remove(T data) {
    if(size == 0)
        return -1;

    int index = getIndex(data);
    if(index == -1)
        return -1;

    mHeap[index] = mHeap[--size]; // 用最後一個元素替換
    filterDown(index, size-1);
    return 1;
}
```

簡單來說，二叉堆的核心就是添加和插入兩個操作，而添加配合的是向上調整的算法，刪除配合的是向下調整的算法。

## 完整代碼實現
```cpp
#include <iostream>
using namespace std;

template <typename T>
class MaxHeap {
public:
    MaxHeap(int capacity);
    ~MaxHeap();

    // 返回 data 在二叉堆中的索引
    int getIndex(T data);

    // 刪除最大堆中的 data
    int remove(T data);

    // 將 data 添加到二叉堆中
    int insert(T data);

    // 打印二叉堆
    void print();

private:
    T *mHeap; // 數據
    int capacity; // 總的容量
    int size; // 實際容量

    // 最大堆向下調整
    void filterDown(int start, int end);

    // 最大堆向上調整(從 start 開始直到 0)
    void filterUp(int start);
};

template <typename T>
MaxHeap<T>::MaxHeap(int capacity) {
    capacity = capacity;
    size = 0;
    mHeap = new T[capacity];
}

template <typename T>
MaxHeap<T>::~MaxHeap() {
    capacity = 0;
    size = 0;
    delete []mHeap;
}

/*
 * @return i:找到索引並返回 / -1:不存在
 * */
template <typename T>
int MaxHeap<T>::getIndex(T data) {
    for(int i = 0; i < size; i++) {
        if(mHeap[i] == data) {
            return i;
        }
    }
    return -1;
}

template <typename T>
void MaxHeap<T>::filterUp(int start) {
    int current = start; // 當前節點位置
    int parent = (current-1)/2; // 當前節點的父節點位置
    T tmp = mHeap[current]; // 當前節點數據

    while (current > 0) {
        if(mHeap[parent] > tmp) {
            break;
        }else {
            mHeap[current] = mHeap[parent];
            current = parent;
            parent = (parent-1)/2;
        }
    }
    mHeap[current] = tmp;
}

/*
 * @return 1:成功 / -1:失敗
 * */
template <typename T>
int MaxHeap<T>::insert(T data) {
    if(size == capacity) {
        return -1;
    }
    mHeap[size] = data;
    filterUp(size);
    size++;

    return 1;
}

/*
 * 向下調整通常從被替換的元素索引，到最後一個元素索引
 * @param start:被替換的元素索引, end:最後一個元素索引
 * */
template <typename T>
void MaxHeap<T>::filterDown(int start, int end) {
    int current = start; // 當前節點位置
    int l = 2*current+1; // 當前節點的左子節點位置
    T tmp = mHeap[current]; // 當前節點數據

    while (l <= end) {
        // l 為左子節點，l+1 為右子節點
        if(l < end && mHeap[l] < mHeap[l+1])
            l++;
        if(tmp >= mHeap[l])
            break;
        else {
            mHeap[current] = mHeap[l];
            current = l;
            l = 2*l + 1;
        }
    }
    mHeap[current] = tmp;
}

/*
 * @return 1:成功 / -1:失敗
 * */
template <typename T>
int MaxHeap<T>::remove(T data) {
    if(size == 0)
        return -1;

    int index = getIndex(data);
    if(index == -1)
        return -1;

    mHeap[index] = mHeap[--size]; // 用最後一個元素替換
    filterDown(index, size-1);
    return 1;
}

template <typename T>
void MaxHeap<T>::print() {
    for(int i = 0; i < size; i++) {
        if(i == size-1)
            cout << mHeap[i] << " " << endl;
        else
            cout << mHeap[i] << " ";
    }
}

int main() {
    int data[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
    int len = sizeof(data)/sizeof(data[0]);

    auto *maxHeap = new MaxHeap<int>(20);

    cout << "Insert in order : ";
    for(int i = 0; i < len; i++) {
        cout << data[i] << " ";
        maxHeap->insert(data[i]);
    }
    cout << endl;

    cout << "MaxHeap : ";
    maxHeap->print();
    cout << "Insert data : " << 85 << endl;
    maxHeap->insert(85);
    cout << "MaxHeap : ";
    maxHeap->print();
    cout << "Remove data : " << 60 << endl;
    maxHeap->remove(60);
    cout << "MaxHeap : ";
    maxHeap->print();
}
```
運行結果如下:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/heap-4.png)
