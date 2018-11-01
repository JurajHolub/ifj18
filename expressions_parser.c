#include "expressions_parser.h"

#include <stdio.h>
#include <string.h>
#include "symtable.h"
#include "error_handle.h"
#include <stdlib.h>
#include "semantic_parser.h"
#include "stack.h"

/**
 * @file expressions_parser.c
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
     2: E -> ( E )
     3: E -> - E
     4: E -> not E
     5: E -> E [-,+,/,*] E 
     6: E -> E [==, !=, <, >, <=, >=] E
     7: E -> id = E
     8: E -> f ([E^n])
     9: E -> f [E^n]
}
Start = {$}
******************************************************************************/

// 1: E -> id
int apply_rule_1(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
{
    syntax_t *symb = marked->data;
    if (symb->token->type != VAR && symb->token->type != INTEGER &&
        symb->token->type != STRING && symb->token->type != FLOAT &&
        symb->token->type != NIL && symb->token->type != FUN )
    {
        return ERR_SYNTAX;
    }
    if (marked->next != NULL)
        return ERR_SYNTAX;

    //return parse_operand(hash_tb, sem_stack, symb->token);
    return SUCCESS;
}
// 2: E -> ( E )
int apply_rule_2(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
{
    syntax_t *symb1, *symb2, *symb3;
    symb1 = marked->data;
    
    // marked => ( E )
    if (symb1->token->type != LEFT_B)
        return ERR_SYNTAX;

    if (marked->next == NULL)
        return ERR_SYNTAX;
    
    symb2 = marked->next->data;

    if (symb2->is_term != false)
        return ERR_SYNTAX;

    // marked => E )

    if (marked->next->next == NULL)
        return ERR_SYNTAX;

    symb3 = marked->next->next->data;
        
    if (symb3->token->type != RIGHT_B)
        return ERR_SYNTAX;

    // marked => E

    if (marked->next->next->next != NULL)
        return ERR_SYNTAX;

    return SUCCESS;
}

//   5: E -> E [-,+,/,*] E 
int apply_rule_3(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
{
    syntax_t *symb1, *symb2, *symb3;
    symb1 = marked->data;

    // marked => E [-,+,/,*] E
    if (symb1->is_term != false)
        return ERR_SYNTAX;

    if (marked->next == NULL)
        return ERR_SYNTAX;

    symb2 = marked->next->data;

    if (symb2->token->type != SUB && symb2->token->type != ADD &&
            symb2->token->type != MUL && symb2->token->type != DIV)
        return ERR_SYNTAX;

    // marked => E E

    if (marked->next->next == NULL)
        return ERR_SYNTAX;

    symb3 = marked->next->next->data;
        
    if (symb3->is_term != false)
        return ERR_SYNTAX;

    // marked => E

    if (marked->next->next->next != NULL)
        return ERR_SYNTAX;

    //return parse_arit_op(hash_tb, sem_stack, symb2->token->type);
    return SUCCESS;
}
//     6: E -> E [==, !=, <, >, <=, >=] E
int apply_rule_4(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
{
    syntax_t *symb1, *symb2, *symb3;
    symb1 = marked->data;

    // marked => E [==, !=, <, >, <=, >=] E
    if (symb1->is_term != false)
        return ERR_SYNTAX;

    if (marked->next == NULL)
        return ERR_SYNTAX;

    symb2 = marked->next->data;

    if (symb2->token->type != EQUAL && symb2->token->type != NOT_EQUAL &&
        symb2->token->type != LESS && symb2->token->type != GREATER &&
        symb2->token->type != LESS_EQ && symb2->token->type != GREATER_EQ)
        return ERR_SYNTAX;

    // marked => E E

    if (marked->next->next == NULL)
        return ERR_SYNTAX;

    symb3 = marked->next->next->data;
        
    if (symb3->is_term != false)
        return ERR_SYNTAX;

    // marked => E

    if (marked->next->next->next != NULL)
        return ERR_SYNTAX;

    //return parse_logic_op(hash_tb, sem_stack, symb2->token->type);
    return SUCCESS;
}
//   7: E -> id = E
int apply_rule_5(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
{
    syntax_t *symb1, *symb2, *symb3;
    symb1 = marked->data;

    // marked => id = E
    if (!symb1->is_term)
        return ERR_SYNTAX;

    if (marked->next == NULL)
        return ERR_SYNTAX;

    symb2 = marked->next->data;

    if (symb2->token->type != ASSIG)
        return ERR_SYNTAX;

    // marked => E E

    if (marked->next->next == NULL)
        return ERR_SYNTAX;

    symb3 = marked->next->next->data;
        
    if (symb3->is_term != false)
        return ERR_SYNTAX;

    // marked => E

    if (marked->next->next->next != NULL)
        return ERR_SYNTAX;

    //return parse_assig(hash_tb, sem_stack, symb3->token);
    return SUCCESS;
}
/*
//     8: E -> f ([E^n])
int apply_rule_8(table_item_t *hash_tb, stack_item_t *marked)
{
    syntax_t *symb1, *symb2, *symb3;
    symb1 = marked->data;
    // marked => f ( [E^n] )
    
    if (symb1->token->type != FUN)
        return ERR_SYNTAX;

    if (marked->next != NULL)
    {
        symb2 = marked->data;
        
        if (symb2->token->type == LEFT_B)
        {
            destroy_stack_item(marked->next); 
            // marked => f E, ... ,E )

            if (marked->next == NULL)
                return ERR_SYNTAX;

            int cnt = 1;
            // marked => f E,E, ... ,E )

            while (marked->next->next != NULL)
            {
                if (cnt % 2 == 1)   // E
                {
                    if (marked->next->data->is_term != false)
                        return ERR_SYNTAX;
                }
                else                // ,
                {
                    if (marked->next->data->token->type != DELIM)
                        return ERR_SYNTAX;
                }
                destroy_stack_item(marked->next); 

                if (marked->next->next != NULL)
                    cnt++;
            }

            if (cnt % 2 != 1) // E
                return ERR_SYNTAX;

            if (marked->next->data->token->type != RIGHT_B)
                return ERR_SYNTAX;
            
            destroy_stack_item(marked->next); 
        }
        else
        {
            int cnt = 1;
            while (marked->next != NULL)
            {
                if (cnt % 2 == 1)   // E
                {
                    if (marked->next->data->is_term != false)
                        return ERR_SYNTAX;
                }
                else                // ,
                {
                    if (marked->next->data->token->type != DELIM)
                        return ERR_SYNTAX;
                }
                destroy_stack_item(marked->next); 

                if (marked->next != NULL)
                    cnt++;
            }

            if (cnt % 2 != 1) // E
                return ERR_SYNTAX;
        }
    }

    //TODO apply semantic analyse for rule_5
    return SUCCESS;
}
*/

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

void print_prec_table(int top, int input_sym, char prec_tab)
{
    fprintf(stderr,"table[top=\"%s\",%d; token=\"%s\",%d]=%c\n",
        get_syntax_type(top),
        map_index(top),
        get_syntax_type(input_sym),
        map_index(input_sym),
        prec_tab);
}

int parse_expression(table_item_t *hash_tb)
{
    char prec_tab;
    token_t *token = get_token();
    stack_t *sem_stack = init_sem_stack();
    syntax_t *input_sym = alloc_syntax_item(token, hash_tb);
    stack_t *stack = init_syntax_stack();
    if (stack == NULL || input_sym == NULL)
        return ERR_COMPILER;
    syntax_t* top_term = get_top_term(stack)->data;
    int top = map_index(top_term->token->type);
    int input = map_index(input_sym->token->type);

    while (input != PT_END || top != PT_END)
    {
        prec_tab = prec_table(top, input);
        //print_prec_table(top, input, prec_tab);

        if (prec_tab == '=')
        {
            stack_push(stack, input_sym);
            //print_stack(stack);
            token = get_token();
            input_sym = alloc_syntax_item(token, hash_tb);
        }
        else if (prec_tab == '<')
        {
            mark_stack_term(stack);
            stack_push(stack, input_sym);
            //print_stack(stack);
            token = get_token();
            input_sym = alloc_syntax_item(token, hash_tb);
        }
        else if (prec_tab == '>')
        {
            int result = find_rule(hash_tb, stack, sem_stack);
            if (result != SUCCESS)
                return result; //error
            //print_stack(stack);
        }
        else
            return ERR_SYNTAX; //ERROR

        top_term = get_top_term(stack)->data;
        top = map_index(top_term->token->type);
        input = map_index(input_sym->token->type);
    } 

    free_syntax_item(input_sym);
    free_syntax_stack(stack);
    free_sem_stack(sem_stack);

    ret_token(token);

    return SUCCESS; //success 
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

char prec_table(int top, int token)
{
    char table[12][12] = {
    //   not   -    +    /    ==   =    id   (    )    $    ,    f   
        {' ' ,' ', ' ', ' ', '<', '<', '<', '<', '>', '>', '>', '<'},// not
        {'>' ,'>', '>', '<', '>', '>', '<', '<', '>', '>', '>', '<'},// -
        {'>' ,'>', '>', '<', '>', '>', '<', '<', '>', '>', '>', '<'},// +
        {'>' ,'>', '>', '>', '>', '>', '<', '<', '>', '>', '>', '<'},// /
        {'>' ,'<', '<', '<', '<', '<', '<', '<', '>', '>', '>', '<'},// ==
        {'>' ,'<', '<', '<', '<', '>', '<', '<', '>', '>', '>', '<'},// =
        {'>' ,'>', '>', '>', '>', '=', ' ', ' ', '>', '>', '>', ' '},// id 
        {'<' ,'<', '<', '<', '<', '<', '<', '<', '=', ' ', '=', '<'},// (
        {'>' ,'>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', ' '},// )
        {'<' ,'<', '<', '<', '<', '<', '<', '<', '<', ' ', ' ', '<'},// $
        {'<' ,'<', '<', '<', ' ', '<', '<', '<', '=', '>', '=', '<'},// ,
        {'<' ,'<', '<', '<', ' ', '<', '<', '=', ' ', '>', '=', ' '} // f
    };

    if (top != PT_ERR && token != PT_ERR)
        return table[top][token];
    else
        return ' ';
}

int find_rule(table_item_t *hash_tb, stack_t *syntax_stack, stack_t *sem_stack)
{
    stack_item_t *mark = get_marked_part(syntax_stack);

    if (mark == NULL)
        return false;

    int rule_exist = ERR_SYNTAX;
    int res;

    if ((res = apply_rule_1(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = apply_rule_2(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = apply_rule_3(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = apply_rule_4(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = apply_rule_5(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;

    if (rule_exist != ERR_SYNTAX)
        free_mark(syntax_stack);
    
    return rule_exist;
}
