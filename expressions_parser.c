#include "expressions_parser.h"

/**
 * @file expression:parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Definition of Expression Parser of language IFJ18. Parser use
 * precedence syntax analyse algorithm with precedence table.
 */

/**
 * Possible values of token key.
 */
#define INT_K "int"
#define FLOAT_K "float"
#define STRING_K "string"
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



/**********************     WORKING VERSION OF GRAMATIC  **********************

Gram = (Nonterm,Term,Rule,Start)
Nonterm = {E}
Term = {int, float, string, nil, $, (, ), <, >, <=, >=, ==, !=, +, -, *, /}
Rule = {
    E -> int
    E -> float
    E -> string
    E -> nil
    E -> (E)
    E -> - E
    E -> E + E 
    E -> E - E 
    E -> E / E 
    E -> E * E 
    E -> E < E
    E -> E > E
    E -> E <= E
    E -> E >= E
    E -> E == E
    E -> E != E
}
Start = {$}

******************************************************************************/

char *get_type(int type)
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
        case LINE_END: return "EOL"; break;
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

stack_item_t* create_stack_item(token_t *token)
{
    int val_size;
    char *val;

    stack_item_t* new = (stack_item_t*)malloc(sizeof(stack_item_t));
    if (new == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return NULL;
    }

    if (token->type == VAR)
    {
        data_t *symbol = search(token->attribute);
        if (symbol == NULL) //not found -> should not happend
            return NULL;

        val_size = strlen(symbol->id);
        new->type = symbol->data_type;
        val = symbol->id;
        new->is_const = false;
    }
    else
    {
        val_size = strlen(token->attribute);
        new->type = token->type;
        val = token->attribute;
        new->is_const = true;
    }

    new->val = malloc(sizeof(char)*val_size+1);
    if (new->val == NULL)
    {
        fprintf(stderr, "Not enough memory!\n");
        return NULL;
    }
    strcpy(new->val, val);
    new->mark = 0;
    new->is_term = true;
    
    return new;
}

void print_prec_table(int top, int input_sym, char *prec_tab)
{
    printf("table[top=\"%s\",%d; token=\"%s\",%d]=%s\n",
        get_type(map_index(top)),
        map_index(top),
        get_type(map_index(input_sym)),
        map_index(input_sym),
        prec_tab);
}

bool parse_expression()
{
    char *prec_tab;
    stack_item_t *input_sym = create_stack_item(get_token());
    stack_t *stack = init_stack();
    stack_item_t *top = stack->top_term;
    set_top_term(stack);

    while (input_sym->type < LINE_END || top->type < LINE_END)
    {
        prec_tab = prec_table(top->type, input_sym->type);
        //print_prec_table(top->type, input_sym->type, prec_tab);

        if (strcmp(prec_tab, "=") == 0)
        {
            stack_push(stack, input_sym);
            //print_stack(stack);
            input_sym = create_stack_item(get_token());
        }
        else if (strcmp(prec_tab, "<") == 0)
        {
            mark_stack_term(stack);
            stack_push(stack, input_sym);
            //print_stack(stack);
            input_sym = create_stack_item(get_token());
        }
        else if (strcmp(prec_tab, ">") == 0)
        {
            if (!find_rule(stack))
                return false; //error
            //print_stack(stack);
        }
        else
            return false; //ERROR
        
        set_top_term(stack);
        top = stack->top_term;
    } 

    free(input_sym->val);
    free(input_sym);
    destroy_stack(stack);

    return true; //success 
}

int map_index(int idx)
{
    if (idx == GREATER || idx == LESS_EQ || idx == GREATER_EQ)
        idx = LESS;
    else if (idx == EQUAL)
        idx = NOT_EQUAL;
    else if (idx == FLOAT)
        idx = INTEGER;
    else if (idx == DO || idx == THEN || idx == DELIM)
        idx = LINE_END;

    return idx;
}

char* prec_table(int top, int token)
{
    top = map_index(top);
    token = map_index(token);

    char *table[13][13] = {
    //   not   -    +    *    /    ==   <   int  str  nil   (    )    $      
        {" " ," ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},// not
        {" " ,">", ">", "<", "<", ">", ">", "<", "<", " ", "<", ">", ">"},// -
        {" " ,">", ">", "<", "<", ">", ">", "<", " ", " ", "<", ">", ">"},// +
        {" " ,">", ">", ">", ">", ">", ">", "<", " ", " ", "<", ">", ">"},// *
        {" " ,">", ">", ">", ">", ">", ">", "<", "<", "<", "<", ">", ">"},// /
        {" " ,"<", "<", "<", "<", ">", ">", "<", "<", " ", "<", ">", ">"},// ==
        {" " ,"<", "<", "<", "<", "<", "<", "<", " ", " ", "<", ">", ">"},// <
        {" " ,">", ">", ">", ">", ">", ">", " ", " ", " ", " ", ">", ">"},// int 
        {" " ," ", ">", " ", " ", ">", ">", " ", " ", " ", " ", ">", ">"},// str
        {" " ," ", " ", " ", " ", ">", " ", " ", " ", " ", " ", ">", ">"},// nil
        {" " ,"<", "<", "<", "<", "<", "<", "<", "<", "<", "<", "<", " "},// (
        {" " ,">", ">", ">", ">", ">", ">", ">", ">", ">", " ", ">", ">"},// )
        {" " ,"<", "<", "<", "<", "<", "<", "<", "<", "<", "<", "<", " "},// $
    };

    if (top < 13 && token < 13)
        return table[top][token];
    else
        return " ";
}

bool handle_one(stack_item_t *marked)
{
    //define variable
    if (marked->type == INTEGER)
    {
        printf("INT VAR %s\n", marked->val);
        apply_rule(INTEGER, marked);
    }
    else if (marked->type == FLOAT)
    {
        printf("FLOAT VAR %s\n", marked->val);
        apply_rule(FLOAT, marked);
    }
    else if (marked->type == STRING) 
    {
        printf("STRING VAR %s\n", marked->val);
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

bool find_rule(stack_t *stack)
{
    int size = 0;
    stack_item_t *marked_part = get_marked_part(stack);
    stack->top = marked_part->next;
    for (stack_item_t *i = marked_part->next; i != NULL; i = i->next)
        size++;

    if (size == 1)
        return handle_one(marked_part->next);
    else if (size == 2)
        return handle_two(marked_part->next);
    else if (size == 3)
        return handle_three(marked_part->next);
    else
        return false;
}
