#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

//구현하면 좋은 함수
//getHeight, max, updateHeight, rebalance
int max(int a, int b) {
    if (a>b) {
        return a; //return max
    }
    else {
        return b;
    }
}

int getHeight(AVLNode *node) {
    if (node == NULL)
        return -1; //empty
    else
        return node->height; //return height
}

void updateHeight(AVLNode *node) {
     if (node != NULL) {
        int leftHeight = getHeight(node->left); //get height of left recursively
        int rightHeight = getHeight(node->right); //get height of right recursively
        node->height = 1 + max(leftHeight, rightHeight); 
    }
}


AVLNode* RotateRight(AVLNode* node){
    AVLNode *L = node->left;
    node->left = L->right; //reassign elements
    L->right = node;
    updateHeight(node); //update heights
    updateHeight(L);
    return L;
}

AVLNode* RotateLeft(AVLNode* node){
    AVLNode *R = node->right;
    node->right = R->left; //reassign elements
    R->left = node;
    updateHeight(node); //update heights
    updateHeight(R);
    return R;

}

AVLNode* rebalance(AVLNode* T){
    if (getHeight(T->left) > getHeight(T->right) + 1) { //compare left > right+1 heights
        if (getHeight(T->left->right) > getHeight(T->left->left)) //if LR case
            T->left = RotateLeft(T->left);
        T = RotateRight(T);
    } else if (getHeight(T->left) +1 < getHeight(T->right)) {  //compare left+1 < right heights
        if (getHeight(T->right->left) > getHeight(T->right->right)) //if RL case
            T->right = RotateRight(T->right);
        T = RotateLeft(T);
    }
    return T;

}


AVLNode* Insert(Element X, AVLNode* T){
    if (T == NULL) { //if null
        T = (AVLNode *)malloc(sizeof(AVLNode)); //make node
        if (T == NULL) { //malloc failure catch
            exit(1);
        }
        T->element = X; //set elements to new tree
        T->left = T->right = NULL;
        T->height = 0;
    } 
    else if (X < T->element) { //if item is bigger than element
        T->left = Insert(X, T->left); //go left
        updateHeight(T);
        T = rebalance(T);
    } 
    else if (X > T->element) {
        T->right = Insert(X, T->right); //go right
        updateHeight(T);
        T = rebalance(T); 
    } 
    else { //if item added is the same as element
        fprintf(fout,"insertion error : %d is already in the tree\n", X);
    }
    return T;

}

AVLNode* Delete(Element X, AVLNode* T){
    if (T == NULL) {
        fprintf(fout,"deletion error : %d is not in the tree\n", X); //
        return 0;
    }
    if (X < T->element) { //if less than element
        T->left = Delete(X, T->left); 
    } 
    else if (X > T->element) { //if bigger than element
        T->right = Delete(X, T->right);
    } 
    else { 
        if (T->left == NULL && T->right == NULL) { //if both are null
            free(T);
            return NULL;
        } 
        else if (T->left == NULL) { //if only left is null
            AVLNode *temp = T;
            T = T->right;
            free(temp);
        } 
        else if (T->right == NULL) { //if only right is null
            AVLNode *temp = T;
            T = T->left;
            free(temp);
        } 
        else {
            AVLNode *temp = T->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            T->element = temp->element;
            T->right = Delete(temp->element, T->right);
        }
    }
    updateHeight(T);
    T = rebalance(T);

    return T;
}

void PrintInorder(AVLNode* T){
    if (T == NULL) {//for possible test case
        return;
    }
    PrintInorder(T->left); //printing left tree (using recursion)
    fprintf(fout, "%d(%d) ", T->element, T->height+1); //printing current node
    PrintInorder(T->right); //printing right tree (using recursion)
}

void DeleteTree(AVLNode* T){
    if (T == NULL) {
        return;
    }
    DeleteTree(T->left); //delete left recursively
    DeleteTree(T->right); //delete right recursively
    free(T); //free
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}
