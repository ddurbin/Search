#include "search.h"

//Function to determine token index
int HashFunction(char letter){
    int tablePosition;
    if(letter >= 48 && letter <= 57){     //first char of token is 0-9
        tablePosition = letter - 48;
    }
    if(letter >= 97 && letter <= 122){    //first char of token is a-z
        tablePosition = letter - 87;
    }
    return tablePosition;
}

//concatenate character to end of a string
char *Concat(char *string, char letter){    
    char *str = string;
    char let = letter;
    size_t len = strlen(str);
    char *result = (char*) malloc((len + 2) * sizeof(char));
    strcpy(result, str);
    result[len] = let;
    return result;
}

//traverses the tree to find the given tokens files. If the token doesn't exist, returns NULL.
file_node *search_tree(TreeNode *root, char *token){ 
    
    file_node *fileList = (file_node*)malloc(sizeof(file_node));
    TreeNode *ptr = root;
    int i;
    char c;
    int index;
    for(i=0; i<strlen(token); i++){
        c = token[i];
        index = HashFunction(c);
        if(ptr->children[index] == NULL){ //token does not exist
            return NULL;
        }
        ptr = ptr->children[index];
    }
    if(ptr->is_word == true){ //Node is a word, return the linked list of associated files
        fileList = ptr->fileList;
        return fileList;
    }
    return NULL;
}

//Given a list of files which first tokens have occured in and new token file list,increment the freq if there is a matching file
file_node *update_filesSa(file_node *clonedFront, file_node *nodeFront){

    file_node *ptr1, *ptr2 = NULL;
    for(ptr1 = clonedFront; ptr1 != NULL; ptr1 = ptr1->nextFN){ //loop through first token file list

        for(ptr2 = nodeFront; ptr2 != NULL; ptr2 = ptr2->nextFN){ //loop through new token file list

            if(!strcmp(ptr2->file, ptr1->file)){ //matching file, both tokens exist in single file, increment freq.
                ptr1->freq++;
            }
        }
    }
    return clonedFront;
}

//Given the file list of first token, clone it if needed, otherwise append the files for the new token list to cloned list
file_node *update_filesSo(file_node *savedFront, file_node *tokenList){
    file_node *ptr1 = NULL;     //runs through temp LL
    file_node *ptr2 = savedFront;   //runs through saved LL
    file_node *frontHold = NULL;    //saves the front of the saved file list so it can be returned
    file_node *new;
    if(savedFront == NULL){ //clone the list
        for(ptr1 = tokenList; ptr1 != NULL; ptr1 = ptr1->nextFN){
            new = malloc(sizeof(file_node));
            new->file = ptr1->file;
            new->freq = 1;
            new->nextFN = NULL;
            if(ptr2 == NULL){
                ptr2 = new;
                frontHold = ptr2;
            } else {
                ptr2->nextFN = new;
                ptr2 = ptr2->nextFN;
            }
        }
        savedFront = frontHold;
    } else {    //saved list already has files in it
        for(ptr1 = tokenList; ptr1 != NULL; ptr1 = ptr1->nextFN){ //loop through existing cloned file list
            for(ptr2 = savedFront; ptr2 != NULL; ptr2 = ptr2->nextFN){ //loop through new token file list
                if(!strcmp(ptr1->file, ptr2->file)){
                    ptr2->freq++;
                    break;
                }
                if(ptr2->nextFN == NULL){   //file not in list and is added to the end of the saved file LL
                    new = malloc(sizeof(file_node));
                    new->file = ptr1->file;
                    new->freq = 1;
                    new->nextFN = NULL;
                    ptr2->nextFN = new;
                    break;
                }
            }
        }
    }
    return savedFront;
}

//Given the first token file list for the "sa" argument, clone the list for future comparison.
file_node *copy_list(file_node *old){
    file_node *cloneList = NULL;
    file_node *clPtr = NULL;
    cloneList = NULL;
    file_node *ptr;
            
    for(ptr = old; ptr != NULL; ptr = ptr->nextFN){ //loop through file list

        if(clPtr == NULL){ //copying first node
            cloneList = (file_node*)malloc(sizeof(file_node));
            cloneList->file = ptr->file;
            cloneList->freq = 1;
            cloneList->nextFN = NULL;
            clPtr = cloneList;
        // copying remaining nodes
        }else{ 
            file_node *temp = (file_node*)malloc(sizeof(file_node));
            temp->file = ptr->file;
            temp->freq = 1;
            temp->nextFN = NULL;
            clPtr->nextFN = temp;
            clPtr = clPtr->nextFN;
        }
    }
    return cloneList;
}

// loop through and print the "sa" argument files in the list
void print_sa(file_node *front, int count){
    file_node *curr;
    bool boolean = false;
    for(curr = front; curr != NULL; curr = curr->nextFN){ //loop through files list
        if(boolean == false && curr->freq == count){ //first file
            printf("\nFiles containing one or more of the searched terms:\n");
            boolean = true;
        }
        if(boolean == true && curr->freq == count){ //file freq count matches the number of given token arguments, print the file
            printf("%s\n", curr->file);
        }
    }
    printf("\n");
    if(front == NULL || boolean == false){ //no files matched the "sa" criteria
        printf("\nNone of the files contain all of the searched terms.\n\n");
    }
}

//loop through and print the files that have matched the "so" criteria
void print_so(file_node *front){
    file_node *curr;
    for(curr = front; curr != NULL; curr = curr->nextFN){ //loop through the file list
        if(curr == front){ //first node
            printf("\nFiles containing one or more of the searched terms:\n");
        }
        printf("%s\n", curr->file);
    }
    printf("\n");
    if(front == NULL){ //no files matched the search criteria
        printf("\nNone of the listed terms were contained in any file.\n\n");
    }
}

//destroys the cloned file lists
void destroy_savedList(file_node *front){
    file_node *temp;
    file_node *next;
    for(temp = front; temp != NULL; temp = next){ //loop through the list
        next = temp->nextFN;
        temp->file = NULL;
        temp->freq = 0;
        free(temp);
    }
}



