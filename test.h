/**
 * @file test.c
 * @author Matej Parobek(xparob00)
 * @date 2019-10-25
 *
 * @brief Program substituting main function of compiler and scanner interface for testing purposes.
 *
 * Scanner functionality is replaced by reading elaborated lexemes from stdin, some of them involve additional attribute
 */

#ifndef IFJ18_TEST_H
#define IFJ18_TEST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dynamic_string.h"

/**
 * @brief Data type converting supported lexemes to integer, to simplify program
 *
 * for more info see scanner documentation
 */
enum data_type_e {
//  EOF = -1    Scanner make token also from end of file symbol.
    NOT = 0,    ///< Term "not".
    SUB,        ///< Term "-".
    ADD,        ///< Term "+".
    MUL,        ///< Term "*".
    DIV,        ///< Term "/".
    NOT_EQUAL,  ///< Term "!=".
    LESS,       ///< Term "<".
    INTEGER,    ///< Term "integer".
    STRING,     ///< Term "string".
    NIL,        ///< Term "nil".
    LEFT_B,     ///< Term "(".
    RIGHT_B,    ///< Term ")".
    EOL,        ///< Term "\n".
    EQUAL,      ///< Term "==".
    GREATER,    ///< Term ">".
    LESS_EQ,    ///< Term "<=".
    GREATER_EQ, ///< Term ">=".
    FLOAT,      ///< Term "float".
    DEF,        ///< Term "def".
    DO,         ///< Term "do".
    ELSE,       ///< Term "else".
    END,        ///< Term "end".
    IF,         ///< Term "if".
    THEN,       ///< Term "then".
    WHILE,      ///< Term "while".
    ASSIG,      ///< Term "=".
    FUN,        ///< Term "function()".
    DELIM,      ///< Term ",".
    VAR         ///< Term "variable".
};

/**
 * @brief Represents token used by parser(syntax analyser)
 *
 * for more information see scanner documentation
 */
typedef struct token_s {
    int type;
    string_t attribute;
} token_t;

/**
 * @brief Simulation of function get_token() (see scanner documentation)
 *
 * function get lexemes from stdin already elaborated
 * @return Scanned token.
 */
token_t* get_token();

/**
 * @brief Return token from parser to scanner. Returned token will be the next return value of get_token()
 *
 * for more information see scanner documentation
 */
void ret_token(token_t *token);

/**
 * @brief function get and check lexeme and its attribute for function get_token() and save it to token structure
 * @param token already allocated token structure
 **/
void read_token(token_t *token);

/**
 * @brief get and save lexeme attribute for token
 * @return return string with attribute
 */
string_t get_attribute(void);


#endif //IFJ18_TEST_H
