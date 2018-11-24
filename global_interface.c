/**
 * @file global_interface.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @brief Global interface of symtables used for parser and generator.
 * @date November 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#include "stack.h"
#include "global_interface.h"
#include <stdio.h>
#include <stdlib.h>

/***************************** GLOBAL SYMBOL TABLES **************************/
table_item_t* glob_main_st = NULL; // sym tab for main body of program
table_item_t* glob_fun_st = NULL; // sym tab for all functions declarations and calls
// stack of sym tabs for local frames (function body), top of stack is last function
stack_t glob_local_st; 
data_t *expr_type = NULL; // ptr to result of expression saved in some sym tab


table_item_t* get_main_st()
{
    if (glob_main_st == NULL)
    {
        glob_main_st = get_hash_table();
    }
    return glob_main_st;
}

table_item_t* get_fun_st()
{
    if (glob_fun_st == NULL)
    {
        glob_fun_st = get_hash_table();

        data_t inputs;
        inputs.type = DEF_FUN;
        inputs.id = string_create("inputs");
        inputs.data_type = STRING;
        inputs.param_cnt = 0;
        inputs.value = NULL;
        insert(glob_fun_st, &inputs);
        string_free(inputs.id);

        data_t inputi;
        inputi.type = DEF_FUN;
        inputi.id = string_create("inputi");
        inputi.data_type = INTEGER;
        inputi.param_cnt = 0;
        inputi.value = NULL;
        insert(glob_fun_st, &inputi);
        string_free(inputi.id);

        data_t inputf;
        inputf.type = DEF_FUN;
        inputf.id = string_create("inputf");
        inputf.data_type = FLOAT;
        inputf.param_cnt = 0;
        inputf.value = NULL;
        insert(glob_fun_st, &inputf);
        string_free(inputf.id);

        data_t print;
        print.type = DEF_FUN;
        print.id = string_create("print");
        print.data_type = NIL;
        print.value = NULL;
        insert(glob_fun_st, &print);
        string_free(print.id);

        data_t length;
        length.type = DEF_FUN;
        length.id = string_create("length");
        length.data_type = INTEGER;
        length.param_cnt = 1;
        insert(glob_fun_st, &length);
        string_free(length.id);

        data_t substr;
        substr.type = DEF_FUN;
        substr.id = string_create("substr");
        substr.data_type = STRING;
        substr.param_cnt = 3;
        substr.value = NULL;
        insert(glob_fun_st, &substr);
        string_free(substr.id);
        
        data_t ord;
        ord.type = DEF_FUN;
        ord.id = string_create("ord");
        ord.data_type = INTEGER;
        ord.param_cnt = 2;
        ord.value = NULL;
        insert(glob_fun_st, &ord);
        string_free(ord.id);

        data_t chr;
        chr.type = DEF_FUN;
        chr.id = string_create("chr");
        chr.data_type = STRING;
        chr.param_cnt = 1;
        chr.value = NULL;
        insert(glob_fun_st, &chr);
        string_free(chr.id);
    }

    return glob_fun_st;
}

table_item_t* get_local_st()
{
    table_item_t* local_st = stack_top(&glob_local_st);
    return local_st;
}

void set_local_st()
{
    table_item_t *local = get_hash_table();
    stack_push(&glob_local_st, local);
}

data_t *get_expr_type()
{
    return expr_type;
}

void set_expr_type(data_t *expr)
{
    expr_type = expr;
}

void remove_all_st()
{
    if (glob_main_st != NULL)
        destroy_hash_table(glob_main_st);
    if (glob_fun_st != NULL)
        destroy_hash_table(glob_fun_st);

    table_item_t* del;
    while (!stack_empty(&glob_local_st))
    {
        del = stack_top(&glob_local_st);
        stack_pop(&glob_local_st);
        destroy_hash_table(del);
    }
}

