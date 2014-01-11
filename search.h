#ifndef search_search_h
#define search_search_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>

//true false boolean
typedef enum {false, true} bool;

/* Tree node is an array of length 36, each poistion representing 0-9
 * or a-z. Each position also has a pointer to its child tree node.
 */
struct TreeNode{
    char c;
    struct TreeNode *parent;
    struct TreeNode **children;
    bool is_word;
    struct file_node *fileList;
};
typedef struct TreeNode TreeNode;

/* node structure to hold the file path name a token exists in and its
 * frequency in that file along with a pointer to the next file_node
 */
struct file_node{
    const char *file;
    int freq;
    struct file_node *nextFN;
};
typedef struct file_node file_node;


/* Concat takes a string and a character as parameters and adds the character
 * to the end of the string and returns the resulting string
 */
char *Concat(char *string, char letter);

/* HashFunction is a function that takes in a char from a token and returns
 * an int so that char can be mapped in the prefix tree
 */
int HashFunction(char letter);


//**********prefix tree functions**********

//Recursively frees memory for the entire tree structure and each node's associated members
void destroy_tree(TreeNode *node);

// constructor function creates tree and returns root node
TreeNode *create_treenode(char c, TreeNode *parent);

//Function maps the given token to the tree along with its associated file list, returns the root to the tree
TreeNode *add_token(TreeNode *root,char *token, file_node *fileFront);

//Function builds the prefix tree from the inverted index file
TreeNode *build_tree(FILE *index);

//traverses the tree to find the given tokens files. If the token doesn't exist, returns NULL.
file_node *search_tree(TreeNode *root, char *token);

//Given a list of files which first tokens have occured in and new token file list,increment the freq if there is a matching file
file_node *update_filesSa(file_node *clonedFront, file_node *nodeFront);

//Given the file list of first token, clone it if needed, otherwise append the files for the new token list to cloned list
file_node *update_filesSo(file_node *savedFront, file_node *tokenList);

//Given the first token file list for the "sa" argument, clone the list for future comparison.
file_node *copy_list(file_node *old);

//loop through and print the files that have matched the "so" criteria
void print_so(file_node *front);

// loop through and print the "sa" argument files in the list
void print_sa(file_node *front, int count);

//destroys the cloned file lists
void destroy_savedList(file_node *front);

#endif
