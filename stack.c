/**
 * @file stack.c
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Definition of pseudo stack data structure with necessary functions
 * above it.
 */

#include "stack.h"

void print_stack(stack_t *stack)
{
    printf("stack: \"");
    stack_item_t *bot = stack->bot;
    while (bot != NULL)
    {
        printf("%s", bot->key);
        for (int i = 0; i < bot->mark; i++)
            printf("%c", '<');
        bot = bot->next;
    }

    printf("\"\n");
}

stack_item_t* get_marked_part(stack_t *stack)
{
    for (stack_item_t *i = stack->top; i != NULL; i = i->prev)
        if (i->mark > 0)
        {
            if (i->next != NULL)
            return i;
        }

    return NULL;
}

void set_top_term(stack_t *stack)
{
    for (stack_item_t *i = stack->top; i != NULL; i = i->prev)
    {
        if (i->is_term)
        {
            stack->top_term = i;
            return;
        }
    }

    stack->top_term = stack->bot;
}

void apply_rule(char *rule, stack_t *stack)
{
    stack_item_t *marked_part = get_marked_part(stack);
    if (marked_part == NULL)
        return; // should not happend

    marked_part->next->key = rule;
    marked_part->next->is_term = false;
    marked_part->mark -= 1;

    stack_item_t *i = marked_part->next;

    stack->top = i;

    i = i->next;
    while (i != NULL)
    {
        stack_item_t *deleted = i;

        if (i->next != NULL)
            i->next->prev = i->prev;
        if (i->prev != NULL)
            i->prev->next = i->next;

        i = i->next;
        free(deleted);
    }
}

stack_t* init_stack()
{
    stack_t* new = (stack_t*)malloc(sizeof(stack_t));
    stack_item_t* first = (stack_item_t*)malloc(sizeof(stack_item_t));
    if (new == NULL && first == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return NULL;
    }

    first->prev = NULL;
    first->next = NULL;
    first->tab_idx = _end;
    first->key = "";
    first->mark = 0;
    first->is_term = true;

    new->bot = first;
    new->top_term = first;
    new->top = first;

    return new;
}

void destroy_stack(stack_t *stack)
{
    stack_item_t *i = stack->bot;
    while (i != NULL)
    {
        stack_item_t *deleted = i;

        if (i->next != NULL)
            i->next->prev = i->prev;
        if (i->prev != NULL)
            i->prev->next = i->next;

        i = i->next;
        free(deleted);
    }

    free(stack);
}

char* get_concat_stack(stack_t *stack)
{
    int size = 1;
    
    stack_item_t *marked_part = get_marked_part(stack);

    if (marked_part == NULL)
        return NULL;

    for (stack_item_t *i = marked_part->next; i != NULL; i = i->next)
        size += strlen(i->key);

    char *top = malloc(size*sizeof(char));
    if (top == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return NULL;
    }

    for (stack_item_t *i = marked_part->next; i != NULL; i = i->next)
        strcat(top, i->key);

    return top;
}

void mark_stack_term(stack_t *stack)
{
    stack->top_term->mark += 1;
}

void stack_push(stack_t *stack, token_t *token)
{
    stack_item_t* new = (stack_item_t*)malloc(sizeof(stack_item_t));
    if (new == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return;
    }
    
    new->tab_idx = token->tab_idx;
    new->key = token->key;
    new->mark = 0;
    new->is_term = true;

    new->next = NULL;
    stack->top->next = new;
    new->prev = stack->top;

    stack->top = new;
    stack->top_term = new;
}
