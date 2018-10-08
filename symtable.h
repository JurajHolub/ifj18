#ifndef _SYM_TABLE_H_IFJ_18_
#define _SYM_TABLE_H_IFJ_18_

/**
 * @file symtable.h
 * @brief Declaration symbol table implemented by hash table.
 * @date October 2018
 */

#define HASH_SIZE 30

/**
 * @brief Data about one symbol saved in symbol table (implemented by hash table).
 * Symbols saved to Symbol table are variables and functions (not constants).
 */
typedef struct data_s {
    int data_type;  ///< Data type of symbol. Always value of enum type "data_type_e".
    char *id;       ///< Name of identificator for variable.
    char *value;    ///< Value of variable, it is assigned when variable was defined.
    /** 
     * Return data type of function if "data_type" is "FUN". Always value of
     * enum type "data_type_e".
     */
    int fun_type;
    int param_cnt;  ///< Number of function parameters.
    char *param_id; ///< Name of function parameters identificators, delimered by space.
} data_t;

/**
 * @brief Linked list used for saving more items hashed to same index of hash table.
 */
typedef struct list_s {
    data_t data;        ///< Data about symbol saved in this item of list.
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
void free_hash_table(table_item_t* hash_table);

/**
 * @brief Hash function. Hash input key to index of hash table.
 * @param key Key for hash table is "data_t.id" because it is unicate for each
 * symbol.
 * @return Index to hash table array.
 */
int hash_fun(char *key);

/**
 * @brief By hash function search and return item in symbol table witch fit to
 * search key.
 */
data_t* search(char *key);
/**
 * @brief By hash function found ideal place in hash table and insert there
 * new item of symbol table.
 */
void insert(data_t *data);

#endif // _SYM_TABLE_H_IFJ_18_
