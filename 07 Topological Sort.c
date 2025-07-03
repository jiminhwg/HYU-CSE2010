#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

/*
구현해야 하는 함수들
*/
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void TopologicalSort(Graph* G);
Queue* CreateQueue();
bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
/*
구현하면 좋은 함수들
*/
// Indegree 배열 설정
void SetIndegree(Graph* G, int* idg);
// Bubble sort도 무방
void SortNode(int* arr, int n);



void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){
    Graph* G = (Graph*)malloc(sizeof(Graph)); //graph malloc
    G->size = n;
    G->node = (int*)malloc(sizeof(int)*n);
    memcpy(G->node, nodes, sizeof(int)*n); //copy memory

    G->matrix = (bool**)malloc(sizeof(bool*)*n); //2d array
    for (int i=0; i < n; i++) {
        G->matrix[i] = (bool*)malloc(sizeof(bool)*n); //내부 할당
        memset(G->matrix[i], false, sizeof(bool)*n); //setting emmory
    }

    return G;
}

void InsertEdge(Graph* G, int a, int b){
    int aIndex = -1, bIndex = -1; //two diff indices
    for (int i=0; i < G->size; i++) { 
        if (G->node[i] == a) { //if a
            aIndex = i;
        }
        if (G->node[i] == b) { //if b
            bIndex = i;
        }
    }
    if (aIndex != -1 && bIndex != -1) { //if both a & b
        G->matrix[aIndex][bIndex] = true; //true
    }

}

void PrintGraph(Graph* G){
    fprintf(fout, "%d ", 0);
    for (int i = 0; i < G->size; i++) { //top
        fprintf(fout, "%d ", G->node[i]); //node의 key값 
    }
    fprintf(fout, "\n");
    for (int i = 0; i < G->size; i++) {
        fprintf(fout, "%d ", G->node[i]); //side
        for (int j = 0; j < G->size; j++) {
            fprintf(fout, "%d ", G->matrix[i][j]); //print elem in matrix
        }
        fprintf(fout, "\n");
    }
    // 출력(띄어쓰기에 주의)
    // 0 2 4 5 6
    // 2 0 0 0 1
    // 4 0 0 0 0
    // 5 1 0 1 0
    // 6 0 0 1 1
    // 위와 같은 형태로 출력이 목표(bool 은 %d로 0 혹은 1로 출력 가능)
}

void DeleteGraph(Graph* G){
    for (int i=0; i < G->size; i++) { //free elements of matrices in g
        free(G->matrix[i]);
    }
    free(G->matrix); //free matrix
    free(G->node); //free g's node
    free(G); //free g
}

void TopologicalSort(Graph* G){
    int* indegree = (int*)malloc(sizeof(int)*G->size);
    SetIndegree(G, indegree); //초기화
    Queue* Q = CreateQueue(); //create queue
    for (int i = 0; i < G->size; i++) {
        if (indegree[i] == 0) { //if indeg = 0, insert in queue
            Enqueue(Q, i);
        }
    }

    int* printOrder = (int*)malloc(sizeof(int)*G->size);
    int printOrder_size = 0;
    while (IsEmpty(Q) == false) { //until empty
        int nodeIndex = Dequeue(Q); //dequeue
        printOrder[printOrder_size++] = G->node[nodeIndex];
        for (int i = 0; i < G->size; i++) {
            if (G->matrix[nodeIndex][i] == true) { //if there exists edge
                indegree[i]--; 
                if (indegree[i] == 0) { //if indegree = 0, insert in queue
                    Enqueue(Q, i);
                }
            }
        }
    }

    if (printOrder_size != G->size) { //cycle
        fprintf(fout, "sorting error : cycle\n");
    } 
    else {
        for (int i = 0; i < printOrder_size; i++) {
            fprintf(fout, "%d ", printOrder[i]);
        }
        fprintf(fout, "\n");
    }

    free(printOrder); //free
    free(indegree);
    DeleteQueue(Q);
}

Queue* CreateQueue(){
    Queue* Q = (Queue*)malloc(sizeof(Queue)); //malloc
    Q->front = Q->rear = NULL; //linkedlist
    Q->size = 0; //setting size = 0
    return Q;
}

bool IsEmpty(Queue* Q){
    if (Q->size==0) {
        return true; //empty
    }
    return false;
}

int Dequeue(Queue* Q){
    if (IsEmpty(Q) == true) { //if empty
        return -1;
    }
    QueueNode* temp = Q->front; //getting front node
    int data = temp->data;
    Q->front = Q->front->next;
    if (Q->front == NULL) {
        Q->rear = NULL;
    }
    free(temp);
    Q->size--;
    return data;
    
}

void Enqueue(Queue* Q, int X){
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode)); //inserting at end
    temp->data = X;
    temp->next = NULL;
    if (Q->rear == NULL) { //if empty
        Q->front = Q->rear = temp; //새로만든node
    } 
    else {
        Q->rear->next = temp;
        Q->rear = temp;
    }
    Q->size++;
    
}

void DeleteQueue(Queue* Q){
    while(IsEmpty(Q)==false) {//while not yet empty
        Dequeue(Q); //dequeue Q
    }
    free(Q);
}

void SetIndegree(Graph* G, int* idg){
    memset(idg, 0, sizeof(int)*G->size);//초기화 by setting memory
    for (int i = 0; i < G->size; i++) {
        for (int j = 0; j < G->size; j++) {
            if (G->matrix[i][j] == true) { //if true
                idg[j]++; //++
            }
        }
    }
   
}

void SortNode(int* arr, int n){
    for (int i=0; i < n-1; i++) {
        for (int j=0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) { //sorting (if left is bigger than right)
                int temp = arr[j]; //swap
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
   
}