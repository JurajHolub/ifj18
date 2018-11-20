/**
 * @author Matej Parobek (xparob00)
 * @file top_down_parser.c
 * @date 2018-10-16
 * @brief Implementation of top-down parser
 */

#ifndef IFJ18_TOP_DOWN_PARSER_H
#define IFJ18_TOP_DOWN_PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expressions_parser.h"
#include "scanner.h"
#include "symtable.h"

#define PARAMS_SIZE 15

typedef struct params_s
{
    int array_size;
    data_t *params;
} *params_t;

/**
 * @brief Run syntax analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int parse(void);

/**
 * @brief Nonterminal symbol program_list and it's syntactic rules simulation
 *
 * Check's syntax of the main program body and creates global symbol table
 *
 * <program_list> -> EOF
 * <program_list> -> <function_def> <program_list> EOF
 * <program_list> -> <statement> <program_list> EOF
 *
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int program_list(void);


/**
 * @brief Nonterminal symbol function_def and it's syntactic rules simulation
 *
 * <function_def> -> def ID(<params>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * Function creates entry for function in symbol table and creates local symbol table for definitions in function body.
 * Function calls semantic analysis and generates prologue and epilogue of function.
 *
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int function_def(void);

/**
 * @brief Nonterminal symbol params and it's syntactic rules simulation
 *
 * <params> -> ) EOL <function_body>
 * <params> -> id <param_list>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * @param symtable_data entry for actual function in global symbol table used for inserting function's parameters
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int params(data_t *ste_newfc, string_t str_params);

/**
 * @brief Nonterminal symbol param_list and it's syntactic rules simulation
 *
 * <param_list> -> ) EOL <function_body>
 * <param_list> -> , id <param_list>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * @param symtable_data allocated entry for actual function in global symbol table used for inserting function's parameters
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int param_list(data_t *ste_newfc, string_t str_params);

/**
 * @brief Nonterminal symbol function_body and it's syntactic rules simulation
 *
 * <function-body> -> <stat> <fun_body>
 * <function-body> -> end EOL
 * @param local_symtable symbol table used by definitions in function body, it's not symbol table used in main body of program
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int function_body();


/**
 * @brief Nonterminal symbol statement checks no syntax, only insure complicated saving of parameters in symbol table
 *
 * function find out, what statement have to be on input and call rule to derive it
 *
 * <statement> -> <if_statement>
 * <statement> -> <while_statement>
 * <statement> -> <assignment> EOL
 *
 * @param symtable symbol table used by definitions in statement
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int statement(table_item_t *symtable, bool main_body_st, bool force_undef);

/**
 * @brief Nonterminal symbol if_statement and it's syntactic rules simulation
 *
 * <if_statement> -> if <expression> then EOL <if_body>
 *
 * nonterminal symbols <if_statement>, <if_body>, <else_body> are set of nonterminal symbols and it's rules,
 * that check syntax of if statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int if_statement(table_item_t *symtable, bool main_body_if);

/**
 * @brief Nonterminal symbol if_body and it's syntactic rules simulation
 *
 * <if_body> -> <stat> <if_body>
 * <if_body> -> else EOL <else-body>
 *
 * nonterminal symbols <if_statement>, <if_body>, <else_body> are set of nonterminal symbols and it's rules,
 * that check syntax of if statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int if_body(table_item_t *symtable, bool main_body_if, data_t *else_label);

/**
 * @brief Nonterminal symbol else_body and it's syntactic rules simulation
 *
 * <else_body> -> <stat> <else_body>
 * <else_body> -> end EOL
 *
 * nonterminal symbols <if_statement>, <if_body>, <else_body> are set of nonterminal symbols and it's rules,
 * that check syntax of if statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int else_body(table_item_t *symtable, bool main_body_else);

/**
 * @brief Nonterminal symbol while_statement and it's syntactic rules simulation
 *
 * <while_statement> -> while <expression> do EOL <while_body>
 *
 * nonterminal symbols <while_statement>, <while_body> are set of nonterminal symbols and it's rules,
 * that check syntax of while statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int while_statement(table_item_t *symtable, bool main_body_while);

/**
 * @brief Nonterminal symbol while_body and it's syntactic rules simulation
 *
 * <while_body> -> <stat> <while_body>
 * <while_body> -> end EOL
 *
 * nonterminal symbols <while_statement>, <while_body> are set of nonterminal symbols and it's rules,
 * that check syntax of while statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int while_body(table_item_t *symtable, bool main_body_while);

/**
 * @brief Nonterminal symbol assignment checks syntax, of assignment statement and calls semantic analysis in case
 * there is an assignment to L-value.
 *
 * <assignment> -> ID = <function_call>
 *        --if after first ID in line is = and then ID saved in symbol table as data_type function
 * <assignment> -> ID = <expression>
 *        --if after first ID in line is = and the first rule is not called
 * <assignment> -> <function_call>
 *        --if after first ID in line is no = and ID is in symbol table saved as function
 * <assignment> -> <expression>
 *        --in other cases
 *
 * @param symtable symbol table used by definitions in statement
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int assignment(table_item_t *symtable, bool main_body_assig, bool force_undef);

/**
 * @brief Nonterminal symbol function_call and it's syntactic rules simulation
 *
 * <function_call> -> ID <call_params> EOL
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @param symtable to check correct function call according to it's definition in semantic analysis
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int function_call(table_item_t *symtable, bool accept_undef);

/**
 * @brief Nonterminal symbol call_params and it's syntactic rules simulation
 *
 * <call_params> -> (<call_param>)
 * <call_params> -> <call_param>
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int call_params(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params);

/**
 * @brief Nonterminal symbol call_param and it's syntactic rules simulation
 *
 * <call_param> -> ID <call_param_list>
 * <call_param> -> ɛ
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int call_param(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params);

/**
 * @brief Nonterminal symbol call_param_list and it's syntactic rules simulation
 *
 * <call_param_list> -> , ID <call_param_list>
 * <call_param_list> -> ɛ
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return 0 if parsed program is correct, else integer code according to error type
 */
int call_param_list(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params);

#endif //IFJ18_TOP_DOWN_PARSER_H
