/**
 * @file expression_parser.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
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

/**
 * @brief Enum type for precedence table, constant maps tokens to precedence
 * table syntax tokens.
 */
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

/**
 * @brief Check if there is identificator necessary by syntax rules.
 * @param hash_tb Symbol table for actual frame of program used in semantic analyse.
 * @param sem_stack Stack used in semantical analyse of this part of code.
 * @param marked Marked token by precedence table saved in stack.
 * @return Result of syntax analyse if it's failed else it return result of
 * semantic analyse of this syntactical part of code.
 */
int syntax_parse_id(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
/**
 * @brief Check if brackets in expresion has correct syntax and reduce them.
 * @param hash_tb Symbol table for actual frame of program used in semantic analyse.
 * @param sem_stack Stack used in semantical analyse of this part of code.
 * @param marked Marked token by precedence table saved in stack.
 * @return Result of syntax analyse if it's failed else it return result of
 * semantic analyse of this syntactical part of code.
 */
int syntax_parse_brackets(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
/**
 * @brief Check if aritmetical expression has correct syntax.
 * @param hash_tb Symbol table for actual frame of program used in semantic analyse.
 * @param sem_stack Stack used in semantical analyse of this part of code.
 * @param marked Marked token by precedence table saved in stack.
 * @return Result of syntax analyse if it's failed else it return result of
 * semantic analyse of this syntactical part of code.
 */
int syntax_parse_arit(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
/**
 * @brief Check if logical expression has correct syntax.
 * @param hash_tb Symbol table for actual frame of program used in semantic analyse.
 * @param sem_stack Stack used in semantical analyse of this part of code.
 * @param marked Marked token by precedence table saved in stack.
 * @return Result of syntax analyse if it's failed else it return result of
 * semantic analyse of this syntactical part of code.
 */
int syntax_parse_logic(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked);
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
/**
 * @brief Convert token type to expression special enum for precedence table analyse.
 * @param idx Value of data_type_e enum.
 * @return Converted enum to precedece table enum.
 */
int map_index(int idx);
/**
 * @brief Debug convert enum type data_type_e to string representation.
 */
char *get_real_type(int type);
/**
 * @brief Debug convert enum type syntax_type_e to string representation.
 */
char *get_syntax_type(int type);
    
#endif // _EXPRESSIONS_PARSER_H_IFJ_18_
