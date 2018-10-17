/**
 * @file semantic_parser.h
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of Semantic Analyse Parser of language IFJ18.
 */

#include "semantic_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include "error_handle.h"
#include "expressions_parser.h"

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

int parse_assig(table_item_t *sym_tb, stack_t* sem_stack, token_t *new_var)
{
    print_sem_stack(sem_stack);
    char *assig_var = stack_top(sem_stack);
    stack_pop(sem_stack);
    data_t *symb = search(sym_tb, assig_var);

    if(symb == NULL) // try to assig variable witch doesnt exist
        return ERR_SEM_DEF;

    data_t var;
    if (symb->data_type == FUN)
        var.data_type = symb->fun_type;
    else
        var.data_type = symb->data_type;
    var.id = new_var->attribute;
    var.value = symb->id; 
    var.fun_type = VAR;
    var.param_cnt = 0;
    var.param_id = NULL;

    insert(sym_tb, &var);

    fprintf(stderr,"DEFVAR %s\n", new_var->attribute);
    fprintf(stderr,"POPS %s\n", assig_var);

    free(assig_var);

    return SUCCESS;
}

int parse_operand(table_item_t *sym_tb, stack_t* sem_stack, token_t *op)
{
    print_sem_stack(sem_stack);
    printf("var: %s[%s]\n", op->attribute, get_real_type(op->type));

    if (op->type == VAR) //it is variable
    {
        data_t *symb = search(sym_tb, op->attribute);
        
        if (symb == NULL) // not initialized variable 
            return ERR_SEM_DEF;
        
        data_t var;
        if (symb->data_type == FUN) // TODO could even this happend ?
            var.data_type = symb->fun_type; //TODO handle this generating
        else
            var.data_type = symb->data_type;
        var.id = op->attribute;
        var.fun_type = VAR;
        var.param_cnt = 0;
        var.param_id = NULL;
        
        insert(sym_tb, &var);
    }
    else // it is constant
    {
        data_t cons;
        cons.data_type = op->type;
        cons.id = op->attribute;
        cons.fun_type = VAR;
        cons.param_cnt = 0;
        cons.param_id = NULL;
        
        insert(sym_tb, &cons);
    }

    fprintf(stderr,"PUSHS %s\n", op->attribute);
    stack_push(sem_stack, alloc_sem_item(op->attribute));

    return SUCCESS;
}

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, token_t *arit)
{
    print_sem_stack(sem_stack);

    switch (arit->type)
    {
        case ADD:
            return parse_add(sym_tb, sem_stack);
        case SUB:
            return parse_sub(sym_tb, sem_stack);
        case DIV:
            return parse_div(sym_tb, sem_stack);
        case MUL:
            return parse_mul(sym_tb, sem_stack);
        default:
            return ERR_SYNTAX; // should never happend
    }
}

int parse_add(table_item_t *sym_tb, stack_t* sem_stack)
{
    char *op1 = stack_top(sem_stack);
    data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    char *op2 = stack_top(sem_stack);
    data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);


    if (symb1 == NULL || symb2 == NULL) // not declared variable
        return ERR_SEM_DEF;

    int type1 = symb1->data_type;
    int type2 = symb2->data_type;
    if (type1 == FUN)
        type1 = symb1->fun_type;
    if (type2 == FUN)
        type2 = symb2->fun_type;

    printf("add: %s[%s] + %s[%s]\n", op1, get_real_type(type1), op2, get_real_type(type2));

    if (type1 == INTEGER && type1 == INTEGER)
    {
        char* tmp = insert_tmp(sym_tb, INTEGER, NULL);
        stack_push(sem_stack, alloc_sem_item(tmp));
        printf("ADDS\n");//res INTEGER -> generate ADDS
    }
    else if ((type1 == FLOAT && type2 == FLOAT) || 
             (type1 == INTEGER && type2 == FLOAT) ||
             (type1 == FLOAT && type2 == INTEGER))
    {
        char* tmp = insert_tmp(sym_tb, FLOAT, NULL);
        stack_push(sem_stack, alloc_sem_item(tmp));
        printf("ADDS\n");//res FLOAT -> generate ADDS
    }
    else if (type1 == STRING && type2 == STRING)
    {
        char* tmp1 = insert_tmp(sym_tb, STRING, op1);
        char* tmp2 = insert_tmp(sym_tb, STRING, op2);
        char* tmp3 = insert_tmp(sym_tb, STRING, tmp2);
        stack_push(sem_stack, alloc_sem_item(tmp3));
        fprintf(stderr,"DEFVAR LF@%s\n", tmp1);
        fprintf(stderr,"DEFVAR LF@%s\n", tmp2);
        fprintf(stderr,"POPS LF@%s\n", tmp1);
        fprintf(stderr,"POPS LF@%s\n", tmp2);
        fprintf(stderr,"CONCAT LF@%s LF@%s LF@%s\n", tmp3, tmp1, tmp2);
        fprintf(stderr,"PUSHS LF@%s\n", tmp3);
    }
    else
        return ERR_SEM_DEF;

    free(op1);
    free(op2);

    return SUCCESS;
}

int parse_sub(table_item_t *sym_tab, stack_t* sem_stack)
{

    return ERR_SEM_DEF;
}
int parse_mul(table_item_t *sym_tab, stack_t* sem_stack)
{

    return ERR_SEM_DEF;
}
int parse_div(table_item_t *sym_tab, stack_t* sem_stack)
{
    return ERR_SEM_DEF;
}
