#include "expressions_parser.h"

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

bool parse_expression(token_t *token)
{
    stack_t *stack = init_stack();
    stack_item_t *top;
    //print_stack(stack);
    set_top_term(stack);

    do
    {
        top = stack->top_term;
        //printf("top: \"%s\"\n", top->key);

        char *prec_tab = prec_table(top->tab_idx, token->tab_idx);
        //printf("table[top=\"%s\",%d; token=\"%s\",%d]=%s\n",top->key, top->tab_idx, token->key, token->tab_idx, prec_tab);

        if (strcmp(prec_tab, "=") == 0)
        {
            //printf("push_token(%s)\n", token->key);
            stack_push(stack, token);
            //print_stack(stack);
            token = get_token();
        }
        else if (strcmp(prec_tab, "<") == 0)
        {
            mark_stack_term(stack);
            //printf("push_token(%s)\n", token->key);
            stack_push(stack, token);
            //print_stack(stack);
            token = get_token();
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

    } while (token->tab_idx != _end || stack->top_term->tab_idx != _end);

    destroy_stack(stack);

    //print_stack(stack);

    return true; //success 
}

char* prec_table(int top, int token)
{
    char *table[13][13] = {
    //    *    /    +    -    <    ==  int  flt  str  nil   $    (    )  
        {">", ">", ">", ">", ">", ">", "<", "<", " ", " ", ">", "<", ">"},// *
        {">", ">", ">", ">", ">", ">", "<", "<", " ", " ", ">", "<", ">"},// /
        {"<", "<", ">", ">", ">", ">", "<", "<", "<", " ", ">", "<", ">"},// +
        {"<", "<", ">", ">", ">", ">", "<", "<", " ", " ", ">", "<", ">"},// -
        {"<", "<", "<", "<", "<", "<", "<", "<", "<", " ", ">", "<", ">"},// <
        {"<", "<", "<", "<", "<", ">", "<", "<", "<", "<", ">", "<", ">"},// ==
        {">", ">", ">", ">", ">", ">", " ", " ", " ", " ", ">", " ", ">"},// int
        {">", ">", ">", ">", ">", ">", " ", " ", " ", " ", ">", " ", ">"},// flt
        {" ", " ", ">", " ", ">", ">", " ", " ", " ", " ", ">", " ", ">"},// str
        {" ", " ", " ", " ", " ", ">", " ", " ", " ", " ", ">", " ", ">"},// nil
        {"<", "<", "<", "<", "<", "<", "<", "<", "<", "<", " ", "<", ">"},// $
        {"<", "<", "<", "<", "<", "<", "<", "<", " ", " ", " ", "<", "="},// (
        {">", ">", ">", ">", ">", ">", ">", ">", ">", ">", ">", " ", ">"},// )
    };

    if (top < 13 || token < 13)
        return table[top][token];
    else
        return " ";
}

bool find_rule(stack_t *stack)
{
    char *top = get_concat_stack(stack); //allocate string

    if (top == NULL)
        return false;

    if (strcmp(top, INT_K)==0)
        apply_rule("E", stack);
    else if (strcmp(top, FLOAT_K)==0)
        apply_rule("E", stack);
    else if (strcmp(top, STRING_K)==0)
        apply_rule("E", stack);
    else if (strcmp(top, NIL_K)==0)
        apply_rule("E", stack);
    else if (strcmp(top, "(E)")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E+E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E-E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "-E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E*E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E/E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E/E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E==E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E!=E")==0) 
        apply_rule("E", stack);
    else if (strcmp(top, "E<E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E>E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E<=E")==0)
        apply_rule("E", stack);
    else if (strcmp(top, "E>=E")==0) 
        apply_rule("E", stack);
    else
        return false;

    free(top); // free string

    return true;
}
