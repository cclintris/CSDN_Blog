/*
 * Graph in Adjacency Matrix
 * 圖的鄰接矩陣表示
 * */

#include <iostream>
#include <queue>
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

/* BFS */
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

int main() {
    Graph myGraph = buildGraph();
    printGraph(myGraph);
    BFS(myGraph, 0);
}
