/**
 * @file semantic_parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of Semantic Analyse Parser of language IFJ18.
 */

#ifndef _SEMANTIC_PARSER_H_IFJ_18_
#define _SEMANTIC_PARSER_H_IFJ_18_

#include "symtable.h"
#include "scanner.h"
#include "stack.h"

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, token_t *arit);
int parse_assig(table_item_t *sym_tb, stack_t* sem_stack, token_t *new_var);
int parse_operand(table_item_t *sym_tb, stack_t* sem_stack, token_t *op);
int parse_add(table_item_t *sym_tab, stack_t *sem_stack);
int parse_sub(table_item_t *sym_tab, stack_t *sem_stack);
int parse_mul(table_item_t *sym_tab, stack_t *sem_stack);
int parse_div(table_item_t *sym_tab, stack_t *sem_stack);

#endif // _SEMANTIC_PARSER_H_IFJ_18_
