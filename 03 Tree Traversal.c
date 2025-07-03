#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);


void DeleteTree(TREE* current){
    if (current == NULL) //checking for NULL inputs (possible test case)
        return;
    DeleteTree(current->left_pointer); //using recursion
    DeleteTree(current->right_pointer);
    free(current); //delete the last node (header)
}
void printPreorder(TREE* current){
    if (current == NULL)
        return;
    fprintf(fout, "%d ", current->data); //printing current node's data
    printPreorder(current->left_pointer); //printing left tree (using recursion)
    printPreorder(current->right_pointer); //printing right tree (using recursion)
}
void printInorder(TREE* current){
    if (current == NULL)
        return;
    printInorder(current->left_pointer); //printing left tree (using recursion)
    fprintf(fout, "%d ", current->data); //printing current node
    printInorder(current->right_pointer); //printing right tree (using recursion)
}
void printPostorder(TREE* current){
    if (current == NULL)
        return;
    printPostorder(current->left_pointer); //printing left tree (using recursion)
    printPostorder(current->right_pointer); //printing right tree (using recursino)
    fprintf(fout, "%d ", current->data);; //printing current node
}  
void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){  //current, index, the data array, number of nodes //CBT
    //현재 트리 노드 데이터터 설정
    current->data = inserted_data_array[current_index]; //assigning the current->data as the first number in the array
    //
    if (current_index < length && inserted_data_array[current_index] != -1) { //condition to keep the code recursing till it reaches the length of array (and to make sure its not empty)
        if((current_index*2) < length && inserted_data_array[current_index*2] != -1) { //condition for left node to be possible (if the index*2 does not exceed length && it exists in list)
            current->left_pointer = malloc(sizeof(TREE)); //malloc
            CreateFromArray(current->left_pointer, current_index*2, inserted_data_array, length); //index*2 for left node
        }
        else {
            current->left_pointer = NULL; //if end
        }
        if((current_index*2+1) < length && inserted_data_array[current_index*2+1] != -1) { //condition for right node to be possible (if the index*2+1 does not exceed length && it exists in list)
            current->right_pointer = malloc(sizeof(TREE)); //malloc
            CreateFromArray(current->right_pointer, current_index*2+1, inserted_data_array, length); //index*2+1 for right node
        }
        else {
            current->right_pointer = NULL;
        }
    }
}


void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
	    fscanf(fin, "%d", &input_data);
	    array[i + 1] = input_data;
	}
	TREE* header = malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1); //current, starts from index 1 (for recursion)
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
	DeleteTree(header);
	fclose(fin);
	fclose(fout);
	return;
}
