#include "search.h"

// constructor function creates tree and returns root node
TreeNode *create_treenode(char c, TreeNode *parent){
    TreeNode *node = malloc(sizeof(TreeNode));
    node->c = c;
    node->parent = parent;
    node->children = malloc(36*sizeof(TreeNode));
    node->is_word = false;
    node->fileList = NULL;
    return node;
}

//Recursively frees memory for the entire tree structure and each node's associated members
void destroy_tree(TreeNode *node){
    if(node == NULL){
        return;
    }
    int i;
    for(i=0; i<36; i++){
        
        if(node->children[i] != NULL){ //Node children array exists
            
            if(node->fileList != NULL){ //Node fileList exists

                destroy_savedList(node->children[i]->fileList); //free file list at current node
            }
            
        }
        destroy_tree(node->children[i]); //recursive call
    }
    free(node->children); //free children array
    free(node); //free node
    return;
}

//Function builds the prefix tree from the inverted index file
TreeNode *build_tree(FILE *index){

    char *string = "";
    char c;
    c = getc(index);
    while(c != EOF){ //store file in a string
        string = Concat(string, c);
        c = getc(index);
    }
    char *token;
    char *hold = NULL;
    char *seps = " \n";
    TreeNode *root = create_treenode(' ', NULL);
    file_node *fileFront = NULL;
    file_node *ptr = NULL;
    char *word;
    token = strtok_r(string, seps, &hold);
    while (token){
        if(strcmp(token, "<list>") && strcmp(token, "</list>")){ // token is a word 
            word = token;
            while(token = strtok_r(NULL, seps, &hold), strcmp(token, "</list>")){ // retrieve all the associated files for the token
                if(fileFront == NULL){ //file list is empty
                    fileFront = malloc(sizeof(file_node));
                    fileFront->file = token;
                    token = strtok_r(NULL, seps, &hold);
                    fileFront->freq = 0;
                    ptr = fileFront;
                } else { //file list NOT empty append the file to the list
                    file_node *new = malloc(sizeof(file_node));
                    new->file = token;
                    token = strtok_r(NULL, seps, &hold);
                    fileFront->freq = 0;
                    ptr->nextFN = new;
                    ptr = ptr->nextFN;
                }
            }
            root = add_token(root, word, fileFront); //add the created node and members to the tree
            fileFront = NULL;
        } else { //get next token from the file
            token = strtok_r(NULL, seps, &hold);
        }
    }
    return root;
}

//Function maps the given token to the tree along with its associated file list, returns the root to the tree
TreeNode *add_token(TreeNode *root, char *token, file_node *fileFront){
    TreeNode *ptr = root;
    int character;
    int converted;
    size_t size = strlen(token);
    int i;
    for(i = 0; i < size; i++){ // loop through token characters
        character = token[i];
        converted = HashFunction(character); // get the index
        if(ptr->children[converted] == NULL){
            ptr->children[converted] = create_treenode(character, ptr); //create Node if one doesn't already exist
        }
        ptr = ptr->children[converted]; //move pointer down the tree
    }
    ptr->is_word = true; //identify the node as a word in the tree
    ptr->fileList = fileFront;
    return root;
}
