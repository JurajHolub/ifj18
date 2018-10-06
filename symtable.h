#ifndef _SYM_TABLE_H_IFJ_18_
#define _SYM_TABLE_H_IFJ_18_

/**
 * @file symtable.h
 * @brief Declaration table of symbols.
 * @date October 2018
 */

#define HASH_SIZE 30

table_item_t hash_table[HASH_SIZE];

typedef struct list_s {
    data_t data;
    struct list_s *next;
} list_t;

typedef struct data_s {
    char *data_type;    ///< Data type of symbol. For function it is return val.
    char *name;         ///< Name of variable or indication of constant.
    char *value;        ///< Value of constant (string, number).
    bool init;          ///< True if variable is initialized;
    char *fun_name;     ///< Name of function.
    int param_cnt;      ///< Number of parameters of function.
    char *param_type;    ///< Data types of function parameters.
} data_t;

typedef struct table_item_s {
    int key;
    list_t *head;
} table_item_t;

int hash_fun(int key);

#endif // _SYM_TABLE_H_IFJ_18_
