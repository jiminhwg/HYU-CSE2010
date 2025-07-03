#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct Node {
	int index; //index of this node
	int dist;	//distance for heap
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** distMatrix;
	Node** nodes;
}Graph;

typedef struct Heap {
	int capacity;
	int size;
	Node** element;
}Heap;

//구현해야 하는 함수
Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
Node* Pop(Heap* heap);

//구현하면 좋은 함수
bool isEmpty(Heap* heap){
	if (heap->size == 0) { //if 0 -> empty
		return true;
	}
	else {
		return false;
	}
}
void swap(Node** node1, Node** node2){
	Node* tmp = *node1; //swap 
    *node1 = *node2;
    *node2 = tmp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = CreateGraph(size);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->distMatrix[node1 - 1][node2 - 1] = weight;
		tmp = fgetc(fin);
	}

	PrintShortestPath(g);

	DeleteGraph(g);
}

Graph* CreateGraph(int size) {
	Graph* graph = (Graph*)malloc(sizeof(Graph)); //malloc
    graph->size = size;
    graph->distMatrix = (int**)malloc(size*sizeof(int*)); //malloc
    graph->nodes = (Node**)malloc(size*sizeof(Node*)); //malloc

    for (int i=0; i < size; i++) {
        graph->distMatrix[i] = (int*)malloc(size*sizeof(int)); //malloc
        for (int j=0; j < size; j++) {
			if (i == j) {
				graph->distMatrix[i][j] = 0; //dist from a node to itself is 0
			} 
			else {
				graph->distMatrix[i][j] = INF; 
			}
        }
        graph->nodes[i] = (Node*)malloc(sizeof(Node));
        graph->nodes[i]->index = i; //index = i
        graph->nodes[i]->dist = INF; //dist = inf
        graph->nodes[i]->prev = -1; //prev = -1 (for clarity)
    }
    return graph;
}

void DeleteGraph(Graph* graph){
	for (int i=0; i < graph->size; i++) {
        free(graph->distMatrix[i]); //free elements in matrix
        free(graph->nodes[i]); //free elements in nodes
    }
    free(graph->distMatrix); //free matrix
    free(graph->nodes); //free nodes
    free(graph);
}

Heap* CreateHeap(int heapSize) {
	Heap* heap = (Heap*)malloc(sizeof(Heap)); //malloc
    heap->capacity = heapSize;
    heap->size = 0; //initializing
    heap->element = (Node**)malloc((heapSize+1)*sizeof(Node*));
    return heap;
}

void DeleteHeap(Heap* heap){
	free(heap->element);
    free(heap);
}

void PrintShortestPath(Graph* graph) {
	graph->nodes[0]->dist = 0; //0th elem's dist = 0
    Heap* heap = CreateHeap(graph->size); //create heap
    Push(heap, graph->nodes[0]); //adding 0 to the heap

    while (!isEmpty(heap)) {//while not emptyt
        Node* a = Pop(heap); //pop
        int distOA = a->dist;

        for (int b=0; b < graph->size; b++) { //loop
            if (a->index == b) {
				continue;
			}
            int distOB = graph->nodes[b]->dist;
            int distAB = graph->distMatrix[a->index][b];
            if (distOA + distAB < distOB) { //if distOA + distAB < distOB
                graph->nodes[b]->dist = distOA + distAB;
                graph->nodes[b]->prev = a->index;
                Push(heap, graph->nodes[b]); //push b
            }
        }
    }

    for (int b=1; b < graph->size; b++) {
        if (graph->nodes[b]->dist == INF) { //if not reachable
            fprintf(fout, "can not reach to node %d\n", b + 1);
        } 
		else {
            int path[graph->size];
            int length = 0;
            for (int node = b; node != -1; node = graph->nodes[node]->prev) {  //using prev to trace backwards
                path[length++] = node + 1;
            }
            fprintf(fout, "%d", path[length - 1]); //path
            for (int i = length - 2; i >= 0; i--) {
                fprintf(fout, "->%d", path[i]); 
            }
            fprintf(fout, " (cost : %d)\n", graph->nodes[b]->dist); //dist
        }
    }

    DeleteHeap(heap); //delete
}

void Push(Heap* heap, Node* node) {
	if (heap->size >= heap->capacity) { //if full
		return;
	}
    heap->element[++heap->size] = node; 
    int i = heap->size;
    while (i > 1 && heap->element[i]->dist < heap->element[i/2]->dist) { //
        swap(&heap->element[i], &heap->element[i/2]); //swap 
        i = i/2;
    }
}

Node* Pop(Heap *heap) {
	if (isEmpty(heap) == true) { //if heap is empty
		return NULL;
	}
    Node* minNode = heap->element[1]; //gets min elem 
    heap->element[1] = heap->element[heap->size--];
    int i = 1;
    while (i*2 <= heap->size) { //maintain heap property (ends when heap doesnt havec hild)
        int child = i*2;
        if (child != heap->size && heap->element[child+1]->dist < heap->element[child]->dist) {
            child++;
        }
        if (heap->element[i]->dist <= heap->element[child]->dist) {
			break;
		}
        swap(&heap->element[i], &heap->element[child]); //swap element[i] and element[child]
        i = child;
    }
    return minNode;
}

