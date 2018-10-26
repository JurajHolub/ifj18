/**
 * @file stack.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Stack used in sentactic and semantic analyses.
 */

#ifndef _STACK_H_IFJ_18_
#define _STACK_H_IFJ_18_

#include "scanner.h"
#include "symtable.h"
#include <stdbool.h>

typedef struct syntax_s {
    token_t *token;
    int mark; ///< Number of mark of this item in stack.
    bool is_term; ///< True if this item of stack is term, else its nonterm.
} syntax_t;

typedef struct semantic_s {
    string_t value;
} semantic_t;
    
typedef struct stack_item_s {
    struct stack_item_s *prev; ///< Previous item in stack (nearer to bottom).
    struct stack_item_s *next; ///< Next item in stack (nearer to top).
    void *data;
} stack_item_t;

typedef struct stack_s {
    stack_item_t *bot; ///< Bottom of the stack.
    stack_item_t *top; ///< Top of the stack.
} stack_t;

/************************ GENERAL OPERATION WITH STACK ***********************/

void stack_init(stack_t *stack);
void stack_push(stack_t *stack, void *item);
void stack_pop(stack_t *stack);
void* stack_top(stack_t *stack); 
bool stack_empty(stack_t *stack);

/****************************** SYNTAX STACK *********************************/

stack_item_t* get_marked_part(stack_t *stack);
stack_item_t* get_top_term(stack_t *stack);
void mark_stack_term(stack_t *stack);
void free_mark(stack_t *stack);
syntax_t* alloc_syntax_item(token_t *token, table_item_t *hash_tb);
void free_syntax_stack(stack_t *stack);
void free_syntax_item(syntax_t *item);
stack_t* init_syntax_stack();

/****************************** SEMANTIC STACK *******************************/

stack_t* init_sem_stack();
void free_sem_stack(stack_t *stack);

#endif // _STACK_H_IFJ_18_
