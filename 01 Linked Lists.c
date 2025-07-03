#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key; //data of node 
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);

void Insert(LE* header, int inserted_key, int target_key){ 
	LE* inserted_node = (LE*)malloc(sizeof(struct LinkedListElement)); // malloc
	if (inserted_node == NULL) { //checking for malloc error
		//error
		return;
	} 
	if (Find(header, inserted_key) != NULL) { //returns a node	//check if the key already exists in the list
		fprintf(fout, "insertion %d failed : the key already exists\n", inserted_key);
		free(inserted_node);
		return;
	}

	LE* target_key_node = NULL; 
	if (target_key == -1) { //if target_key = -1, targetkeynode = header
        target_key_node = header;
    }
	else {
        target_key_node = Find(header, target_key); //finds the node of the target key
    }

	if (target_key_node != NULL) {
		inserted_node->key = inserted_key;
		inserted_node->next_pointer = target_key_node->next_pointer;
		target_key_node->next_pointer = inserted_node;
	}
	else  { //if not in list
		fprintf(fout, "insertion %d failed : can not find location\n", inserted_key);
		free(inserted_node);
	}  
}

void Delete(LE* header, int target_key){ 
	if (FindPrevious(header, target_key) != NULL) {
		LE* prev_target_node = FindPrevious(header, target_key); //previous node
		LE* target_node = prev_target_node->next_pointer; //find target_key's node using nextpointer
		prev_target_node->next_pointer = target_node->next_pointer;
		free(target_node);
	}
	else if (FindPrevious(header, target_key) == NULL) { //if not in list
		fprintf(fout, "deletion %d failed : node is not in the list\n", target_key);
        return;
	}
    
}

LE* Find(LE* header, int target_key){ // returns null if not in list, returns current node if in list
	LE* current = header; // 포인터 current를 header로 설정
	while(current!=NULL) { //while current is not null and its key is not target key
		if (current->key == target_key) { //if target_key found
			return current; 
		}
		current = current->next_pointer; //move to next pointer
	}
	
    return NULL;
}

LE* FindPrevious(LE* header, int target_key){
	LE* current = header; // 포인터 current를 header로 설정
	while(current->next_pointer!=NULL) { //while current->nextpointer is not null and its key is not target key
		if (current->next_pointer->key == target_key) {  // current 의 next_pointer의 key 가 target_key 면 반환 (if c->n->k is targetkey then return c)
			return current;
		}
		else {
			current = current->next_pointer; //if not found,move to next pointer
		}
	}

    return NULL;
}

void PrintPrevious(LE* header, int target_key){ 
	if (FindPrevious(header, target_key) == NULL) { //if not in list
		fprintf(fout, "finding %d failed : node is not in the list\n", target_key);
	}
	else { 
		if (FindPrevious(header, target_key)->key == header->key) { //if previous is header
			fprintf(fout, "previous node of %d is head\n", target_key);
		}
		else { //if previous is not -1
			fprintf(fout, "previous node of %d is %d\n", target_key, FindPrevious(header, target_key)->key);
		}
	}
}

void PrintList(LE* header){ //final node == NULL 
	LE* temp = header->next_pointer; //let temp = header 
	if (temp == NULL) { //if list is empty
		fprintf(fout, "empty list\n");
	}
	else { //if list is not empty
		while(temp != NULL) { //while temp is not empty, print temp
			fprintf(fout, "%d ", temp->key);
			temp = temp->next_pointer; //update temp to next element (2nd element)
		}
		fprintf(fout, "\n");
	}
}

LE* CreateList(){
	LE* new_list = (LE*)malloc(sizeof(struct LinkedListElement));
	new_list->next_pointer = NULL; //let its next pointer be NULL
    new_list->key = 0; //let element = 0
	return new_list;
}

void DeleteList(LE* header){ // ex) 0 -> 1 -> 2 -I 
    LE* current = header->next_pointer; // current =1
    while (current != NULL) {
        LE* temp = current; //temp = 1
        current = current->next_pointer; //current = 2
        free(temp); //free 1
    }
    free(header); //free 0
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

    LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return;
}