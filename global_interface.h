/**
 * @file global_interface.h
 * @brief Global interface of symtables used for parser and generator.
 * @date November 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#ifndef _GLOBAL_IFACE_H_IFJ_18_
#define _GLOBAL_IFACE_H_IFJ_18_

#include "symtable.h"

/*****************************************************************************/
/******************************* GETTERS *************************************/
/*****************************************************************************/

/********************** GLOBAL SYMBOL TABLE FOR MAIN BODY ********************/
/**
 * @return Symbol table for main body of program. Variables in main body,
 * but not functions!
 */
table_item_t* get_main_st();

/******************* GLOBAL SYMBOL TABLE FOR ALL FUNCTIONS *******************/
/**
 * @return Symbol table of all functions (also library functions).
 */
table_item_t* get_fun_st();

/***************** GLOBAL SYMBOL TABLE FOR ACTUAL LOCAL FRAME ****************/
/**
 * @brief Symbol table of last created local frame. Local variables but not
 * deinition function.
 */
table_item_t* get_local_st();
/**
 * @return Id to actual symbol table where is saved result of expression.
 * It could be VAR, VAR_INTIGER, ... ,NIL, BOOL(for if, while).
 */
string_t get_expr_type();

/*****************************************************************************/
/******************************* SETTERS *************************************/
/*****************************************************************************/
/**
 * @brief Set symbol table for local frame (body inside function but not 
 * function it self).
 */
void set_local_st();
/**
 * @brief Set actual id to actual symbol table where is saved return value of
 * expression.
 * @param Id to symbol table.
 */
void set_expr_type(string_t expr);
/**
 * @brief Remove all symbol tables.
 */
void remove_all_st();

#endif // _GLOBAL_IFACE_H_IFJ_18_
