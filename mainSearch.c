#include "search.h"


int main(int argc, char *argv[]){

    char *token, *hold, *seps;
    file_node *nodeList = (file_node*)malloc(sizeof(file_node));
    nodeList = NULL;
    hold = NULL;
    seps = " \n";
    char input[1000];
    FILE *index = fopen(argv[1], "r");
    if(index == NULL){
        fprintf(stderr, "ERROR: Index file doesn't exist\n");
        return 1;
    }
    TreeNode *root = build_tree(index);
    int count; //determines number of token arguments to search for
    fputs("Welcome to Search!!\nPlease select an option:\n\t\"sa <terms>\", for a list of files with all the listed terms.\n\t\"so <terms>\", for a list of files with one or more of the listed terms.\n\t\"q\" to quit.\n", stdout);
    fgets(input, sizeof(input), stdin);
    while(strcmp(input, "q\n")){ // Ask for input until user wants to quit
        
        file_node *savedFront = NULL;
        token = strtok_r(input, seps, &hold); //get first argument "sa" or "so"
        if(token != NULL && !strcmp(token, "sa")){ //argument is "sa"
            count = 0;
            token = strtok_r(NULL, seps, &hold); //get token
            count++;
            if(token != NULL){
                nodeList = search_tree(root, token); //get file list for token
            }
            if(nodeList != NULL){ //if token exists get it's file list
                savedFront = copy_list(nodeList);
            }
            token = strtok_r(NULL, seps, &hold); //get next token
            while(token){ //get tokens
                nodeList = search_tree(root, token); //get file list for token
                if(nodeList != NULL){ //if token exists get it's file list
                    
                    savedFront = update_filesSa(savedFront, nodeList); //determine files which coincide with last word(s)
                }
                count++;
            token = strtok_r(NULL, seps, &hold); //get next token
            }
            print_sa(savedFront, count); //output the final list of files
        
        }else if(token != NULL && !strcmp(token, "so")){ //argument is "so"
            
            file_node *savedFront = NULL;
            token = strtok_r(NULL, seps, &hold);
            while(token){ //get tokens
                nodeList = search_tree(root, token); // get file list for token
                if(nodeList != NULL){ //if token exists get it's file list
                    
                    savedFront= update_filesSo(savedFront, nodeList); // combine files from new token and old token
                }
                token = strtok_r(NULL, seps, &hold); //get next token
            }
            print_so(savedFront); //output final list of files

        }else{ //error improper user arguments
            fprintf(stderr, "ERROR: Incorrect arguments\n");
            return 1;
        }
    fputs("Welcome to Search!!\nPlease select an option:\n\t\"sa <terms>\", for a list of files with all the listed terms.\n\t\"so <terms>\", for a list of files with one or more of the listed terms.\n\t\"q\" to quit.\n", stdout);
    fgets(input, sizeof(input), stdin);
    destroy_savedList(savedFront); //free file lists for next round       
    }
    printf("Quitting program......\n");
    destroy_tree(root); //free the tree
    return 0;
}

