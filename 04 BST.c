#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
	if (current == NULL) {
		current = malloc(sizeof(BSTNode)); //malloc
		current->value = key; //assign the current value as the key
        current->left = current->right = NULL; //assign the left and right children to be null
	}
	else if (key < current->value) { 
		current->left = insertNode(current->left, key); //since left is smaller than value
	}
	else if(key > current->value) {
		current->right = insertNode(current->right, key); //since right is greater than vallue
	}
	else {
		exit(1); //error
	}
    return current;
}
//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){
	if (current == NULL) {
		return current;
	}
	if (key < current->value) {
		current->left = deleteNode(current->left, key);
	}
	else if (key > current->value) {
		current->right = deleteNode(current->right, key);
	}
	else {
		// Node to be deleted found
		if (current->left == NULL || current->right == NULL) { //if one is null (meaning it has one or no child)
			if (current->left == NULL && current->right == NULL) { //if both are null (meaning 0 child)
				free(current);
				return NULL;
			}
			else { //(1 child)
				if (current->left == NULL) {//if left is null
					BSTNode* child = current->right; //return the child
					free(current);
					return child;
				}
				else if (current->right == NULL) {
					BSTNode* child = current->left;
					free(current);
					return child;
       			}
			}
		}
		//(2 children)
        BSTNode* node = current->right;
        while (node->left != NULL) { //getting the left-most child of the right node
            node = node->left; 
        } //node now has the leftmost value
        current->value = node->value; //saving that value as the current value 
        current->right = deleteNode(current->right, node->value); //delete recursively

		
	}
    return current;
}
BSTNode* findNode(BSTNode* current, int key){
	if (current == NULL) { 
		return current;
	}
	if (key < current->value) { 
		return findNode(current->left, key); //finding through current->left recursively
	}
	else if (key > current->value) {
		return findNode(current->right, key);  //finding through current->right recursively
	}
	else {
		return current;
	}
}
void printInorder(BSTNode* current){
	if (current == NULL) //for possible test case
        return;
    printInorder(current->left); //printing left tree (using recursion)
    fprintf(fout, "%d ", current->value); //printing current node
    printInorder(current->right); //printing right tree (using recursion)
}

void deleteTree(BSTNode* current){
	if (current == NULL) //for possible test case
        return;
	deleteTree(current->right); //using recursion
	deleteTree(current->left);
	free(current); //delete the last node (header)
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}