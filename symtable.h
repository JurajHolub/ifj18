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
};

typedef struct data_s {
    ...
    ...
} data_t;

typedef struct table_item_s {
    int key;
    list_t *head;
} table_item_t;

int hash_fun(int key);

#endif // _SYM_TABLE_H_IFJ_18_
