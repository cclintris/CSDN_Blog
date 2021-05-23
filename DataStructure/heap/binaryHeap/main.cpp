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
