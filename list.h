/**
 * @file list.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Definition of list used in generating of output code.
 */

#ifndef _LIST_H_IFJ_18_
#define _LIST_H_IFJ_18_

#include "gen_out.h"

typedef struct gen_item_s {
    inst_t *inst;
    struct gen_item_s *next;
} gen_item_t;

typedef struct gen_list_s {
    gen_item_t *first;
    gen_item_t *last;
} gen_list_t;

gen_list_t* gen_list_create();
void gen_list_insert_first(gen_list_t *list, inst_t *inst);
void gen_list_insert_last(gen_list_t *list, inst_t *inst);
void gen_list_append(gen_list_t *dst, gen_list_t *appended);
void gen_list_delete_first(gen_list_t *list);
bool gen_list_empty(gen_list_t *list);
void gen_list_free(gen_list_t *list);

#endif // _LIST_H_IFJ_18_
