/**
 * @file stack.c
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Stack used in sentactic and semantic analyses.
 */

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include "error_handle.h"
#include <string.h>
#include "expressions_parser.h"

/************************ GENERAL OPERATION WITH STACK ***********************/

void stack_init(stack_t *stack)
{
    stack->bot = NULL;
    stack->top = NULL;
}

void stack_push(stack_t *stack, void *data)
{
    stack_item_t* new = malloc(sizeof(stack_item_t));
    if (new == NULL)
    {
        free(new);
        mem_error();
    }

    new->data = data;
    new->prev = stack->top;
    new->next = NULL;
    if (stack->top != NULL)
        stack->top->next = new;
    else
        stack->bot = new;
    stack->top = new;
}

void stack_pop(stack_t *stack)
{
    stack_item_t *deleted = stack->top;

    if (stack->bot == stack->top)
    {
        stack->bot = NULL;
        stack->top = NULL;
    }
    else
    {
        stack->top = stack->top->prev;
        stack->top->next = NULL;
    }

    free(deleted);
}

void* stack_top(stack_t *stack)
{
    return stack->top->data;
}

bool stack_empty(stack_t *stack)
{
    return stack->top == NULL;
}

/****************************** SYNTAX STACK *********************************/

// this 3 break rules of stack data type
stack_item_t* get_marked_part(stack_t *stack)
{
    syntax_t *syntax;
    stack_item_t *mark;

    for (mark = stack->top; mark != NULL; mark = mark->prev)
    {
        syntax = mark->data;
        if (syntax->mark > 0)
        {
            return mark->next;
        }
    }


    return NULL;
}

stack_item_t* get_top_term(stack_t *stack)
{
    syntax_t *syntax;
    for (stack_item_t *i = stack->top; i != NULL; i = i->prev)
    {
        syntax = i->data;
        if (syntax->is_term)
            return i;
    }

    return NULL;
}

void free_mark(stack_t *stack)
{
    stack_item_t *mark = get_marked_part(stack);
    syntax_t* mark_syntax = mark->data;
    syntax_t* prev_syntax = mark->prev->data;
    mark_syntax->is_term = false;
    prev_syntax->mark -= 1;

    syntax_t *top = stack_top(stack);

    if (mark_syntax != top)
    {
        while (mark_syntax != top)
        {
            free_syntax_item(top);
            stack_pop(stack);
            top = stack_top(stack);
        }
    }
}

void mark_stack_term(stack_t *stack)
{
    stack_item_t *top_term = get_top_term(stack);

    if (top_term != NULL)
    {
        syntax_t *syntax = top_term->data;
        syntax->mark += 1;
    }
}

void free_syntax_stack(stack_t *stack)
{
    syntax_t *i;

    while (!stack_empty(stack))
    {
        i = stack_top(stack);
        stack_pop(stack);
        free_syntax_item(i);
    }

    free(stack);
}

syntax_t* alloc_syntax_item(token_t *token, table_item_t *hash_tb)
{
    token_t* copy_of_token = malloc(sizeof(token_t));
    syntax_t *new_syntax = malloc(sizeof(syntax_t));
    if (copy_of_token == NULL || new_syntax == NULL)
    {
        free(copy_of_token);
        free(new_syntax);
        mem_error();
        return NULL;
    }
    copy_of_token->attribute = string_create(token->attribute->string);

    data_t *sym = search(hash_tb, token->attribute);
    if (sym != NULL)
        copy_of_token->type = sym->data_type;
    else
        copy_of_token->type = token->type;

    new_syntax->token = copy_of_token;
    new_syntax->mark = 0;
    new_syntax->is_term = true;
    
    return new_syntax;
    
}

void free_syntax_item(syntax_t *item)
{
    string_free(item->token->attribute);
    free(item->token);
    free(item);
}

stack_t* init_syntax_stack()
{
    token_t t;
    t.type=EOL;
    t.attribute= string_create(NULL);

    stack_t* new = malloc(sizeof(stack_t));
    if (new == NULL)
    {
        free(new);
        mem_error();
        return NULL;
    }

    stack_init(new);
    syntax_t *syntax = alloc_syntax_item(&t, NULL);
    stack_push(new, syntax);

    string_free(t.attribute);

    return new;
}

void print_stack(stack_t *stack)
{
    stack_item_t *bot = stack->bot;
    while (bot != NULL)
    {
        syntax_t *act = bot->data;
        fprintf(stderr,"%s", get_real_type(act->token->type));
        for (int i = 0; i < act->mark; i++)
            fprintf(stderr,"%c", '<');
        bot = bot->next;
    }

    fprintf(stderr,"\"\n");
}

/****************************** SEMANTIC STACK *******************************/

void print_sem_stack(stack_t *sem_stack)
{
    fprintf(stderr,"\"");
    stack_item_t *bot = sem_stack->bot;
    while (bot != NULL)
    {
        char *symbol = bot->data;
        fprintf(stderr,"%s, ", symbol);
        bot = bot->next;
    }

    fprintf(stderr,"\"\n");
}

stack_t* init_sem_stack()
{
    stack_t* new = malloc(sizeof(stack_t));
    if (new == NULL)
    {
        free(new);
        mem_error();
        return NULL;
    }

    stack_init(new);

    return new;
}

void free_sem_stack(stack_t *stack)
{
    stack_item_t *i;

    while (!stack_empty(stack))
    {
        i = stack_top(stack);
        stack_pop(stack);
        string_free(i->data);
    }

    free(stack);
}
