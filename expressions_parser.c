#include "expressions_parser.h"

#include <stdio.h>
#include <string.h>
#include "symtable.h"
#include "error_handle.h"
#include <stdlib.h>

/**
 * @file expression:parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Definition of Expression Parser of language IFJ18. Parser use
 * precedence syntax analyse algorithm with precedence table.
 */

/**********************     WORKING VERSION OF GRAMATIC  **********************
Gram = (Nonterm,Term,Rule,Start)
Nonterm = {E}
Term = {int, float, string, nil, $, (, ), <, >, <=, >=, ==, !=, +, -, *, /, EOL, EOF}
Rule = {
     1: E -> id
     2: E -> nil
     3: E -> (E)
     4: E -> - E
     5: E -> not E
     6: E -> E + E 
     7: E -> E - E 
     8: E -> E / E 
     9: E -> E == E
    10: E -> E = E
}
Start = {$}
******************************************************************************/

char *get_real_type(int type)
{
    switch (type)
    {
        case NOT: return "NOT"; break;
        case SUB: return "-"; break;
        case ADD: return "+"; break;
        case MUL: return "*"; break;
        case DIV: return "/"; break;
        case NOT_EQUAL: return "!="; break;
        case LESS: return "<"; break;
        case INTEGER: return "int"; break;
        case STRING: return "string"; break;
        case NIL: return "nil"; break;
        case LEFT_B: return "("; break;
        case RIGHT_B: return ")"; break;
        case EOL: return "EOL"; break;
        case EQUAL: return "=="; break;
        case GREATER: return ">"; break;
        case LESS_EQ: return "<="; break;
        case GREATER_EQ: return ">="; break;
        case FLOAT: return "float"; break;
        case DEF: return "def"; break;
        case DO: return "do"; break;
        case ELSE: return "else"; break;
        case END: return "end"; break;
        case IF: return "if"; break;
        case THEN: return "then"; break;
        case WHILE: return "while"; break;
        case ASSIG: return "="; break;
        case FUN: return "function"; break;
        case DELIM: return ","; break;
        case VAR: return "variable"; break;
        default: return "UNKNOWN";
    }
}

char *get_syntax_type(int type)
{
    switch(type)
    {
        case PT_NOT:
            return "not";
        case PT_SUB:
            return "-";
        case PT_ADD:
            return "+";
        case PT_MUL:
            return "*";
        case PT_EQ:
            return "==";
        case PT_ASSIG:
            return "=";
        case PT_ID:
            return "id";
        case PT_LEFT_B:
            return "(";
        case PT_RIGHT_B:
            return ")";
        case PT_END:
            return "$";
        case PT_DELIM:
            return ",";
        case PT_FUN:
            return "fun";
        default:
            return "unknown";
    }

}

stack_item_t* create_stack_item(token_t *token)
{
    stack_item_t* new = malloc(sizeof(stack_item_t));
    token_t* copy_of_token = malloc(sizeof(token_t));
    if (new == NULL || copy_of_token == NULL)
    {
        free(new);
        free(copy_of_token);
        mem_error();
        return NULL;
    }
    copy_of_token->attribute = malloc(sizeof(char) * strlen(token->attribute) + 1);
    if (copy_of_token->attribute == NULL)
    {
        free(new);
        free(copy_of_token);
        mem_error();
        return NULL;
    }

    if (token->type == VAR)
    {
        data_t *sym = search(token->attribute);
        if (sym != NULL)
        {
            if (sym->data_type == FUN)
                token->type = FUN;
        }
    }


    strcpy(copy_of_token->attribute, token->attribute);
    copy_of_token->type = token->type;
    new->token = copy_of_token;
    new->mark = 0;
    new->is_term = true;
    
    return new;
}

void print_prec_table(int top, int input_sym, char *prec_tab)
{
    fprintf(stderr,"table[top=\"%s\",%d; token=\"%s\",%d]=%s\n",
        get_syntax_type(top),
        map_index(top),
        get_syntax_type(input_sym),
        map_index(input_sym),
        prec_tab);
}

bool parse_expression()
{
    char *prec_tab;
    stack_item_t *input_sym = create_stack_item(get_token());
    fprintf(stderr,"%d\n", input_sym->token->type);
    stack_t *stack = init_stack();
    if (stack == NULL || input_sym == NULL)
        return ERR_COMPILER;
    int top = map_index(stack->top_term->token->type);
    int input = map_index(input_sym->token->type);
    fprintf(stderr,"%d\n", input);

    while (input != PT_END || top != PT_END)
    {
        prec_tab = prec_table(top, input);
        print_prec_table(top, input, prec_tab);

        if (strcmp(prec_tab, "=") == 0)
        {
            stack_push(stack, input_sym);
            print_stack(stack);
            input_sym = create_stack_item(get_token());
        }
        else if (strcmp(prec_tab, "<") == 0)
        {
            mark_stack_term(stack);
            stack_push(stack, input_sym);
            print_stack(stack);
            input_sym = create_stack_item(get_token());
        }
        else if (strcmp(prec_tab, ">") == 0)
        {
            if (!find_rule(stack))
                return false; //error
            print_stack(stack);
        }
        else
            return false; //ERROR

        set_top_term(stack);
        top = map_index(stack->top_term->token->type);
        input = map_index(input_sym->token->type);
    } 

    free(input_sym->token->attribute);
    free(input_sym->token);
    free(input_sym);
    destroy_stack(stack);

    return true; //success 
}

int map_index(int idx)
{
    switch (idx)
    {
        case NOT:
            return PT_NOT;
        case SUB:
            return PT_SUB;
        case ADD:
            return PT_ADD;
        case MUL:
        case DIV:
            return PT_MUL;
        case EQUAL:
        case NOT_EQUAL:
        case LESS:
        case LESS_EQ:
        case GREATER:
        case GREATER_EQ:
            return PT_EQ;
        case ASSIG:
            return PT_ASSIG;
        case VAR:
        case INTEGER:
        case FLOAT:
        case STRING:
        case NIL:
            return PT_ID;
        case LEFT_B:
            return PT_LEFT_B;
        case RIGHT_B:
            return PT_RIGHT_B;
        case DELIM:
            return PT_DELIM;
        case FUN:
            return PT_FUN;
        case EOL:
        case THEN:
        case DO:
        case EOF:
            return PT_END;
        default:
            return PT_ERR;
    }
}

char* prec_table(int top, int token)
{
    char *table[12][12] = {
    //   not   -    +    /    ==   =    id   (    )    $    ,    f   
        {" " ," ", " ", " ", "<", "<", "<", "<", ">", ">", ">", "<"},// not
        {">" ,">", ">", "<", ">", ">", "<", "<", ">", ">", ">", "<"},// -
        {">" ,">", ">", "<", ">", ">", "<", "<", ">", ">", ">", "<"},// +
        {">" ,">", ">", ">", ">", ">", "<", "<", ">", ">", ">", "<"},// /
        {">" ,"<", "<", "<", "<", "<", "<", "<", ">", ">", ">", "<"},// ==
        {">" ,"<", "<", "<", "<", ">", "<", "<", ">", ">", ">", "<"},// <
        {">" ,">", ">", ">", ">", ">", " ", " ", ">", ">", ">", " "},// id 
        {"<" ,"<", "<", "<", "<", "<", "<", "<", "=", " ", "=", "<"},// (
        {">" ,">", ">", ">", ">", ">", ">", " ", ">", ">", ">", " "},// )
        {"<" ,"<", "<", "<", "<", "<", "<", "<", "<", " ", " ", " "},// $
        {"<" ,"<", "<", "<", " ", "<", "<", "<", "=", ">", "=", "<"},// ,
        {"<" ,"<", "<", "<", " ", "<", "<", "=", " ", ">", "=", " "} // f
    };

    if (top != PT_ERR && token != PT_ERR)
        return table[top][token];
    else
        return " ";
}

/*
bool handle_one(stack_item_t *marked)
{
    //define variable
    if (marked->type == INTEGER)
    {
        printf("PUSHS %s\n", marked->val);
        apply_rule(INTEGER, marked);
    }
    else if (marked->type == FLOAT)
    {
        printf("PUSHS %s\n", marked->val);
        apply_rule(FLOAT, marked);
    }
    else if (marked->type == STRING) 
    {
        printf("PUSHS VAR %s\n", marked->val);
        apply_rule(STRING, marked);
    }
    else if (marked->type == NIL) 
    {
        printf("NIL VAR\n");
        apply_rule(NIL, marked);
    }
    else
        return false;

    return true;
}

bool handle_two(stack_item_t *marked)
{
    stack_item_t *first = marked;
    stack_item_t *second = marked->next;
    if (first->type == NOT && !second->is_term) //comparsion negation
    {
        printf("NOT %s\n", second->val);
        apply_rule(NOT, marked); //TODO bool?
    }
    else if (first->type == SUB && !second->is_term) //define negative variable TODO what if - string???
    {
        printf("VAR %s = - %s\n", second->val, second->val);
        apply_rule(second->type, marked); //TODO bool?
    }
    else return false;

    return true;
}

bool handle_three(stack_item_t *marked)
{
    stack_item_t *first = marked;
    stack_item_t *second = marked->next;
    stack_item_t *third = marked->next->next;

    if (!first->is_term && !third->is_term)
    {
        if (second->type == SUB || second->type == ADD
            || second->type == MUL || second->type == DIV)
        {
            printf("ARIT %s %s = %s, %s\n", get_type(second->type), first->val, first->val, third->val);
            apply_rule(first->type, marked);//vysledok arit op INTEGER or FLOAT
        }
        else if (second->type == LESS || second->type == GREATER 
            || second->type == NOT_EQUAL || second->type == EQUAL
            || second->type == LESS_EQ || second->type == GREATER_EQ)
        {
            printf("CMP %d = %s, %s\n", second->type, first->val, third->val);
            apply_rule(second->type, marked);// COMPARE
        }
        else 
            return false;
    }
    else if (first->type == LEFT_B && !second->is_term && third->type == RIGHT_B)
        apply_rule(second->type, marked);// do nothing just remove brackets
    else
        return false;

    return true;
}
*/
bool find_rule(stack_t *stack)
{
    stack_item_t *marked_part = get_marked_part(stack);
    stack->top = marked_part->next;

    apply_rule(marked_part->next);

    /*
    if (size == 1)
        return handle_one(marked_part->next);
    else if (size == 2)
        return handle_two(marked_part->next);
    else if (size == 3)
        return handle_three(marked_part->next);
    else
        return false;
        */
    return true;
}

void print_stack(stack_t *stack)
{
    fprintf(stderr,"stack: \"");
    stack_item_t *bot = stack->bot;
    while (bot != NULL)
    {
        fprintf(stderr,"%s", get_real_type(bot->token->type));
        for (int i = 0; i < bot->mark; i++)
            fprintf(stderr,"%c", '<');
        bot = bot->next;
    }

    fprintf(stderr,"\"\n");
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

void apply_rule(stack_item_t *marked_part)
{
    marked_part->prev->mark -= 1;
    marked_part->is_term = false;

    marked_part = marked_part->next;
    while (marked_part != NULL)
    {
        stack_item_t *deleted = marked_part;

        if (marked_part->next != NULL)
            marked_part->next->prev = marked_part->prev;
        if (marked_part->prev != NULL)
            marked_part->prev->next = marked_part->next;

        free(deleted->token->attribute);
        free(deleted->token);
        marked_part = marked_part->next;
        free(deleted);
    }
}

stack_t* init_stack()
{
    token_t t = {.type=EOL, .attribute=""};
    stack_t* new = (stack_t*)malloc(sizeof(stack_t));
    stack_item_t *first = create_stack_item(&t);
    if (new == NULL || first == NULL)
    {
        free(new);
        mem_error();
        return NULL;
    }

    first->prev = NULL;
    first->next = NULL;
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

        if (i != stack->bot)
        {
            free(deleted->token->attribute);
            free(deleted->token);
        }
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
