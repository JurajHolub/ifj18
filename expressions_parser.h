/**
 * @file expression_parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of Expression Parser of language IFJ18. Parser use
 * precedence syntax analyse algorithm with precedence table.
 */

#ifndef _EXPRESSIONS_PARSER_H_IFJ_18_
#define _EXPRESSIONS_PARSER_H_IFJ_18_

#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "stack.h"

enum prec_tab_idx_e {
    PT_NOT = 0,
    PT_SUB,
    PT_ADD,
    PT_MUL,
    PT_EQ,
    PT_ID,
    PT_LEFT_B,
    PT_RIGHT_B,
    PT_END, 
    PT_ERR
};

void destroy_stack_item(stack_item_t *item);
int apply_rule_1(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_2(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_3(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_4(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
/**
 * @brief Debug print of stack.
 */
void print_stack(stack_t *stack);


/**
 * @brief Parsing of arithmetic and logic expressions. Called by Recursive
 * Top-down parser after it recognize possible arit./log. expression. Top-down
 * parser call this function with token witch indicate him that it's logic
 * expression. After parsing logic expression is control return to top-down
 * parser.
 * @param First token of arithmetic or logic expression. This token have already
 * Top-down parser, but it recognize arit/logic expression and give control and
 * this token to this parser.
 * @return True if parsed arithmetic or logic expression is correct, else false.
 */
int parse_expression(table_item_t *hash_tb);

/**
 * @brief Precedence Table of arithmetic and logic expression parser.
 * @param top Actual top of stack, it is always value of enum type
 * "expression_terms".
 * @param token Actual input term (token), it is always value of enum type
 * "expression_terms".
 * @return Right value from table for actual combination of top of stack and 
 * input term (token).
 */
char prec_table(int top, int token);

/**
 * @brief Find fitting rule for actual marked part of stack and apply.
 * @param stack Stack for witch marked part is part of expression witch will
 * be transformed by fitting rule.
 * @return True if found fitting rule else false.
 */
int find_rule(table_item_t *hash_tb, stack_t *syntax_stack, stack_t *sem_stack);
int map_index(int idx);
    
stack_item_t* create_stack_item(table_item_t *hash_tb, token_t *token);

/**
 * @brief Debug convert enum type data_type_e to string representation.
 */
char *get_real_type(int type);
char *get_syntax_type(int type);
    
#endif // _EXPRESSIONS_PARSER_H_IFJ_18_
