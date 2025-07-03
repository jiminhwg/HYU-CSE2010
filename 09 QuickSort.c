#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    Array* array = (Array*)malloc(sizeof(Array)); //malloc
    array->size = size;
    array->values = (int*)malloc(size*sizeof(int));
    return array;
}

void QuickSort(Array* array, int left, int right, int depth){
    if (left < right) { //condition
        int pivotIndex = Partition(array, left, right, depth); //partition
        QuickSort(array, left, pivotIndex-1, depth+1); //left
        QuickSort(array, pivotIndex+1, right, depth+1); //right
    }
}

int Partition(Array* array, int left, int right, int depth){ //j is left; i is right
    int pivot = array->values[right]; //pivot is last elem
    int l = left; 
    int r = right - 1; //r is one elem in front of last elem

    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right); //prints out starting array
    while (l <= r){
        if (array->values[l] > pivot && array->values[r] < pivot) { //prints when elements are swapped bcuz of being < or > than pivot
            swap(&array->values[l], &array->values[r]);
            PrintArray(array, left, right);
        }
        if (array->values[l] <= pivot) {
            l++; //moving l 
        }
        if (array->values[r] >= pivot) {
            r--; //moving r
        }
    }

    if (l != right) { //prints only if swap is needed
        swap(&array->values[l], &array->values[right]); //swap pivot to be in middle
        PrintArray(array, left, right); // print after swap
    }
    return l;

}

void PrintArray(Array* array, int left, int right){
    for(int i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-"); //print -
        }
        else{
            fprintf(fout, "%-3d", array->values[i]); //print
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values); //delete
    free(array);
}

void swap(int* a, int* b) {
   int temp = *a; //temp swap
    *a = *b;
    *b = temp;
}
