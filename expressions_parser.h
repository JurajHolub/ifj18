#ifndef _EXPRESSIONS_PARSER_H_IFJ_18_
#define _EXPRESSIONS_PARSER_H_IFJ_18_

/**
 * @file expression:parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of Expression Parser of language IFJ18. Parser use
 * precedence syntax analyse algorithm with precedence table.
 */

#include <stdio.h>
#include "stack.h"
#include "scanner.h"
#include <stdbool.h>

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
bool parse_expression(token_t *token);

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
bool find_rule(stack_t *stack);
    
    
#endif // _EXPRESSIONS_PARSER_H_IFJ_18_
