#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : "); //printing first line
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			Push(stack,input_str[i]-'0');
		}
		else{
			int b = Pop(stack);
			int a = Pop(stack);
			switch (input_str[i]){
				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;
				/*
					/, % 연산 시 0으로 나누는 경우
						“error : invalid postfix expression, divide by zero”
				*/
				case '/' : 
					if (b == 0) {
						fprintf(fout, "\nerror : invalid postfix expression, divide by zero");
						exit(1);
					}
					else {
						Push(stack, a/b);
					}
					break;
				case '%' : 
					if (b == 0) {
						fprintf(fout, "\nerror : invalid postfix expression, divide by zero");						
						exit(1);

					}
					else {
						Push(stack, a%b);
					} 
					break;
				////////////////////
				default : break;
			}
		}
		fprintf(fout, "%d ", Top(stack));
		i++;
	}
	fprintf(fout, "\n");
	if (stack->top+1 > 1) {  //if the stack is not empty (stack->top+1 represents number of elements in list)
        fprintf(fout, "error : invalid postfix expression, %d elements are left\n", Top(stack));
        exit(1);
    }
	else {
		fprintf(fout, "evaluation result : %d", Top(stack));
	}

	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
	Stack* S = malloc(sizeof(Stack)); //Stack* s
	S->key = (int*)malloc(sizeof(int)*max); //size of int array pointer * max
	S->max_stack_size = 20; //max size of stack = 20
	S->top = -1; //top of empty stack == -1
	return S;

}

void DeleteStack(Stack* S){ 
	free(S->key); //free the key first
	free(S);

	return;
}

void Push(Stack* S, int X){
	if (IsFull(S)== true) { //if the stack is full
		fprintf(fout, "error : invalid postfix expression, stack is full");
		exit(1);
	}
	else {
		S->top +=1;
		S->key[S->top] = X; //the top of s->key
		return;
	}
	
}

int Pop(Stack* S){
	if (IsEmpty(S) == true) { //if stack is empty
		fprintf(fout, "error : invalid postfix expression, stack is empty");
		exit(1); //exiting the program while acknowledging presence of an error
	}
	else {
		return S->key[S->top--]; //return first then subtract top by 1
	}
}

int Top(Stack* S){ //returns the top element 
	return S->key[S->top];
}

bool IsFull(Stack* S){
	return (S->top)+1== S->max_stack_size; //since top is 1 less than max size, add 1 then compare
}

bool IsEmpty(Stack* S){
	return S->top == -1; //top == -1 means empty stack
}