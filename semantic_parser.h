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

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, int arit_op);
int parse_logic_op(table_item_t *sym_tb, stack_t* sem_stack, int logic_op);
int parse_assig(table_item_t *sym_tb, stack_t* sem_stack, token_t *new_var);
int parse_operand(table_item_t *sym_tb, stack_t* sem_stack, token_t *op);
int convert_type(stack_t* sem_stack, table_item_t *sym_tb, data_t* symb1, data_t* symb2);
int sem_action_assig(table_item_t *vars_symtable, data_t *l_value_entry);
int sem_action_fcdef(data_t *ste_ptr_newfc, string_t str_params, data_t **params_array);
    
#endif // _SEMANTIC_PARSER_H_IFJ_18_
