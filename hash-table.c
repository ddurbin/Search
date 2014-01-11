#include "indexer.h"

//create hashtable.
hash_table *HTcreate(const char *outputFile){   
    hash_table *ht = (hash_table*)malloc(sizeof(hash_table)); 
    ht->table = malloc(36 * sizeof(token_node));    //creates the hash table array
    int i;
    for(i = 0; i < 36; i++){    //initialize the hash table
        ht->table[i].nextTN = NULL;
        ht->table[i].FNptr = NULL;
        ht->table[i].token = NULL;
    }
    invertIndex = fopen(outputFile, "w");   //open inverted index for output
    return ht;
}

//free hash table and tokenizer and close output file
void HTdestroy(hash_table *ht){
    int i;
    token_node *temp, *next;
    file_node *temp2, *next2;
    for(i = 0; i < 36; i++){    //loop through array
        for(temp = ht->table[i].nextTN; temp != NULL; temp = next){ //loop through token linked list
            next = temp->nextTN;
            temp->token = NULL;
            for(temp2 = temp->FNptr; temp2 != NULL; temp2 = next2){ //loop through file linked list
                next2 = temp2->nextFN;
                temp2->file = NULL;
                temp2->file = 0;
                free(temp2);
            }
            free(temp);
        }
    }
    free(ht);
    if(tokenizer != NULL){
        TKDestroy(tokenizer);
    }   
    fclose(invertIndex);
}

//inserts a token into the hashtable
void HTinsert(hash_table *ht, char *token, const char *file){
    int first = token[0];
    file_node *new2 = malloc(sizeof(file_node));
    new2->file = file;
    new2->nextFN = NULL;
    new2->freq = 1;
    int tablePosition = HashFunction(first);    //hash function to determine position in array
    if(ht->table[tablePosition].token == NULL){     //front of linked list is empty
        ht->table[tablePosition].token = token;
        ht->table[tablePosition].FNptr = new2;
    } else {    //linked list not empty
        token_node *new = malloc(sizeof(token_node));
        new->token = token;
        new->nextTN = NULL;
        new->FNptr = new2;
        token_node *curr = &ht->table[tablePosition];
        token_node *prev = NULL;
        int compare;
        int duplicate = 0;
        int dupCompare;
        while(curr != NULL){    //determine if a duplicate token exists
            dupCompare = compareStrings(new->token, curr->token);
            if(dupCompare == 0){
                duplicate = 1;
            }
            prev = curr;
            curr = curr->nextTN;
        }
        curr = &ht->table[tablePosition];
        prev = NULL;
        while(curr != NULL){    //find where token goes in token linked list
            compare = compareStrings(new->token, curr->token);
            if(compare < 0 && !duplicate){    //token should go in front of the token its being compared to
                if(prev == NULL){   //new token needs to be at front of linked list
                    if(ht->table[tablePosition].nextTN != NULL){    //a node already exists will be placed front of list
                        curr->nextTN = ht->table[tablePosition].nextTN;
                        ht->table[tablePosition].nextTN = new;
                        new->token = ht->table[tablePosition].token;
                        new->FNptr = ht->table[tablePosition].FNptr;
                        ht->table[tablePosition].token = token;
                        ht->table[tablePosition].FNptr = new2;
                    } else {    //only one token in the list
                        ht->table[tablePosition].nextTN = new;
                        new->token = ht->table[tablePosition].token;
                        new->FNptr = ht->table[tablePosition].FNptr;
                        ht->table[tablePosition].token = token;
                        ht->table[tablePosition].FNptr = new2;
                    }
                } else {   //insert token in list somewhere in the list (not front) 
                    new->nextTN = prev->nextTN;
                    prev->nextTN = new;
                }
                return;
            } else if(compare > 0 && !duplicate){
                if(curr->nextTN == NULL){   //token should go at the end of the linked list
                    curr->nextTN = new;
                    return;
                }
                
            } else if(compare == 0){    //increase file frequency count or add new file node
                file_node *currFile, *prevFile;
                currFile = curr->FNptr;
                prevFile = NULL;
                int compareFile;
                while(currFile != NULL){ //find if there exists the same file
                    compareFile = compareStrings((char*)file, (char*)currFile->file);
                    if(compareFile == 0){   //word and file already exists increment frequency count
                        currFile->freq++;
                        //sort if necassary
                        if(prevFile != NULL){   //file
                            if(prevFile->freq > currFile->freq){    //files still in correct order
                                return;
                            }
                            else {  //files out of order
                                prevFile->nextFN = currFile->nextFN;    //take file_node out of list to re-sort
                                file_node *tempCurr, *tempPrev;
                                tempCurr = curr->FNptr;
                                tempPrev = NULL;
                                while(tempCurr != NULL){
                                    if(tempCurr->freq < currFile->freq){
                                        if(tempPrev == NULL){
                                            curr->FNptr = currFile;
                                            currFile->nextFN = tempCurr;
                                            return;
                                        } else {
                                            tempPrev->nextFN = currFile;
                                            currFile->nextFN = tempCurr;
                                            return;
                                        }
                                    }
                                    tempPrev = tempCurr;
                                    tempCurr = tempCurr->nextFN;
                                }
                            }
                        } else {    //file incremented was first in list
                            return;
                        }
                    }
                    prevFile = currFile;
                    currFile = currFile->nextFN;
                }
                //first occurence of this word in this file
                file_node *newFile = (file_node*)malloc(sizeof(file_node));
                newFile->file = file;
                newFile->freq = 1;
                newFile->nextFN = NULL;
                prevFile->nextFN = newFile;
                return;
                
            }
            prev = curr;
            curr = curr->nextTN;
        }
    }
}

//determine which index in hashtable token is mapped to
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

//compare two strings
int compareStrings(char *p1, char *p2){
    return strcmp(p1, p2);
}

//function opens current file and hashes tokens to the hash table
int hash_Tokens(const char *fileName){
    if(fileName == NULL){   //file doens't exist
        printf("***Filename does not exist***\n");
        return 1;
    }
    int fileCheck;
    char c;
    char *string = (char*)malloc(sizeof(char));
    string = "";
    fileCheck = access(fileName, F_OK);
    if(fileCheck == 0){ //open file 
        fp = fopen(fileName, "r");
        c = getc(fp);
        while(c != EOF){    //create string to send to tokenizer
            c = tolower(c);
            string = Concat(string, c);
            c = getc(fp);
        }
    }
    tokenizer = TKCreate(string);
    char* token = TKGetNextToken(tokenizer);
    while(token != NULL ){ //parse and insert tokens into hash table
        HTinsert(ht, token, fileName);
        token = TKGetNextToken(tokenizer);
    }
    free(token);
    fclose(fp);
    return 0;
}

//output the entire hashtable to the output file
void HToutput(hash_table *ht){
    int i;
    token_node *ptr;
    file_node *ptr2;
    for(i = 0; i < 36; i++){    //get each token in hashtable
        ptr = &ht->table[i];
        ptr2 = ht->table[i].FNptr;
        while(ptr != NULL){ 
            if(ptr->token != NULL){
                fprintf(invertIndex, "<list> %s\n\n", ptr->token);  //write opening brace and token
            }
            while(ptr2 != NULL){
                if(ptr2->nextFN == NULL){
                    fprintf(invertIndex, "%s %d\n\n</list>\n\n", ptr2->file, ptr2->freq);   //last file node to be written, write closing braces
                    break;
                } else {
                    fprintf(invertIndex, "%s %d ", ptr2->file, ptr2->freq); //write the file and frequency
                    ptr2 = ptr2->nextFN;
                }
            }
            ptr = ptr->nextTN;
            if(ptr != NULL){
                ptr2 = ptr->FNptr;
            }
        }
    }
}

char *Concat(char *string, char letter){    //concatenate character to end of a string
    char *str = string;
    char let = letter;
    size_t len = strlen(str);
    char *result = (char*) malloc((len + 2) * sizeof(char));
    strcpy(result, str);
    result[len] = let;
    return result;
}

char *ConcatString(char *string1, char *string2){   //concatenate 2 strings together
    size_t len = strlen(string2);
    int i;
    for(i = 0; i<len; i++){
        string1 = Concat(string1, string2[i]);
    }
    return string1;
}
