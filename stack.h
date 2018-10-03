#ifndef _STACK_H_IFJ18_
#define _STACK_H_IFJ18_

/**
 * @file stack.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of pseudo stack data structure with necessary functions
 * above it.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scanner.h"
#include <stdbool.h>

/**
 * @brief One item of pseudo stack data structure.
 */
typedef struct stack_item_s {
    struct stack_item_s *prev; ///< Previous item in stack (nearer to bottom).
    struct stack_item_s *next; ///< Next item in stack (nearer to top).
    /**
     * Index to Precedence Table. Always set to value of enum type
     * expression_terms. 
     */
    int tab_idx; 
    int mark; ///< Number of mark of this item in stack.
    char *key; ///< Key value of scanned term, for every type of token there is one.
    bool is_term; ///< True if this item of stack is term, else its nonterm.
} stack_item_t;

/**
 * @brief Pseudo stack representation (mostly double linked list).
 * Used to simulate stack for Precedence Table Parser algorithm.
 */
typedef struct stack_s {
    stack_item_t *bot; ///< Bottom of the stack.
    stack_item_t *top_term; ///< Term highest on the stack.
    stack_item_t *top; ///< Top of the stack.
} stack_t;

/**
 * @brief  Initialize stack, create first item necessary for Precedance Table
 * algorithm.
 * @return Initialized ptr to stack.
 */
stack_t* init_stack();
/**
 * @brief Destroy stack, free all alocated memory.
 * @param stack Destroyed stack.
 */
void destroy_stack(stack_t *stack);
/**
 * @brief Concatente marked part of stack to one string.
 * @param str1 First concatenated string, it is start of new string.
 * @param str2 Second concatenated string, it is end of new string.
 * @post Expect call free() at the end of using new allocated string.
 * @return Dynamicly created concatenated string.
 */
char* get_concat_stack(stack_t *stack);
/**
 * @brief Set one mark after term witch is on the top of the stack.
 * @param stack Stack where is set mark to top term.
 */
void mark_stack_term(stack_t *stack);
/**
 * @brief Push new item to stack with value of actual token.
 * @param stack Stack where will be new item pushed.
 * @param token Token (term) witch will be copyied to stack item.
 */
void stack_push(stack_t *stack, token_t *token);
/**
 * @brief Set stack pointer to top term.
 * @param stack Stack where will be top term setted.
 */
void set_top_term(stack_t *stack);
/**
 * @brief Apply choosen rule to stack -> transform marked part of stack by this
 * rule.
 * @param stack Stack where will be applyed choosen rule.
 * @pre Stack must have at least one mark.
 */
void apply_rule(char *rule, stack_t *stack);
/**
 * @brief Find marked part of stack.
 * @param stack Stack where we search for marked part.
 * @return Marked part of stack or NULL if not exist any mark in stack.
 */
stack_item_t* get_marked_part(stack_t *stack);
/**
 * @brief Debug print of stack.
 */
void print_stack(stack_t *stack);

#endif // _STACK_H_IFJ18_
