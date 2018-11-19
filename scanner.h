#ifndef _SCANER_H_IFJ_18_
#define _SCANER_H_IFJ_18_

#include "dynamic_string.h"

/**
 * @file scanner.h
 * @brief Declaration of scanner interface for parser.
 * @date October 2018
 */

/**
 * @brief Data types of input symbols from IFJ18 language. Never change
 * possition of enum items please (possition is important for parser), if you
 * want add more symbols add it at the end of enum.
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
    VAR,         ///< Term "variable".
    BOOL,
    UNDEF,
    CONST,
    DEF_FUN,
    UNDEF_FUN
};

enum states{
    
    START_state=0,
    ID_state,
    INT_state,
    DOUBLE_state,
    EXPONENT_state,
    EXPONENT_sign_state,
    COMMENT_state,
    COMMENT_BEGIN_state,
    COMMENT_END_state,
    COMMENT_END_ENTER_state,
    COMMENT_END_ENTER2_state,
    CONSTANT_state,
    FUNCTION_state,
    SIGN_state,
    N_state,
    STRING_state,
    ESCAPE_state,
    HEX_ESCAPE_state,
    HEX_ESCAPE2_state

};

/**
 * @brief Describe one terminal (symbol) of input IFJ18 language. Created by
 * scanner (lexical analyse) and used in parser (syntax/semantic analyse).
 */
typedef struct token_s {
    int type;   ///< Type of parsed symbol. Always value of enum type "data_type_e".
    /**
     * Value of parsed symbol.
     * NULL if type is one of *,/,-,+,(,) ...
     * String representation of parsed value if type is integer, float or string.
     * Name of variable if type is VAR (variable).
     */
    string_t attribute; 
} token_t;

/**
 * @brief Scan one term from input and return as a token. Always and only
 *  called by parser.
 *  @return Scanned token.
 */
token_t* get_token();

/**
 * @brief Return token from parser to scanner. After that if parser call
 * get_token(), it will return this token (same as return at last time
 * of call get_token()). Always and only called by parser.
 * Parser can return 2 tokens in row so scanner must save always last 2 tokens.
 * @param token Token witch will be returned to scanner and will be first witch
 * parser recieve after nearest call of get_token().
 */
void ret_token(token_t *token);


/**
 * @brief Deallocate all memory used in scanner.
 */
void free_scanner();


void tokenLexOutput();


#endif // _SCANER_H_IFJ_18_
