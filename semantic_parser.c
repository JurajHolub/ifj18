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
#include "gen_out.h"
#include "global_interface.h"

int parse_operand(table_item_t *sym_tb, stack_t* sem_stack, token_t *op)
{
    print_sem_stack(sem_stack);
    printf("var: %s[%s]\n", op->attribute->string, get_real_type(op->type));

    data_t *symb = search(sym_tb, op->attribute);

    if (op->type == VAR) //it is variable
    {
        if (symb == NULL) // not initialized variable 
            return ERR_SEM_DEF;
    }
    else // it is constant
    {
        printf("is is constant\n");
        if (symb == NULL)
        {
            data_t cons;
            cons.type = CONST;
            cons.value = op->type;
            cons.id = op->attribute;
            cons.param_cnt = 0;
            
            insert(sym_tb, &cons);
            symb = search(sym_tb, op->attribute);
            printf("%s\n", symb->id->string);
        }
    }

    add_instruction(I_PUSHS, &symb, NULL, NULL);

    string_t tmp_str = string_create(op->attribute->string);
    printf("%s\n", tmp_str->string);
    
    set_expr_type(symb);
    stack_push(sem_stack, tmp_str);

    return SUCCESS;
}

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, int arit_op)
{
    //string_t op1 = stack_top(sem_stack);
    //data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    //string_t op2 = stack_top(sem_stack);
    //data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);

    //int type1, type2;

    data_t d;
    d.type = CONST;
    d.value = STRING;
    d.id = string_create("int");
    d.param_cnt = 0;
    insert(sym_tb, &d);
    data_t *int_const = search(sym_tb, d.id);
    string_free(d.id);
    d.id = string_create("float");
    insert(sym_tb, &d);
    data_t *float_const = search(sym_tb, d.id);
    string_free(d.id);
    d.id = string_create("string");
    insert(sym_tb, &d);
    data_t *string_const = search(sym_tb, d.id);
    string_free(d.id);
    d.type = CONST;
    d.value = BOOL;
    d.id = string_create("true");
    insert(sym_tb, &d);
    data_t *true_const = search(sym_tb, d.id);
    string_free(d.id);
    d.type = CONST;
    d.value = INTEGER;
    d.id = string_create("4");
    insert(sym_tb, &d);
    data_t *const_4 = search(sym_tb, d.id);
    string_free(d.id);

    string_t str_res = insert_tmp(sym_tb, VAR);
    string_t str_type1 = insert_tmp(sym_tb, VAR);
    string_t str_type2 = insert_tmp(sym_tb, VAR);
    string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
    string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
    string_t str_same = insert_tmp(sym_tb, VAR);
    string_t str_first_int = insert_tmp(sym_tb, VAR);
    string_t str_first_float = insert_tmp(sym_tb, VAR);
    string_t str_second_int = insert_tmp(sym_tb, VAR);
    string_t str_second_float = insert_tmp(sym_tb, VAR);
    string_t str_int_or_float = insert_tmp(sym_tb, VAR);
    string_t str_same_int_or_float = insert_tmp(sym_tb, VAR);
    string_t str_same_first_string = insert_tmp(sym_tb, VAR);
    string_t str_first_int_second_float = insert_tmp(sym_tb, VAR);
    string_t str_first_float_second_int = insert_tmp(sym_tb, VAR);
    //LABELS
    string_t str_simple_add = insert_tmp(sym_tb, UNDEF);//$simple_add
    string_t str_string_concat = insert_tmp(sym_tb, UNDEF);//$string_concat
    string_t str_convert_first = insert_tmp(sym_tb, UNDEF);//$convert_first
    string_t str_convert_second = insert_tmp(sym_tb, UNDEF);//$convert_second
    string_t str_end = insert_tmp(sym_tb, UNDEF);//$end$of$parni$mlaticka

    data_t *res = search(sym_tb, str_res);
    data_t *type1 = search(sym_tb, str_type1);
    data_t *type2 = search(sym_tb, str_type2);
    data_t *tmpop1 = search(sym_tb, str_tmpop1);
    data_t *tmpop2 = search(sym_tb, str_tmpop2);
    data_t *same = search(sym_tb, str_same);
    data_t *first_int = search(sym_tb, str_first_int);
    data_t *first_float = search(sym_tb, str_first_float);
    data_t *second_int = search(sym_tb, str_second_int);
    data_t *second_float = search(sym_tb, str_second_float);
    data_t *int_or_float = search(sym_tb, str_int_or_float);
    data_t *same_int_or_float = search(sym_tb, str_same_int_or_float);
    data_t *same_first_string = search(sym_tb, str_same_first_string);
    data_t *first_int_second_float = search(sym_tb, str_first_int_second_float);
    data_t *first_float_second_int = search(sym_tb, str_first_float_second_int);    
    data_t *simple_add = search(sym_tb, str_simple_add);
    data_t *string_concat = search(sym_tb, str_string_concat);
    data_t *convert_first = search(sym_tb, str_convert_first);
    data_t *convert_second = search(sym_tb, str_convert_second);
    data_t *end = search(sym_tb, str_end);


    
    add_text("######################## BEGIN PARNI MLATICKA\n");
    add_var(&type1);//DEFVAR LF@$type1
    add_var(&type2);//DEFVAR LF@$type2
    add_var(&tmpop1);//DEFVAR LF@$tmp$op1
    add_var(&tmpop2);//DEFVAR LF@$tmp$op2
    add_var(&same);//DEFVAR LF@same
    add_var(&first_int);//DEFVAR LF@first_int
    add_var(&first_float);//DEFVAR LF@first_float
    add_var(&second_int);//DEFVAR LF@second_int
    add_var(&second_float);//DEFVAR LF@second_float
    add_var(&int_or_float);//DEFVAR LF@int_or_float
    add_var(&same_int_or_float);//DEFVAR LF@same_int_or_float
    add_var(&same_first_string);//DEFVAR LF@same_first_string
    add_var(&first_int_second_float);//DEFVAR LF@first_int_second_float
    add_var(&first_float_second_int);//DEFVAR LF@first_float_second_int
    
    add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
    add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
    add_instruction(I_TYPE, &type1, &tmpop1, NULL);//TYPE LF@$type1 LF@$tmp$op1
    add_instruction(I_TYPE, &type2, &tmpop2, NULL);//TYPE LF@$type2 LF@$tmp$op2
    add_instruction(I_EQ, &same, &type1, &type2);//EQ LF@same LF@$type1 LF@$type2
    add_instruction(I_EQ, &first_int, &type1, &int_const);//EQ LF@first_int LF@$type1 string@int
    add_instruction(I_EQ, &first_float, &type1, &float_const);//EQ LF@first_float LF@$type1 string@float
    add_instruction(I_EQ, &second_int, &type2, &int_const);//EQ LF@second_int LF@$type2 string@int
    add_instruction(I_EQ, &second_float, &type2, &float_const);//EQ LF@second_float LF@$type2 string@float
    add_instruction(I_OR, &int_or_float, &first_int, &first_float);//OR LF@int_or_float LF@first_int LF@first_float
    add_instruction(I_AND, &same_int_or_float, &same, &int_or_float);//AND LF@same_int_or_float LF@same LF@int_or_float
    add_instruction(I_EQ, &same_first_string, &type1, &string_const);//EQ LF@same_first_string LF@$type1 string@string
    add_instruction(I_AND, &same_first_string, &same_first_string, &same);//AND LF@same_first_string LF@same_first_string LF@same
    add_instruction(I_AND, &first_int_second_float, &first_int, &second_float);//AND LF@first_int_second_float LF@first_int LF@second_float
    add_instruction(I_AND, &first_float_second_int, &first_float, &second_int);//AND LF@first_float_second_int LF@first_float LF@second_int
    add_instruction(I_JUMPIFEQ, &simple_add, &same_int_or_float, &true_const);//JUMPIFEQ $simple_add LF@same_int_or_float bool@true
    add_instruction(I_JUMPIFEQ, &string_concat, &same_first_string, &true_const);//JUMPIFEQ $string_concat LF@same_first_string bool@true
    add_instruction(I_JUMPIFEQ, &convert_first, &first_int_second_float, &true_const);//JUMPIFEQ $convert_first LF@first_int_second_float bool@true
    add_instruction(I_JUMPIFEQ, &convert_second, &first_float_second_int, &true_const);//JUMPIFEQ $convert_second LF@first_float_second_int bool@true
    add_instruction(I_EXIT, &const_4, NULL, NULL);//EXIT int@4
    add_instruction(I_LABEL, &simple_add, NULL, NULL);//LABEL $simple_add
    add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
    add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
    add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
    add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
    add_instruction(I_LABEL, &string_concat, NULL, NULL);//LABEL $string_concat
    add_instruction(I_CONCAT, &tmpop1, &tmpop1, &tmpop2);//CONCAT LF@$tmp$op1 LF@$tmp$op1 LF@$tmp$op2
    add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
    add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
    add_instruction(I_LABEL, &convert_first, NULL, NULL);//LABEL $convert_first
    add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
    add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
    add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
    add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
    add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
    add_instruction(I_LABEL, &convert_second, NULL, NULL);//LABEL $convert_second
    add_instruction(I_INT2FLOAT, &tmpop2, &tmpop2, NULL);//INT2FLOAT LF@$tmp$op2 LF@$tmp$op2
    add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op1
    add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
    add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
    add_instruction(I_LABEL, &end, NULL, NULL);//LABEL $end$of$parni$mlaticka
    add_text("######################## END PARNI MLATICKA\n");

    /*
    if (symb1->value == INTEGER && symb2->value == INTEGER 
                                || 
        symb1->value == FLOAT   && symb2->value == FLOAT)
    {
        if (arit_op == ADD)
            //ADDS
        else if (arit_op == SUB)
            //SUBS
        else if (arit_op == MUL)
            //MULS
        else if (arit_op == DIV && symb1->value == INTEGER)
            //IDIVS
        else if (arit_op == DIV && symb1->value == FLOAT)
            //DIVS
    }
        // ADDS, SUBS, MULS, DIVS
    if (symb1->value == STRING && symb2->value == STRING && arit_op == ADD)
    {
        // DEFVAR LF@$tmp$op1
        // DEFVAR LF@$tmp$op2
        // POPS LF$tmp$op2
        // POPS LF$tmp$op1
        // CONCAT LF@$tmp$op1 LF@$tmp$op1 LF@$tmp$op2
        // PUSHS LF@$tmp$op1
    }
    if (symb1->value == INTEGER && symb2->value == FLOAT) //convert first
    {
        // DEFVAR LF@$tmp$op1
        // DEFVAR LF@$tmp$op2
        // POPS LF$tmp$op2
        // POPS LF$tmp$op1
        // INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
        // PUSHS LF@$tmp$op1
        // PUSHS LF@$tmp$op2

        if (arit_op == ADD)
            //ADDS
        else if (arit_op == SUB)
            //SUBS
        else if (arit_op == MUL)
            //MULS
        else if (arit_op == DIV)
            //DIVS
    }
    if (symb1->value == FLOAT && symb2->value == INTEGER) //convert second
    {
        // INT2FLOATS
        if (arit_op == ADD)
            //ADDS
        else if (arit_op == SUB)
            //SUBS
        else if (arit_op == MUL)
            //MULS
        else if (arit_op == DIV)
            //DIVS
    }

    bool first_undef = symb1->type==VAR && symb1->value==UNDEF;
    bool second_undef = symb2->type==VAR && symb2->value==UNDEF;
    
    // one or more undefined variables
    if (first_undef || second_undef)
    {
        // DEFVAR LF@$type
        // DEFVAR LF@$tmp$op1
        // DEFVAR LF@$tmp$op2
        // POPS LF$tmp$op2
        // POPS LF$tmp$op1
        
        // TYPE LF@$type1 LF@tmp$op1
        // TYPE LF@$type2 LF@tmp$op2
        

        // ADD
        bool same =  (type1 == type2);
        bool first_int = type1 == int;
        bool second_int = type1 == int;
        bool first_float = type1 == float;
        bool second_float = type1 == float;
        bool first_string = type1 == string;
        bool second_string = type1 == string;
        if (same && first_int || first_float)
            //ADDS
        if (same && first_string)
            // concat
        if (first_int && second_float)
            // convert first to float
            // ADDS
        if (first_float && second_int)
            //convert second to float
            // ADDS
        else ERROR


        bool string_concat = (symb1->value==STRING || symb2->value==STRING);
        bool one_int = (symb1->value == INTEGER || symb2->value == INTEGER);
        bool one_float = (symb1->value == FLOAT|| symb2->value == FLOAT);

        // PUSHS LF@$tmp$op1
        // PUSHS LF@$tmp$op2

    } // if (first_undef || second_undef)
    else
    {
        if ((symb1->type == INTEGER && symb2->type == INTEGER)
                                    ||
           (symb1->type == FLOAT    && symb2->type == FLOAT))
            ;//ADDS
        else if ((symb1->type == STRING && symb2->type == STRING)
            ;//CONCAT
        else if (symb1->type == FLOAT && symb2->type == INTEGER)
            ;//CONVEERT 2. ADDS
        else if (symb1->type == INTEGER && symb2->type == FLOAT)
            ;//CONVERT 1. ADDS
        else
            return ERR_SEM_CPBLT; 
    }

    if (!string_concat && arit_op == ADD)
    {
        //ADDS
        suc = true;
    }
    else if (arit_op == SUB)
    {
        //SUBS
        suc = true;
    }
    else if (arit_op == MUL)
    {
        //MULS
        suc = true;
    }
    else if (arit_op == DIV && symb2->value == INTEGER)
    {
        //IDIVS
        suc = true;
    }
    else if (arit_op == DIV && symb2->value == FLOAT)
    {
        //DIVS
        suc = true;
    }

        string_t tmp1 = insert_tmp(sym_tb, STRING, op1);
        string_t tmp2 = insert_tmp(sym_tb, STRING, op2);
        string_t tmp3 = insert_tmp(sym_tb, STRING, tmp2);
        stack_push(sem_stack, tmp3);
        
    string_free(op1);
    string_free(op2);
*/

    set_expr_type(res);

    return SUCCESS;
}

int convert_type(stack_t* sem_stack, table_item_t *sym_tb, data_t* symb1, data_t* symb2)
{
    /*
    int type1 = symb1->type;
    int type2 = symb2->type;
    int value1 = symb1->value;
    int value2 = symb2->value;

    if (symb1->type == VAR)
    {
        if (symb1->value = UNDEF)
            ;
    }
    else // CONST
    {
        
    }
*/
    return 1;
}
//        // DIV
//        bool same =  (type1 == type2);
//        bool first_int = type1 == int;
//        bool second_int = type1 == int;
//        bool first_float = type1 == float;
//        bool second_float = type1 == float;
//        if (same && first_int)
//            // IDIVS
//        if (same && first_float)
//            // DIVS
//        if (first_int && second_float)
//            // convert first to float
//            // DIVS
//        if (first_float && second_int)
//            // convert second to float
//            // DIVS
//        else ERROR
//
//        //MUL, SUB
//        bool same =  (type1 == type2);
//        bool first_int = type1 == int;
//        bool second_int = type1 == int;
//        bool first_float = type1 == float;
//        bool second_float = type1 == float;
//        if (same && first_int || first) else ERROR
//        if (first_int && second_float)
//            // convert first to float
//            // DIVS
//        if (first_float && second_int)
//            //convert second to float
//            // DIVS

int parse_logic_op(table_item_t *sym_tb, stack_t* sem_stack, int logic_op)
{
    string_t op1 = stack_top(sem_stack);
    data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    string_t op2 = stack_top(sem_stack);
    data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);
    string_free(op1);
    string_free(op2);

    if (symb1 == NULL || symb2 == NULL) // not declared variable
        return ERR_SEM_DEF;

    /*
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
    {   //type conversion and then float copmaration
        if (type1 == INTEGER && type2 == FLOAT)
        {
            string_t tmp = insert_tmp(sym_tb, FLOAT, NULL);
            stack_push(sem_stack, tmp);
            printf("DEFVAR %s\n", tmp->string);
            printf("POPS %s\n", tmp->string);
            printf("INT2FLOAT %s %s\n", tmp->string, tmp->string);
            printf("PUSHS %s\n", tmp->string);
        } // type conversion and after that float comparation
        else if (type1 == FLOAT && type2 == INTEGER)
            printf("INT2FLOATS\n");
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
        string_t tmp1 = insert_tmp(sym_tb, res, NULL);
        string_t tmp2 = insert_tmp(sym_tb, res, NULL);
        string_t tmp3 = insert_tmp(sym_tb, res, NULL);
        printf("DEFVAR LF@%s\n", tmp1->string);
        printf("DEFVAR LF@%s\n", tmp2->string);
        printf("DEFVAR LF@%s\n", tmp3->string);
        printf("POPS LF@%s\n", tmp1->string);
        printf("POPS LF@%s\n", tmp2->string);
        if (logic_op == LESS_EQ)
            printf("LT LF@%s LF@%s LF@%s\n", tmp3->string, tmp2->string, tmp1->string);
        else
            printf("GT LF@%s LF@%s LF@%s\n", tmp3->string, tmp2->string, tmp1->string);
        printf("PUSHS LF@%s\n", tmp3->string);
        printf("EQ LF@%s LF@%s LF@%s\n", tmp3->string, tmp2->string, tmp1->string);
        printf("PUSHS LF@%s\n", tmp3->string);
        printf("ORS\n");
    }
*/
    return SUCCESS;
}
