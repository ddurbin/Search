#include "indexer.h"

typedef enum {false, true} bool;

typedef struct Node{

	char c;
	struct Node *parent;
	struct Node **children;
	bool is_word;
	file_node *fileList;

}Node;

Node *createNode(char c, Node *parent){

	Node *node = malloc(sizeof(Node));
	node->c = c;
	node->parent = parent;
	node->children = malloc(36*sizeof(Node));
	node->is_word = false;
	file_node *list = malloc(sizeof(file_node));
	list = NULL;
	node->fileList = list;

	int i;
	for(i=0; i<36; i++){

		node->children[i] = NULL;
	}
	return node;
}

Node *insertToken(Node *root, char *token, file_node *fileList){

	Node *ptr = root;
	char character;
	int index;
	int i;
	for(i=0; i<strlen(token); i++){
		character = token[i];
		index = HashFunction(character);
		if(ptr->children[index] == NULL){
			ptr->children[index] = createNode(character, ptr);
		}
		ptr = ptr->children[index];
	}
	ptr->is_word = true;
	ptr->fileList = fileList;
	return root;
}

int main(int argc, char *argv[]){
	Node *root = createNode(' ', NULL);
	char *token = malloc(25*sizeof(char));
	char *word = malloc(100*sizeof(char));
	int fileFreq;
	file_node *fileFront, *ptr = NULL;
	ht = HTcreate(argv[1]);
	traverse_Dir(argv[2]);
	HToutput(ht);
	HTdestroy(ht);
	FILE *index = fopen(argv[1], "r");
	while(fscanf(index, "%s", word) == 1){
		
		if(strcmp(word, "<list>") && strcmp(word, "</list>")){
			while(fscanf(index, "%s", token), strcmp(token, "</list>")){
				if(fileFront == NULL){
					fileFront = (file_node*)malloc(sizeof(file_node));
					fileFront->file = token;
					fscanf(index, "%d", &fileFreq);
					fileFront->freq = fileFreq;
					fileFront->nextFN = NULL;
					ptr = fileFront;
				}else{
					file_node *new = (file_node*)malloc(sizeof(file_node));
					new->file = token;
					fscanf(index, "%d", &fileFreq);
					new->freq = fileFreq;
					new->nextFN = NULL;
					ptr->nextFN = new;
					ptr = new;
				}
			}
		root = insertToken(root, word, fileFront);
		fileFront = NULL;
		}
	}
	return 0;
}
