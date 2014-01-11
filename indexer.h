#ifndef indexer_indexer_h
#define indexer_indexer_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>

//struct for the tokenizer provided
struct TokenizerT_ {
        char* copied_string;
        char* current_position;
};
typedef struct TokenizerT_ TokenizerT;

//struct for the hash table that is an array of token_nodes
struct hash_table{
    struct token_node *table;
};
typedef struct hash_table hash_table;

/* node structure that holds a token and has a pointer to the
 * next token_node and to the files that token exists in
 */
struct token_node{
    char *token;
    struct file_node *FNptr;
    struct token_node *nextTN;
};
typedef struct token_node token_node;

/* node structure to hold the file path name a token exists in and its
 * frequency in that file along with a pointer to the next file_node
 */
struct file_node{
    const char *file;
    int freq;
    struct file_node *nextFN;
};
typedef struct file_node file_node;

//globabl variables for the hash table, tokenizer, and output file
hash_table *ht;
TokenizerT *tokenizer;
FILE *invertIndex;
FILE *fp;


//**********tokenizer function declarations**********
char *TKGetNextToken(TokenizerT *tk);
char is_delimiter(char character);
void TKDestroy(TokenizerT *tk);
TokenizerT *TKCreate(char *string);
char* unescape_string(char* string);
int is_oct_digit(char oct_digit);
int char_to_oct(char character);
int char_to_hex(char character);
char is_escape_character(char character);


//**********hash table function declarations**********

/* HTcreate takes the given output file name as a parameter and
 * initalizes a new hash table and creates the output file
 */
hash_table *HTcreate(const char *outputFile);

/* HTdestroy frees the memory allocated for the hash table structure, the linked lists
 * associated with it, the tokenizer, and closes the output file
 */
void HTdestroy(hash_table *ht);

/* HTinsert takes a hash table, a token, and a file path name as parameters
 * and inserts the token into the hash table and adds the file or updates the
 * files frequency if that file is already in the list of files
 */
void HTinsert(hash_table *ht, char *token, const char *file);

/* HToutput takes a hash table as a parameter and outputs the information in that
 * hash table to the given output file
 */
void HToutput(hash_table *ht);

/* HashFunction is just a very basic hash function that takes in a char which is the
 * first letter of a token and maps it to the hash table
 */
int HashFunction(char letter);


//**********traverse directory functions**********

/* Traverse_Dir takes a directory name as a parameter and recursivley traverses through
 * the directory and its sub-directories
 */
int traverse_Dir(char *dirName);

//**********other function declarations**********

// string comparison function from the sorted list assignment
int compareStrings(char *p1, char *p2);

/* Concat takes a string and a character as parameters and adds the character
 * to the end of the string and returns the resulting string
 */
char *Concat(char *string, char letter);

/* ConcatString takes two strings as parameters and combines the two
 * strings and returns the resulting string
 */
char *ConcatString(char *string1, char *string2);

/* hash_tokens takes a file path name as a parameter and tokenizes the file and
 * sends the tokens to HTinsert to be inserted into the hash table
 */
int hash_Tokens(const char *fileName);

#endif
