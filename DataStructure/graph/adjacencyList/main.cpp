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
        // cout << "Here " << index[V] << endl;
    }

    for(V = 0; V < graph->Nv; V++) {
        index[V]++;
        // cout << "Here " << index[V] << endl;
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

int main() {
    Graph graph = buildGraph();
    printGraph(graph);
    DFS(graph, 0);
}