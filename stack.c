/**
 * @file stack.c
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Definition of pseudo stack data structure with necessary functions
 * above it.
 */

#include "stack.h"
#include "scanner.h"
#include "expressions_parser.h"

void print_stack(stack_t *stack)
{
    printf("stack: \"");
    stack_item_t *bot = stack->bot;
    while (bot != NULL)
    {
        printf("%s", get_type(bot->type));
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
            return i;

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

void apply_rule(int type, stack_item_t *marked_part)
{
    marked_part->prev->mark -= 1;
    marked_part->is_term = false;
    marked_part->type = type;

    marked_part = marked_part->next;
    while (marked_part != NULL)
    {
        stack_item_t *deleted = marked_part;

        if (marked_part->next != NULL)
            marked_part->next->prev = marked_part->prev;
        if (marked_part->prev != NULL)
            marked_part->prev->next = marked_part->next;

        free(deleted->val);
        marked_part = marked_part->next;
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
    first->type = LINE_END;
    first->val = "";
    first->mark = 0;
    first->is_term = true;
    first->is_const = false;

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

        if (i != stack->bot)
            free(deleted->val);
        i = i->next;
        free(deleted);
    }

    free(stack);
}

void mark_stack_term(stack_t *stack)
{
    stack->top_term->mark += 1;
}

void stack_push(stack_t *stack, stack_item_t *item)
{
    item->next = NULL;
    stack->top->next = item;
    item->prev = stack->top;

    stack->top = item;
    stack->top_term = item;
}
