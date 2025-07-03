#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	heap->Size = 0;
    heap->Capacity = heapSize;
    heap->Elements = (int*)malloc((heapSize+1) * sizeof(int));  //assigning size for element array
    heap->Elements[0] = INF;  //infinity//easier to check 
    return heap;
}
void DeleteHeap(Heap* heap){
    free(heap->Elements); //free elems in heap
    free(heap); //free heap
}
void Push(Heap *heap, int value){
    if (Find(heap, value)) { //if val is already in heap
        fprintf(fout, "push error : %d is already in the heap\n", value);
        return;
    }
	if (heap->Size == heap->Capacity) { //if heap is full
        fprintf(fout, "push error : heap is full\n");
        return;
    }
    int i = ++heap->Size;
    while (heap->Elements[i/2] < value) { //parent < value
        heap->Elements[i] = heap->Elements[i/2]; //
        i /= 2;
    }
    heap->Elements[i] = value;
}
bool Find(Heap *heap, int value){
	for (int i = 1; i <= heap->Size; i++) {
        if (heap->Elements[i] == value) {//if found
            return true;
        }
    }
    return false;
}
int Pop(Heap* heap){
	if (heap->Size == 0) { //checks if empty
        fprintf(fout, "pop error : heap is empty\n");
        return -INF;
    }

    int max_elem = heap->Elements[1];
    int last_elem = heap->Elements[heap->Size--];
    int i = 1;
    int child;
    while (2*i <= heap->Size) {
        child = 2*i;
        if (heap->Elements[child+1] > heap->Elements[child] && child < heap->Size ) {
            child++;
        }
        if (last_elem >= heap->Elements[child]) {
            break;
        }
        heap->Elements[i] = heap->Elements[child]; //swap
        i = child;
    }
    heap->Elements[i] = last_elem;
    return max_elem; //returns removed max elem of heap
}
void PrintHeap(Heap* heap){
	if (heap->Size == 0) { //if empty
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    for (int i = 1; i <= heap->Size; i++) { //printing elements
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap = NULL;
    int heapSize, key, max_element;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch (cv) {
        case 'n':
            fscanf(fin, "%d", &heapSize);
            maxHeap = CreateHeap(heapSize);
            break;
        case 'i':
            fscanf(fin, "%d", &key);
            Push(maxHeap, key);
            break;
        case 'd':
            if (maxHeap == NULL || maxHeap->Size == 0) {
                fprintf(fout, "pop error : heap is empty\n");
                break;
            }
            max_element = Pop(maxHeap);
            if (max_element != -INF) {
                fprintf(fout, "max element : %d deleted\n", max_element);
            }
            break;
        case 'p':
            if (maxHeap == NULL || maxHeap->Size == 0) {
                fprintf(fout, "print error : heap is empty\n");
                break;
            }
            PrintHeap(maxHeap);
            break;
        case 'f':
            fscanf(fin, "%d", &key);
            if (maxHeap == NULL || maxHeap->Size == 0) {
                fprintf(fout, "%d is not in the heap\n", key);
                break;
            }
            if (Find(maxHeap, key)) {
                fprintf(fout, "%d is in the heap\n", key);
            } else {
                fprintf(fout, "%d is not in the heap\n", key);
            }
            break;
        }
    }
    DeleteHeap(maxHeap);
    fclose(fin);
    fclose(fout);

    return 0;
}


