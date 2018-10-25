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

    printf("DEFVAR %s\n", new_var->attribute);
    printf("POPS %s\n", assig_var);

    free(assig_var);

    return SUCCESS;
}

int parse_operand(table_item_t *sym_tb, stack_t* sem_stack, token_t *op)
{
    print_sem_stack(sem_stack);
    //printf("var: %s[%s]\n", op->attribute, get_real_type(op->type));

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
        var.value = symb->value;
        
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
        cons.value = op->attribute;
        
        insert(sym_tb, &cons);
    }

    printf("PUSHS %s\n", op->attribute);
    stack_push(sem_stack, alloc_sem_item(op->attribute));

    return SUCCESS;
}

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, int arit_op)
{
    char *op1 = stack_top(sem_stack);
    data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    char *op2 = stack_top(sem_stack);
    data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);


    if (symb1 == NULL || symb2 == NULL) // not declared variable
        return ERR_SEM_DEF;

    int res = convert_type(sem_stack, sym_tb, symb1, symb2);

    if (res == INTEGER && res == FLOAT)
    {
        char* tmp = insert_tmp(sym_tb, res, NULL);
        stack_push(sem_stack, alloc_sem_item(tmp));

        if (arit_op == ADD)
            printf("ADDS\n");
        else if (arit_op == SUB)
            printf("SUBS\n");
        else if (arit_op == DIV)
            printf("DIVS\n");
        else if (arit_op == MUL)
            printf("MULS\n");
        else 
            return ERR_SYNTAX; // should never happend
    }
    else if (res == STRING)
    {
        if (arit_op != ADD)
            return ERR_SYNTAX; // should never happend

        char* tmp1 = insert_tmp(sym_tb, STRING, op1);
        char* tmp2 = insert_tmp(sym_tb, STRING, op2);
        char* tmp3 = insert_tmp(sym_tb, STRING, tmp2);
        stack_push(sem_stack, alloc_sem_item(tmp3));
        printf("DEFVAR LF@%s\n", tmp1);
        printf("DEFVAR LF@%s\n", tmp2);
        printf("POPS LF@%s\n", tmp1);
        printf("POPS LF@%s\n", tmp2);
        printf("CONCAT LF@%s LF@%s LF@%s\n", tmp3, tmp1, tmp2);
        printf("PUSHS LF@%s\n", tmp3);
    }
    else
        return ERR_SEM_CPBLT;

    free(op1);
    free(op2);

    return SUCCESS;
}

int convert_type(stack_t* sem_stack, table_item_t *sym_tb, data_t* symb1, data_t* symb2)
{
    int type1 = symb1->data_type;
    int type2 = symb2->data_type;
    if (type1 == FUN)
        type1 = symb1->fun_type;
    if (type2 == FUN)
        type2 = symb2->fun_type;

    if (type1 == INTEGER && type2 == INTEGER)
        return INTEGER;
    else if (type1 == FLOAT && type2 == FLOAT)
        return FLOAT;
    else if (type1 == INTEGER && type2 == FLOAT) 
    {
        printf("INT2FLOAT\n");
        return INTEGER;
    }
    else if (type1 == FLOAT && type2 == INTEGER)
    {
        char* tmp = insert_tmp(sym_tb, INTEGER, NULL);
        stack_push(sem_stack, alloc_sem_item(tmp));
        printf("DEFVAR %s\n", tmp);
        printf("POPS %s\n", tmp);
        printf("INT2FLOAT %s %s\n", tmp, tmp);
        printf("PUSHS %s\n", tmp);
        
        return FLOAT;
    }
    else if (type1 == STRING && type2 == STRING)
        return STRING;
    else
        return -1;
}

int parse_logic_op(table_item_t *sym_tb, stack_t* sem_stack, int logic_op)
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

    int res;
    if (type1 == type2) // no need type conversion
    {
        if (type1 != INTEGER && type1 != FLOAT && type1 != STRING)
            return ERR_SEM_CPBLT;// comparation of nils or bools 

        res = type1;
    }
    else // possible type conversion
    {
        if (type1 == INTEGER && type2 == FLOAT)
            ;// type conversion and then float copmaration
        else if (type1 == FLOAT && type2 == INTEGER)
            ;// type conversion and after that float comparation
        else // not possible type conversion
        {
            if (logic_op != EQUAL || logic_op != NOT_EQUAL)//not possible compar
                return ERR_SEM_CPBLT;

            printf("PUSHS bool@false\n"); //automatic false result
            return SUCCESS;
        }

        res = FLOAT;
    }

    if (logic_op == EQUAL)
        printf("EQS\n");
    else if (logic_op == NOT_EQUAL)
        printf("EQS\nNOTS\n");
    else if (logic_op == LESS)
        printf("LTS\n");
    else if (logic_op == GREATER)
        printf("GTS\n");
    else if (logic_op == LESS_EQ || logic_op == GREATER_EQ)
    {
        char* tmp1 = insert_tmp(sym_tb, res, NULL);
        char* tmp2 = insert_tmp(sym_tb, res, NULL);
        char* tmp3 = insert_tmp(sym_tb, res, NULL);
        printf("DEFVAR LF@%s\n", tmp1);
        printf("DEFVAR LF@%s\n", tmp2);
        printf("DEFVAR LF@%s\n", tmp3);
        printf("POPS LF@%s\n", tmp1);
        printf("POPS LF@%s\n", tmp2);
        if (logic_op == LESS_EQ)
            printf("LT LF@%s LF@%s LF@%s\n", tmp3, tmp2, tmp1);
        else
            printf("GT LF@%s LF@%s LF@%s\n", tmp3, tmp2, tmp1);
        printf("POPS LF@%s\n", tmp3);
        printf("EQ LF@%s LF@%s LF@%s\n", tmp3, tmp2, tmp1);
        printf("PUSHS LF@%s\n", tmp3);
        printf("ORS\n");
    }

    free(op1);
    free(op2);

    return SUCCESS;
}
