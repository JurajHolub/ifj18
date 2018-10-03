#ifndef _SCANER_H_IFJ_18_
#define _SCANER_H_IFJ_18_

/**
 * @file scanner.h
 * @brief Declaration of scanner interface for parser.
 * @date October 2018
 * @note You can create your token as you need but I need here declared part
 * of it for parser (this values in structure). Juraj
 */

/**
 * Possible values of token key.
 */
#define INT_K "int"
#define FLOAT_K "float"
#define STRING "string"
#define NIL_K "nil"
#define END_K "$"
#define LEFTB_K "("
#define RIGHTB_K ")"
#define LESS_K "<"
#define GREATER_K ">"
#define LEQ_K "<="
#define REQ_K ">="
#define EQ_K "=="
#define NEQ_K "!="
#define ADD_K "+"
#define SUB_K "-"
#define MUL_K "*"
#define DIV_K "/"


/**
 * @brief Every value is index to Precedence Table for actual scanned token.
 */
enum expression_terms {
    _mul = 0, ///< Index to Precedence Table for multipication term "*".
    _div, ///< Index to Precedence Table for divitation term "/".
    _add,///< Index to Precedence Table for addition term "+".
    _sub,///< Index to Precedence Table for substract term "-".
    /**
     * Index to Precedence Table for comparation (less or greater)
     * term "<,>,<=,>=".
     */
    _less,
    _eq,///< Index to Precedence Table for comparation (equal) term "==, !=".
    _integer,///< Index to Precedence Table for integer term "30, intVar".
    _float,///< Index to Precedence Table for float term "30.3, floatVar".
    _string,///< Index to Precedence Table for string term (constant or variable).
    _nil,///< Index to Precedence Table for nil data type term.
    /**
     * Index to Precedence Table for possible end of arit. expression term.
     * "EOL, then, do"
     */
    _end,
    _leftB,///< Index to Precedence Table for left bracket term "(".
    _rightB///< Index to Precedence Table for right bracket term ")".
};

/**
 * @brief Describe one terminal of input src file in IFJ18 language. Created by
 * scanner (lexical analyse) and used in parser (syntax/semantic analyse).
 */
typedef struct token_s {
    /**
     * Index to Precedence Table (used in parser of aritmetic and logic
     *  expressions). Always set to value of enum type expression_terms.
     */
   int tab_idx; 
   /**
    * Key value of scanned term, for every type of token there is one.
    */
   char *key;
} token_t;

/**
 * @brief Scan one term from input and return as a token. Always and only
 *  called by parser.
 *  @return Scanned token.
 */
token_t* get_token();

#endif // _SCANER_H_IFJ_18_
