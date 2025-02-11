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
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
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
     3: E -> E [-,+,/,*] E 
     4: E -> E [==, !=, <, >, <=, >=] E
}
Start = {$}
******************************************************************************/

// 1: E -> id
int syntax_parse_id(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
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

    return parse_operand(hash_tb, sem_stack, symb->token);
    //return SUCCESS;
}
// 2: E -> ( E )
int syntax_parse_brackets(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
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
int syntax_parse_arit(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
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

    return parse_arit_op(hash_tb, sem_stack, symb2->token->type);
    //return SUCCESS;
}
//     6: E -> E [==, !=, <, >, <=, >=] E
int syntax_parse_logic(table_item_t *hash_tb, stack_t *sem_stack, stack_item_t *marked)
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

    return parse_logic_op(hash_tb, sem_stack, symb2->token->type);
}

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
        case PT_ID:
            return "id";
        case PT_LEFT_B:
            return "(";
        case PT_RIGHT_B:
            return ")";
        case PT_END:
            return "$";
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
    if (token->type == ERROR)
        return ERR_LEX;
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
            if (token->type == ERROR)
            {
                free_syntax_item(input_sym);
                free_syntax_stack(stack);
                free_sem_stack(sem_stack);
                return ERR_LEX;
            }
        }
        else if (prec_tab == '<')
        {
            mark_stack_term(stack);
            stack_push(stack, input_sym);
            //print_stack(stack);
            token = get_token();
            input_sym = alloc_syntax_item(token, hash_tb);
            if (token->type == ERROR)
            {
                free_syntax_item(input_sym);
                free_syntax_stack(stack);
                free_sem_stack(sem_stack);
                return ERR_LEX;
            }
        }
        else if (prec_tab == '>')
        {
            int result = find_rule(hash_tb, stack, sem_stack);
            if (result != SUCCESS)
            {
                free_syntax_item(input_sym);
                free_syntax_stack(stack);
                free_sem_stack(sem_stack);

                return result; // SEMANTIC ERROR
            }
            //print_stack(stack);
        }
        else
        {
            free_syntax_item(input_sym);
            free_syntax_stack(stack);
            free_sem_stack(sem_stack);

            return ERR_SYNTAX; // SYNTAX ERROR
        }

        top_term = get_top_term(stack)->data;
        top = map_index(top_term->token->type);
        input = map_index(input_sym->token->type);
    } 

    free_syntax_item(input_sym);
    free_syntax_stack(stack);
    //print_sem_stack(sem_stack);
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
    char table[9][9] = {
    //   not   -    +    /    ==   id   (    )    $   
        {' ' ,' ', ' ', ' ', '<', '<', '<', '>', '>'},// not
        {'>' ,'>', '>', '<', '>', '<', '<', '>', '>'},// -
        {'>' ,'>', '>', '<', '>', '<', '<', '>', '>'},// +
        {'>' ,'>', '>', '>', '>', '<', '<', '>', '>'},// /
        {'>' ,'<', '<', '<', '<', '<', '<', '>', '>'},// ==
        {'>' ,'>', '>', '>', '>', ' ', ' ', '>', '>'},// id 
        {'<' ,'<', '<', '<', '<', '<', '<', '=', ' '},// (
        {'>' ,'>', '>', '>', '>', '>', ' ', '>', '>'},// )
        {'<' ,'<', '<', '<', '<', '<', '<', '<', ' '} // $
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

    if ((res = syntax_parse_id(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = syntax_parse_brackets(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = syntax_parse_arit(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;
    if ((res = syntax_parse_logic(hash_tb, sem_stack, mark)) != ERR_SYNTAX)
        rule_exist = res;

    if (rule_exist != ERR_SYNTAX)
        free_mark(syntax_stack);
    
    return rule_exist;
}
