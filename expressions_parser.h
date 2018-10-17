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
    PT_ASSIG,
    PT_ID,
    PT_LEFT_B,
    PT_RIGHT_B,
    PT_END, 
    PT_DELIM, 
    PT_FUN, 
    PT_ERR
};

/**
 * @brief One item of pseudo stack data structure.
 */
//typedef struct stack_item_s {
//    struct stack_item_s *prev; ///< Previous item in stack (nearer to bottom).
//    struct stack_item_s *next; ///< Next item in stack (nearer to top).
//    token_t *token;
//    int mark; ///< Number of mark of this item in stack.
//    bool is_term; ///< True if this item of stack is term, else its nonterm.
//} stack_item_t;
//
///**
// * @brief Pseudo stack representation (mostly double linked list).
// * Used to simulate stack for Precedence Table Parser algorithm.
// */
//typedef struct stack_s {
//    stack_item_t *bot; ///< Bottom of the stack.
//    stack_item_t *top_term; ///< Term highest on the stack.
//    stack_item_t *top; ///< Top of the stack.
//} stack_t;

void destroy_stack_item(stack_item_t *item);
int apply_rule_1(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_2(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_3(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_5(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_6(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
int apply_rule_7(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
/**
 * @brief  Initialize stack, create first item necessary for Precedance Table
 * algorithm.
 * @return Initialized ptr to stack.
 */
//stack_t* init_stack(table_item_t *hash_tb);
///**
// * @brief Destroy stack, free all alocated memory.
// * @param stack Destroyed stack.
// */
//void destroy_stack(stack_t *stack);
///**
// * @brief Set one mark after term witch is on the top of the stack.
// * @param stack Stack where is set mark to top term.
// */
//void mark_stack_term(stack_t *stack);
///**
// * @brief Push new item to stack with value of actual token.
// * @param stack Stack where will be new item pushed.
// * @param token Token (term) witch will be copyied to stack item.
// */
//void stack_push(stack_t *stack, stack_item_t *item);
///**
// * @brief Set stack pointer to top term.
// * @param stack Stack where will be top term setted.
// */
//void set_top_term(stack_t *stack);
///**
// * @brief Apply choosen rule to stack -> transform marked part of stack by this
// * rule.
// * @param stack Stack where will be applyed choosen rule.
// * @pre Stack must have at least one mark.
// */
//void apply_rule(stack_item_t *marked_part);
///**
// * @brief Find marked part of stack.
// * @param stack Stack where we search for marked part.
// * @return Marked part of stack or NULL if not exist any mark in stack.
// */
//stack_item_t* get_marked_part(stack_t *stack);
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
char* prec_table(int top, int token);

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
