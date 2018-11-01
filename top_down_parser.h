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
#include "test.h"
#include "symtable.h"

/**
 * @brief Run syntax analysis, initialize and clean data used by analysis
 * @return 0 correct syntax, 2 incorrect syntax
 */
int parse(void);

/**
 * @brief Nonterminal symbol program_list and it's syntactic rules simulation
 *
 * Check's syntax of the main program body
 *
 * <program_list> -> EOF
 * <program_list> -> <function_def> <program_list>
 * <program_list> -> <statement> <program_list>
 *
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool program_list(void);


/**
 * @brief Nonterminal symbol function_def and it's syntactic rules simulation
 *
 * <function_def> -> def ID(<params>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <param>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * Important: this function allocates entry for function in symbol table and insert type function and it's name here
 * This function allocates local symbol table for definitions in function body, too The table entry and local symbol table
 * are given to other functions checking function definition syntax by parameter for next use
 *
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool function_def(void);

/**
 * @brief Nonterminal symbol params and it's syntactic rules simulation
 *
 * <params> -> ) EOL <function_body>
 * <params> -> <param> <param_list>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <param>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * @param symtable_data allocated entry for actual function in global symbol table used for inserting function's parameters
 * @param local_symtable this pointer function only gives to next function for use in function function_body()
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool params(table_item_t *local_symtable, data_t *symtable_data);

/**
 * @brief Nonterminal symbol param_list and it's syntactic rules simulation
 *
 * <param_list> -> ) EOL <function_body>
 * <param_list> -> , <param> <param_list>
 *
 * nonterminal symbols <function_def>, <params>, <params_list>, <param>, <fun-body> are set of nonterminal symbols and
 * it's rules, that check syntactic correct function definition
 *
 * @param symtable_data allocated entry for actual function in global symbol table used for inserting function's parameters
 * @param local_symtable this pointer function only gives to next function for use in function function_body()
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool param_list(table_item_t *local_symtable, data_t *symtable_data);

/**
 * @brief Nonterminal symbol param checks no syntax, only insure saving of parameters in symbol table and incrementing
 * parameters counter
 *
 * @param symtable_data allocated entry for actual function in global symbol table used for inserting function's parameters
 * @param param inserted function parameter
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool param(data_t *symtable_data, string_t param);

/**
 * @brief Nonterminal symbol function_body and it's syntactic rules simulation
 *
 * <function-body> -> <stat> <fun_body>
 * <function-body> -> end EOL
 * @param local_symtable symbol table used by definitions in function body, it's not symbol table used in main body of program
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool function_body(table_item_t *local_symtable);


/**
 * @brief Nonterminal symbol statement checks no syntax, only insure complicated saving of parameters in symbol table
 *
 * function find out, what statement have to be on input and call rule to derive it
 *
 * <statement> -> <if_statement>
 * <statement> -> <while_statement>
 * <statement> -> <function_call>
 * <statement> -> <expression>
 *
 * @param symtable symbol table used by definitions in statement
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool statement(table_item_t *symtable);

/**
 * @brief Nonterminal symbol if_statement and it's syntactic rules simulation
 *
 * <if_statement> -> if <expression> then EOL <if_body>
 *
 * nonterminal symbols <if_statement>, <if_body>, <else_body> are set of nonterminal symbols and it's rules,
 * that check syntax of if statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool if_statement(table_item_t *symtable);

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
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool if_body(table_item_t *symtable);

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
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool else_body(table_item_t *symtable);

/**
 * @brief Nonterminal symbol while_statement and it's syntactic rules simulation
 *
 * <while_statement> -> while <expression> do EOL <while_body>
 *
 * nonterminal symbols <while_statement>, <while_body> are set of nonterminal symbols and it's rules,
 * that check syntax of while statement
 *
 * @param symtable symtable symbol table used by definitions in statement and by semantic analysis
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool while_statement(table_item_t *symtable);

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
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool while_body(table_item_t *symtable);


/**
 * @brief Nonterminal symbol function_call and it's syntactic rules simulation
 *
 * <function_call> -> ID <call_params> EOL
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @param symtable to check correct function call according to it's definition in semantic analysis
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool function_call(table_item_t *symtable);

/**
 * @brief Nonterminal symbol call_params and it's syntactic rules simulation
 *
 * <call_params> -> (<call_param>)
 * <call_params> -> <call_param>
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool call_params(void);

/**
 * @brief Nonterminal symbol call_param and it's syntactic rules simulation
 *
 * <call_param> -> ID <call_param_list>
 * <call_param> -> ɛ
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool call_param(void);

/**
 * @brief Nonterminal symbol call_param_list and it's syntactic rules simulation
 *
 * <call_param_list> -> , ID <call_param_list>
 * <call_param_list> -> ɛ
 *
 * nonterminal symbols <function_call>, <call_params>, <call_params_list>, <call_param> are set of nonterminal symbols
 * and it's rules, that check syntactic correct function call
 *
 * @return true if syntax of program follow this rule derived rules. Simple syntax is correct, else false.
 */
bool call_param_list(void);

#endif //IFJ18_TOP_DOWN_PARSER_H
