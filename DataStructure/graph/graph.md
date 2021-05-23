@[TOC](C++ 理解與實現 圖Graph)

# 前言
當時在學習數據結構時，可能基礎不好加上沒有耐心，導致學習的不是很明白，而圖論也一直是我覺得較困難抽象的數據結構。藉著這篇就來記錄學習關於圖的數據結構以及相關算法吧!

# 正文

## 什麼是圖?
圖其實可以說是一個比較有意思的數據結構，比較抽象但其實理解後也不會到太難。

圖其實就是用一個集合來裝一些相互連接的邊和頂點，這個集合就叫做圖，通常用 `G (V, E)` 表示，其中 G 表 Graph(圖)，V 表 Vertex(頂點)，E 表 Edge(邊)。這種邊和頂點存在著一對多的關係，且各頂點有邊並帶有某種聯繫的抽象關係就叫做圖。

看下圖(底部截圖不完全請見諒):
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-1.png)

上圖是一個村落圖，我們就可以把每個村落看成是每個頂點，而連接村落與村落之間的道路就是邊。

- 有向圖 vs 無向圖

圖分為兩種，有向圖和無向圖。如果一張圖中，有點 A,B，他們之間既可以從 A 到 B 也可以從 B 到 A，那該圖就是無向圖，AB 邊表示成 (A, B)。如果只是單向的，那就是有向圖，AB 邊表示成 <A, B>。

此外，根據邊的規定多少，也可以分為稀疏圖和稠密圖。一張每兩點間都有邊的圖則稱為完全圖。

- 權(Weight)

再來，我們還可以引入權重的概念。甚麼是權呢?以上圖來說，每條邊的距離就可以稱為邊的權。權其實可以理解為邊對於其所連接的兩點的強度的體現。

- 度(Degree)

度也是圖中的一個概念，其實就是對於一個頂點有幾條邊聯繫著它，度就是多少。其中在有向圖中也可以分為出度和入度，出度就從該頂點向外所聯繫的邊數，反之則為入度。以上圖的李家村來說，度 = 4。 

## 圖的實現
有了基本概念，當然接下來就是要來代碼實現!

### 圖的存儲表示

- 鄰接矩陣表示
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-3.webp)

在鄰接矩陣中，行和列都表示頂點，由兩個頂點對應的矩陣元素決定這兩個頂點之間是否相連，權重大小。但這樣的方式很耗費存儲空間，且要加入邊或是頂點成本相對昂貴。

因此也有鄰接表的表示，大部分時候還是採取下面提到的鄰接表實現方式。不過還是來看看鄰接矩陣的代碼實現!

```cpp
/*
 * Graph in Adjacency Matrix
 * 圖的鄰接矩陣表示
 * */

#include <iostream>
using namespace std;

#define MaxVertexNum 100 // 最大頂點數設為100
#define EMPTY -1 // 空值定義為 -1
typedef int Vertex; // 用頂點下標表示頂點，int型
typedef int Weight; // 邊的權為int
typedef char DataType; // 頂點存儲的數據為char型

/* 邊定義 */
typedef struct ENode *ENodePtr;
struct ENode {
    Vertex V1, V2; // 有向邊 <V1, V2>
    Weight W; // 權重
};
typedef ENodePtr Edge;

/* 圖定義 */
typedef struct GNode *GNodePtr;
struct GNode {
    int Nv; // 頂點數
    int Ne; // 邊數
    Weight M[MaxVertexNum][MaxVertexNum]; // 鄰接矩陣
    DataType Data[MaxVertexNum]; // 存頂點數據
    /* 很多情況下頂點無數據，則 Data 無須出現 */
};
typedef GNodePtr Graph;

// 初始化一個有 VertexNum 個頂點但沒有邊的圖
Graph createGraph(int VertexNum) {
    Graph graph;

    graph = (Graph)malloc(sizeof(GNode));
    graph->Nv = VertexNum;
    graph->Ne = 0;
    /*
     * 初始化鄰接矩陣，頂點編號默認從 0 開始一直到 G->Nv -1
     * */
    for(int i = 0; i < graph->Nv; i++) {
        for(int j = 0; j < graph->Nv; j++) {
            graph->M[i][j] = EMPTY;
        }
    }

    return graph;
}

// 添加邊
void addEdge(Graph graph, Edge edge, bool direct) {
    /* 添加有向邊 <V1, V2> */
    graph->M[edge->V1][edge->V2] = edge->W;
    /* 若為無向邊，還要添加 <V2, V1> */
    if(!direct)
        graph->M[edge->V2][edge->V1] = edge->W;
}

// 建立圖
Graph buildGraph() {
    Graph graph;
    Vertex V;
    Edge E;
    int Nv;
    bool direct;

    string s;
    cout << "Create direct graph ? (Yes/No)" << endl;
    cin >> s;
    if(s == "Yes") {
        direct = true;
    }else {
        direct = false;
    }

    cout << "Enter graph vertices :";
    cin >> Nv; // 讀入頂點個數
    graph = createGraph(Nv); // 初始化一個有 Nv 個頂點但沒有邊的圖

    cout << "Enter graph edges :";
    cin >> graph->Ne; // 讀入邊數
    if(graph->Ne != 0) { // 如果有邊
        E = (Edge)malloc(sizeof(ENode));
        // 讀入邊，格式為 "起點 終點 權重"，加入鄰接矩陣
        for(int i = 0; i < graph->Ne; i++) {
            cout << "Edge" << i << " (start end weight):" << endl;
            cin >> E->V1 >> E->V2 >> E->W;
            addEdge(graph, E, direct);
        }
    }

    string s1;
    cout << "Graph node has data ? (Yes/No)" << endl;
    cin >> s1;
    if(s1 == "Yes") {
        for(V = 0; V < graph->Nv; V++) {
            cout << "Node" << V << " data :";
            cin >> graph->Data[V];
        }
    }

    return graph;
}

/* 打印圖 */
void printGraph(Graph graph) {
    cout << "Printing Graph......";
    cout << "Graph vertices :" << graph->Nv << endl;
    cout << "Graph edges :" << graph->Ne << endl;

    cout << "  ";
    for(int i = 0; i < graph->Nv; i++) {
        cout << graph->Data[i] << " ";
    }
    cout << endl;

    for(int i = 0; i < graph->Nv; i++) {
        cout << graph->Data[i] << " ";
        for(int j = 0; j < graph->Nv; j++) {
            cout << graph->M[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    Graph myGraph = buildGraph();
    printGraph(myGraph);
}
```

- 鄰接表表示
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-2.webp)

鄰接表的實現如上圖所示，每個頂點會存儲從它這裡開始 1 步能到達下個頂點的邊的列表。比如，從頂點 A 有 1 條邊到可以到頂點 B,C,D，那麼 A 的列表中會有 3 個邊。

不過，鄰接列表卻只描述了指向外部的邊，比如，A 有到 B 的邊，但 B 沒有到 A 的邊，所以 A 就沒有出現在 B 的列表中。

代碼實現如下:

```cpp
/*
 * Graph in Adjacency List
 * 圖的鄰接表表示
 * */

#include <iostream>
using namespace std;

#define MaxVertexNum 100 // 最大頂點數設為100
typedef int Vertex; // 用頂點下標表示頂點，int型
typedef int Weight; // 邊的權為int
typedef char DataType; // 頂點存儲的數據為char型
int index[MaxVertexNum]; // 鄰接表索引

/* 邊定義 */
typedef struct ENode *ENodePtr;
struct ENode {
    Vertex V1, V2; // 有向邊 <V1, V2>
    Weight W; // 權重
};
typedef ENodePtr Edge;

/* 鄰接點定義 */
typedef struct AdjNode *AdjNodePtr;
struct AdjNode {
    Vertex AdjV; // 鄰接點下標
    Weight W; // 邊權重
    AdjNodePtr next; // 指向下一個鄰接點的指針
};

/* 頂點表頭節點定義 */
typedef struct VNode {
    AdjNodePtr FirstEdge; // 鄰接表表頭指針
    Vertex AdjList[MaxVertexNum]; // 完整鄰接表
    DataType Data; // 存頂點數據
    /* 很多情況下頂點無數據，則 Data 無須出現 */
}AdjList[MaxVertexNum]; // AdjList 是鄰接表類型

/* 圖定義 */
typedef struct GNode *GNodePtr;
struct GNode {
    int Nv; // 頂點數
    int Ne; // 邊數
    AdjList G; // 鄰接表
};
typedef GNodePtr Graph;

void init(int VertexNum) {
    for(int i = 0; i < VertexNum; i++)
        index[i] = 0;
}

// 初始化一個有 VertexNum 個頂點但沒有邊的圖
Graph createGraph(int VertexNum) {
    Graph graph;
    Vertex V;

    graph = (Graph)malloc(sizeof(GNode));
    graph->Nv = VertexNum;
    graph->Ne = 0;
    /*
     * 初始化鄰接表頭指針，頂點編號默認從 0 開始一直到 G->Nv -1
     * */
    for(V = 0; V < graph->Nv; V++) {
        graph->G[V].FirstEdge = nullptr;
        graph->G[V].AdjList[index[V]] = V;
    }

    for(V = 0; V < graph->Nv; V++) {
        index[V]++;
    }

    return graph;
}

// 添加邊
void addEdge(Graph graph, Edge edge, bool direct) {
    AdjNodePtr newNode;

    /* 添加有向邊 <V1, V2> */
    /* 為 V2 建立新的鄰接點 */
    newNode = (AdjNodePtr)malloc(sizeof(AdjNode));
    newNode->AdjV = edge->V2;
    newNode->W = edge->W;
    /* 將 V2 插入 V1 的表頭 */
    newNode->next = nullptr;
    graph->G[edge->V1].FirstEdge = newNode;
    graph->G[edge->V1].AdjList[index[edge->V1]] = newNode->AdjV;
    index[edge->V1]++;
    // cout << "Here d " << index[edge->V1] << endl;

    if(!direct) { /* 若為無向邊，則還需添加 <V2, V1> */
        newNode = (AdjNodePtr)malloc(sizeof(AdjNode));
        newNode->AdjV = edge->V1;
        newNode->W = edge->W;
        /* 將 V1 插入 V2 的表頭 */
        newNode->next = nullptr;
        graph->G[edge->V2].FirstEdge = newNode;
        graph->G[edge->V2].AdjList[index[edge->V2]] = newNode->AdjV;
        index[edge->V2]++;
    }
}

// 建立圖
Graph buildGraph() {
    Graph graph;
    Vertex V;
    Edge E;
    int Nv;
    bool direct;

    string s;
    cout << "Create direct graph ? (Yes/No)" << endl;
    cin >> s;
    if(s == "Yes") {
        direct = true;
    }else {
        direct = false;
    }

    cout << "Enter graph vertices :";
    cin >> Nv; // 讀入頂點個數
    graph = createGraph(Nv); // 初始化一個有 Nv 個頂點但沒有邊的圖

    cout << "Enter graph edges :";
    cin >> graph->Ne; // 讀入邊數
    if(graph->Ne != 0) { // 如果有邊
        E = (Edge)malloc(sizeof(ENode));
        // 讀入邊，格式為 "起點 終點 權重"，加入鄰接表
        for(int i = 0; i < graph->Ne; i++) {
            cout << "Edge" << i << " (start end weight):" << endl;
            cin >> E->V1 >> E->V2 >> E->W;
            addEdge(graph, E, direct);
        }
    }

    string s1;
    cout << "Graph node has data ? (Yes/No)" << endl;
    cin >> s1;
    if(s1 == "Yes") {
        for(V = 0; V < graph->Nv; V++) {
            cout << "Node" << V << " data :";
            cin >> graph->G[V].Data;
        }
    }

    return graph;
}

/* 打印圖 */
void printGraph(Graph graph) {
    cout << "Printing Graph......" << endl;
    cout << "Graph vertices :" << graph->Nv << endl;
    cout << "Graph edges :" << graph->Ne << endl;

    for(int i = 0; i < graph->Nv; i++) {
        cout << "Adjacency List " << i << " firstEdge : ";
        if(graph->G[i].FirstEdge == nullptr)
            cout << "None" << endl;
        else
            cout << graph->G[i].FirstEdge->AdjV << endl;
    }

    for(int i = 0; i < graph->Nv; i++) {
        cout << "Adjacency List " << i << " : ";
        // cout << index[i] << endl;
        for(int j = 0; j < index[i]; j++) {
            if(j == index[i] -1)
                cout << graph->G[i].AdjList[j];
            else
                cout << graph->G[i].AdjList[j] << " -> ";
        }
        cout << endl;
    }
}

int main() {
    Graph graph = buildGraph();
    printGraph(graph);
}
```
最後也附上一個表格，整理了兩種表示法的複雜度:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-2.png)

### 圖的遍歷
圖的遍歷分為兩種，DFS(深度優先遍歷)和 BFS(廣度優先遍歷)。

#### DFS(深度優先)
深度優先遍歷其實就很像樹的先序遍歷，它會通過類似堆棧的方式，先在起點隨意選擇一條邊走，一直走下去走到死胡同為止，然後再一個個回退，如果有沒有走的邊，就再走，一直到退回起點，確定都沒有路可以走了，就結束 DFS。

代碼實現如下:
```cpp
/* 
DFS
基於鄰接表實現 
*/

bool visited[MaxVertexNum] = { false };

void Visit(Vertex V) {
    cout << "Visiting vertex " << V << endl;
}

void DFS(Graph graph, Vertex V) {
    Visit(V);
    visited[V] = true;

    for(AdjNodePtr W = graph->G[V].FirstEdge; W; W = W->next) {
        if(!visited[W->AdjV])
            DFS(graph, W->AdjV);
    }
}
```

#### BFS(廣度優先)
廣度優先其實也就是一層層蔓延，直接看下面網上的圖，非常清楚:

首先給定一個起點:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-4.png)
之後往外一個個遍歷:
![](https://wtfhhh.oss-cn-beijing.aliyuncs.com/graph-5.png)
之後就一直這樣擴散，直到所有節點都遍歷結束。

代碼實現如下:
```cpp
/* 
BFS
基於鄰接矩陣實現 
*/

bool visited[MaxVertexNum] = { false };

bool isEdge(Graph graph, Vertex V1, Vertex V2) {
    return graph->M[V1][V2] == EMPTY;
}

void Visit(Vertex V) {
    cout << "Visiting vertex " << V << endl;
}

void BFS(Graph graph, Vertex S) {
    queue<int> Q;
    Vertex V, W;

    Visit(S);
    visited[S] = true;
    Q.push(S);

    while (!Q.empty()) {
        V = Q.front();
        Q.pop();
        for(W = 0; W < graph->Nv; W++) {
            if(!visited[W] && isEdge(graph, V, W)){
                Visit(W);
                visited[W] = true;
                Q.push(W);
            }
        }
    }
}
```

# 結語
這篇基本介紹了關於數據結構中圖的存儲兩種存儲表示，以及兩種遍歷方式，理解下應該還好理解。其實關於圖還有很多算法，像是最短路徑，最大流等等，之後都會再慢慢學習更新!
