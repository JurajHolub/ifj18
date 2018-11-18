/**
 * @file symtable.h
 * @brief Declaration symbol table implemented by hash table.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#ifndef _SYM_TABLE_H_IFJ_18_
#define _SYM_TABLE_H_IFJ_18_

#include "dynamic_string.h"

#define HASH_SIZE 53

/******************************************************************************
***************************** INTERFACE ***************************************
******************************************************************************/

/**
 * @brief Data about one symbol saved in symbol table (implemented by hash table).
 * Symbols saved to Symbol table are variables and functions (not constants).
 */
typedef struct data_s {
    /**
     * Data type of symbol. Could be {VAR, CONST} in symtable of some frame
     * or {DEF_FUN, UNDEF_FUN} in symtable of functions.
     */
    int type;  
    /** 
     * True type of variable or function. It could be {INTEGER, FLOAT, STRING,
     * NIL, BOOL}. If it is symtable of frame it is true type of variable (if 
     * is is CONST than it is not defined) else if it is symtable of function
     * then it is return value of function.
     */
    int value;  
    string_t id;       
    /**
     * Name of identificator for variable, function or constant (if it is
     * constant then id is value of constant it self).
     */
    int param_cnt;  
    /**
     *  Number of function parameters, if it is symtab of frame then it is undefined.
     */
} data_t;

/**
 * @brief Linked list used for saving more items hashed to same index of hash table.
 */
typedef struct list_s {
    data_t *data;        ///< Data about symbol saved in this item of list.
    struct list_s *next;///< Next item of list.
} list_t;

/**
 * @brief One item of Hash Table. Hash Table is static array of pointers to
 * list of items hashed to this index.
 */
typedef struct table_item_s {
    list_t *head;       ///< List of symbols hashed to same key.
} table_item_t;

/**
 * @brief Create array of table_item_t of size HASH_SIZE and init.
 * @return Allocated and initialized hash_table.
 */
table_item_t* get_hash_table();

/**
 * @brief Destroy array of table_item_t and free all allocated memory.
 */
void destroy_hash_table(table_item_t* hash_table);

/**
 * @brief By hash function search and return item in symbol table witch fit to
 * search key.
 */
data_t* search(table_item_t *table, string_t key);
/**
 * @brief By hash function found ideal place in hash table and insert there
 * new item of symbol table.
 */
void insert(table_item_t *table, data_t *data);

/**
 * @brief Initialize data from input parameter to temporary variable with
 * unicate id for hash table.
 * @param data Data witch will be initailized.
 * @param type Type of temporary symbol initialized in data.
 */
string_t insert_tmp(table_item_t *table, int type);

/******************************************************************************
***************************** INSIDE LOGIC ************************************
******************************************************************************/

/**
 * @brief Hash function. Hash input key to index of hash table. Implementation
 * of djdj2 hash function algorithm.
 * @see http://www.cse.yorku.ca/~oz/hash.html
 * @param key Key for hash table is "data_t.id" because it is unicate for each
 * symbol.
 * @return Index to hash table array.
 */
unsigned long hash_fun(string_t key);

data_t* list_search(list_t *list, string_t key);
list_t* list_insert_first(data_t *data);
list_t* list_insert(list_t *list, data_t *data); 
void print_table(table_item_t* table);
data_t* data_copy(data_t *src);
//char* cpy_string(char *src);
void list_destroy_first(list_t **list);
void data_destroy(data_t *data);

#endif // _SYM_TABLE_H_IFJ_18_
