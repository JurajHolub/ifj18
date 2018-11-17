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
    //print_sem_stack(sem_stack);
    //printf("var: %s[%s]\n", op->attribute->string, get_real_type(op->type));

    data_t *symb = search(sym_tb, op->attribute);

    if (op->type == VAR) //it is variable
    {
        if (symb == NULL) // not initialized variable 
            return ERR_SEM_DEF;
    }
    else // it is constant
    {
        if (symb == NULL)
        {
            data_t cons;
            cons.type = CONST;
            cons.value = op->type;
            cons.id = op->attribute;
            cons.param_cnt = 0;
            
            insert(sym_tb, &cons);
            symb = search(sym_tb, op->attribute);
        }
    }

    add_instruction(I_PUSHS, &symb, NULL, NULL);

    string_t tmp_str = string_create(op->attribute->string);
    
    set_expr_type(symb);
    stack_push(sem_stack, tmp_str);

    return SUCCESS;
}

int sem_action_add(table_item_t *sym_tb, data_t *symb1, data_t *symb2)
{
    string_t str_res = insert_tmp(sym_tb, VAR);
    data_t *res = search(sym_tb, str_res);

    add_text("######################## BEGIN PARNI MLATICKA NA SCITANI\n");
    if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
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
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
        add_instruction(I_LABEL, &end, NULL, NULL);//LABEL $end$of$parni$mlaticka

    } // if (symb1->value == UNDEF || symb2->value == UNDEF)
    else
    {
        if (symb1->value == INTEGER && symb2->value == INTEGER)
        {
            add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
            res->value = INTEGER;
        }
        else if (symb1->value == FLOAT && symb2->value == FLOAT)
        {
            add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
            res->value = FLOAT;
        }
        else if (symb1->value == INTEGER && symb2->value == FLOAT)
        {
            add_instruction(I_INT2FLOATS, NULL, NULL, NULL);//INT2FLOATS
            add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS
            res->value = FLOAT;
        }
        else if (symb1->value == FLOAT && symb2->value == INTEGER)
        {
            string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
            string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
            data_t *tmpop1 = search(sym_tb, str_tmpop1);
            data_t *tmpop2 = search(sym_tb, str_tmpop2);

            add_var(&tmpop1);//DEFVAR LF@$tmp$op1
            add_var(&tmpop2);//DEFVAR LF@$tmp$op2
            add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
            add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
            add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
            add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS

            res->value = FLOAT;
        }
        else if (symb1->value == STRING && symb2->value == STRING)
        {
            string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
            string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
            data_t *tmpop1 = search(sym_tb, str_tmpop1);
            data_t *tmpop2 = search(sym_tb, str_tmpop2);

            add_var(&tmpop1);//DEFVAR LF@$tmp$op1
            add_var(&tmpop2);//DEFVAR LF@$tmp$op2
            add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
            add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
            add_instruction(I_CONCAT, &tmpop1, &tmpop1, &tmpop2);// CONCAT LF@$tmp$op1 LF@$tmp$op1 LF@$tmp$op2
            add_instruction(I_ADDS, NULL, NULL, NULL);//ADDS

            res->value = STRING;
        }
        else
        {
            return ERR_SEM_CPBLT;
        }
    }
    add_text("######################## END PARNI MLATICKA NA SCITANI\n");

    set_expr_type(res);

    return SUCCESS;
}

int sem_action_sub_mul(table_item_t *sym_tb, data_t *symb1, data_t *symb2, int arit_op)
{
    string_t str_res = insert_tmp(sym_tb, VAR);
    data_t *res = search(sym_tb, str_res);

    add_text("######################## BEGIN PARNI MLATICKA NA ODCITANI A NASOBENI\n");
    if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
        int instr = arit_op == SUB ? I_SUB : I_MUL;

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
        data_t *true_const = search(sym_tb, d.id);
        string_free(d.id);
        d.type = CONST;
        d.value = INTEGER;
        d.id = string_create("4");
        insert(sym_tb, &d);
        data_t *const_4 = search(sym_tb, d.id);
        string_free(d.id);

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
        string_t str_first_int_second_float = insert_tmp(sym_tb, VAR);
        string_t str_first_float_second_int = insert_tmp(sym_tb, VAR);
        //LABELS
        string_t str_simple_add = insert_tmp(sym_tb, UNDEF);//$simple_add
        string_t str_convert_first = insert_tmp(sym_tb, UNDEF);//$convert_first
        string_t str_convert_second = insert_tmp(sym_tb, UNDEF);//$convert_second
        string_t str_end = insert_tmp(sym_tb, UNDEF);//$end$of$parni$mlaticka

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
        data_t *first_int_second_float = search(sym_tb, str_first_int_second_float);
        data_t *first_float_second_int = search(sym_tb, str_first_float_second_int);    
        data_t *simple_add = search(sym_tb, str_simple_add);
        data_t *convert_first = search(sym_tb, str_convert_first);
        data_t *convert_second = search(sym_tb, str_convert_second);
        data_t *end = search(sym_tb, str_end);

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
        add_instruction(I_AND, &first_int_second_float, &first_int, &second_float);//AND LF@first_int_second_float LF@first_int LF@second_float
        add_instruction(I_AND, &first_float_second_int, &first_float, &second_int);//AND LF@first_float_second_int LF@first_float LF@second_int
        add_instruction(I_JUMPIFEQ, &simple_add, &same_int_or_float, &true_const);//JUMPIFEQ $simple_add LF@same_int_or_float bool@true
        add_instruction(I_JUMPIFEQ, &convert_first, &first_int_second_float, &true_const);//JUMPIFEQ $convert_first LF@first_int_second_float bool@true
        add_instruction(I_JUMPIFEQ, &convert_second, &first_float_second_int, &true_const);//JUMPIFEQ $convert_second LF@first_float_second_int bool@true
        add_instruction(I_EXIT, &const_4, NULL, NULL);//EXIT int@4
        add_instruction(I_LABEL, &simple_add, NULL, NULL);//LABEL $simple_add
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(instr, NULL, NULL, NULL);//SUBS or MULS
        add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
        add_instruction(I_LABEL, &convert_first, NULL, NULL);//LABEL $convert_first
        add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(instr, NULL, NULL, NULL);//SUBS or MULS
        add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
        add_instruction(I_LABEL, &convert_second, NULL, NULL);//LABEL $convert_second
        add_instruction(I_INT2FLOAT, &tmpop2, &tmpop2, NULL);//INT2FLOAT LF@$tmp$op2 LF@$tmp$op2
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(instr, NULL, NULL, NULL);//SUBS or MULS
        add_instruction(I_LABEL, &end, NULL, NULL);//LABEL $end$of$parni$mlaticka

    }
    else // if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
        if (symb1->value == INTEGER && symb2->value == INTEGER)
        {
            res->value = INTEGER;
        }
        else if (symb1->value == FLOAT && symb2->value == FLOAT)
        {
            res->value = FLOAT;
        }
        else if (symb1->value == INTEGER && symb2->value == FLOAT)
        {
            add_instruction(I_INT2FLOATS, NULL, NULL, NULL);//INT2FLOATS

            res->value = FLOAT;
        }
        else if (symb1->value == FLOAT && symb2->value == INTEGER)
        {
            string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
            string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
            data_t *tmpop1 = search(sym_tb, str_tmpop1);
            data_t *tmpop2 = search(sym_tb, str_tmpop2);

            add_var(&tmpop1);//DEFVAR LF@$tmp$op1
            add_var(&tmpop2);//DEFVAR LF@$tmp$op2
            add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
            add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
            add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2

            res->value = FLOAT;
        }
        else
        {
            return ERR_SEM_CPBLT;
        }

        if (arit_op == SUB)
            add_instruction(I_SUBS, NULL, NULL, NULL);//SUBS
        if (arit_op == MUL)
            add_instruction(I_MULS, NULL, NULL, NULL);//MULS
    }
    add_text("######################## END PARNI MLATICKA NA ODCITANI A NASOBENI\n");

    set_expr_type(res);

    return SUCCESS;
}

int sem_action_div(table_item_t *sym_tb, data_t *symb1, data_t *symb2)
{
    string_t str_res = insert_tmp(sym_tb, VAR);
    data_t *res = search(sym_tb, str_res);

    add_text("######################## BEGIN PARNI MLATICKA NA DELENI\n");
    if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
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
        d.id = string_create("9");
        insert(sym_tb, &d);
        data_t *const_9 = search(sym_tb, d.id);
        string_free(d.id);
        d.id = string_create("0");
        insert(sym_tb, &d);
        data_t *const_int_0 = search(sym_tb, d.id);
        d.value = FLOAT;
        insert(sym_tb, &d);
        data_t *const_float_0 = search(sym_tb, d.id);
        string_free(d.id);

        string_t str_type1 = insert_tmp(sym_tb, VAR);
        string_t str_type2 = insert_tmp(sym_tb, VAR);
        string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
        string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
        string_t str_same = insert_tmp(sym_tb, VAR);
        string_t str_first_int = insert_tmp(sym_tb, VAR);
        string_t str_first_float = insert_tmp(sym_tb, VAR);
        string_t str_second_int = insert_tmp(sym_tb, VAR);
        string_t str_second_float = insert_tmp(sym_tb, VAR);
        string_t str_int_and_int = insert_tmp(sym_tb, VAR);
        string_t str_float_and_float = insert_tmp(sym_tb, VAR);
        string_t str_first_int_second_float = insert_tmp(sym_tb, VAR);
        string_t str_first_float_second_int = insert_tmp(sym_tb, VAR);
        string_t str_second_int_zero = insert_tmp(sym_tb, VAR);
        string_t str_second_float_zero = insert_tmp(sym_tb, VAR);
        //LABELS
        string_t str_convert_first = insert_tmp(sym_tb, UNDEF);//$convert_first
        string_t str_convert_second = insert_tmp(sym_tb, UNDEF);//$convert_second
        string_t str_no_zero_err_0 = insert_tmp(sym_tb, UNDEF);//$zero_err
        string_t str_no_zero_err_1 = insert_tmp(sym_tb, UNDEF);//$zero_err
        string_t str_no_zero_err_2 = insert_tmp(sym_tb, UNDEF);//$zero_err
        string_t str_no_zero_err_3 = insert_tmp(sym_tb, UNDEF);//$zero_err
        string_t str_int_int = insert_tmp(sym_tb, UNDEF);
        string_t str_float_float = insert_tmp(sym_tb, UNDEF);
        string_t str_end = insert_tmp(sym_tb, UNDEF);//$end$of$parni$mlaticka

        data_t *type1 = search(sym_tb, str_type1);
        data_t *type2 = search(sym_tb, str_type2);
        data_t *tmpop1 = search(sym_tb, str_tmpop1);
        data_t *tmpop2 = search(sym_tb, str_tmpop2);
        data_t *same = search(sym_tb, str_same);
        data_t *first_int = search(sym_tb, str_first_int);
        data_t *first_float = search(sym_tb, str_first_float);
        data_t *second_int = search(sym_tb, str_second_int);
        data_t *second_float = search(sym_tb, str_second_float);
        data_t *int_and_int = search(sym_tb, str_int_and_int);
        data_t *float_and_float = search(sym_tb, str_float_and_float);
        data_t *first_int_second_float = search(sym_tb, str_first_int_second_float);
        data_t *first_float_second_int = search(sym_tb, str_first_float_second_int);    
        data_t *convert_first = search(sym_tb, str_convert_first);
        data_t *convert_second = search(sym_tb, str_convert_second);
        data_t *second_int_zero = search(sym_tb, str_second_int_zero);
        data_t *second_float_zero = search(sym_tb, str_second_float_zero);
        data_t *no_zero_err_0 = search(sym_tb, str_no_zero_err_0);
        data_t *no_zero_err_1 = search(sym_tb, str_no_zero_err_1);
        data_t *no_zero_err_2 = search(sym_tb, str_no_zero_err_2);
        data_t *no_zero_err_3 = search(sym_tb, str_no_zero_err_3);
        data_t *int_int = search(sym_tb, str_int_int);
        data_t *float_float = search(sym_tb, str_float_float);
        data_t *end = search(sym_tb, str_end);

        add_var(&type1);//DEFVAR LF@$type1
        add_var(&type2);//DEFVAR LF@$type2
        add_var(&tmpop1);//DEFVAR LF@$tmp$op1
        add_var(&tmpop2);//DEFVAR LF@$tmp$op2
        add_var(&same);//DEFVAR LF@same
        add_var(&first_int);//DEFVAR LF@first_int
        add_var(&first_float);//DEFVAR LF@first_float
        add_var(&second_int);//DEFVAR LF@second_int
        add_var(&second_float);//DEFVAR LF@second_float
        add_var(&int_and_int);//DEFVAR LF@int_or_float
        add_var(&float_and_float);//DEFVAR LF@int_or_float
        add_var(&first_int_second_float);//DEFVAR LF@first_int_second_float
        add_var(&first_float_second_int);//DEFVAR LF@first_float_second_int
        add_var(&second_int_zero);//DEFVAR LF@$second_int_zero
        add_var(&second_float_zero);//DEFVAR LF@$second_float_zero
        
        add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
        add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
        add_instruction(I_TYPE, &type1, &tmpop1, NULL);//TYPE LF@$type1 LF@$tmp$op1
        add_instruction(I_TYPE, &type2, &tmpop2, NULL);//TYPE LF@$type2 LF@$tmp$op2
        add_instruction(I_EQ, &same, &type1, &type2);//EQ LF@same LF@$type1 LF@$type2
        add_instruction(I_EQ, &first_int, &type1, &int_const);//EQ LF@first_int LF@$type1 string@int
        add_instruction(I_EQ, &first_float, &type1, &float_const);//EQ LF@first_float LF@$type1 string@float
        add_instruction(I_EQ, &second_int, &type2, &int_const);//EQ LF@second_int LF@$type2 string@int
        add_instruction(I_EQ, &second_float, &type2, &float_const);//EQ LF@second_float LF@$type2 string@float
        add_instruction(I_AND, &int_and_int, &first_int, &second_int);//OR LF@int_or_float LF@first_int LF@first_float
        add_instruction(I_AND, &float_and_float, &first_float, &second_float);//AND LF@same_int_or_float LF@same LF@int_or_float
        add_instruction(I_AND, &first_int_second_float, &first_int, &second_float);//AND LF@first_int_second_float LF@first_int LF@second_float
        add_instruction(I_AND, &first_float_second_int, &first_float, &second_int);//AND LF@first_float_second_int LF@first_float LF@second_int
        add_instruction(I_JUMPIFEQ, &int_int, &first_int, &second_int);//JUMPIFEQ $simple_add LF@same_int_or_float bool@true
        add_instruction(I_JUMPIFEQ, &float_float, &first_float, &second_float);//JUMPIFEQ $simple_add LF@same_int_or_float bool@true
        add_instruction(I_JUMPIFEQ, &convert_first, &first_int_second_float, &true_const);//JUMPIFEQ $convert_first LF@first_int_second_float bool@true
        add_instruction(I_JUMPIFEQ, &convert_second, &first_float_second_int, &true_const);//JUMPIFEQ $convert_second LF@first_float_second_int bool@true
        add_instruction(I_EXIT, &const_4, NULL, NULL);//EXIT int@4
        add_instruction(I_LABEL, &int_int, NULL, NULL);//LABEL $simple_add
        add_instruction(I_EQ, &second_int_zero, &tmpop2, &const_int_0);// EQ LF@second_int_zero LF@$tmp$op2 int@0 
        add_instruction(I_JUMPIFNEQ, &no_zero_err_1, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
        add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
        add_instruction(I_LABEL, &no_zero_err_1, NULL, NULL);//LABEL $zero_err
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(I_IDIVS, NULL, NULL, NULL);//IDIVS
        add_instruction(I_LABEL, &float_float, NULL, NULL);//LABEL $simple_add
        add_instruction(I_EQ, &second_float_zero, &tmpop2, &const_int_0);// EQ LF@second_int_zero LF@$tmp$op2 int@0 
        add_instruction(I_JUMPIFNEQ, &no_zero_err_0, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
        add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
        add_instruction(I_LABEL, &no_zero_err_0, NULL, NULL);//LABEL $zero_err
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(I_DIVS, NULL, NULL, NULL);//IDIVS
        add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
        add_instruction(I_LABEL, &convert_first, NULL, NULL);//LABEL $convert_first
        add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
        add_instruction(I_EQ, &second_float_zero, &tmpop2, &const_float_0);// EQ LF@second_float_zero LF@$tmp$op2 float@0x0p+0
        add_instruction(I_JUMPIFNEQ, &no_zero_err_2, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
        add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
        add_instruction(I_LABEL, &no_zero_err_2, NULL, NULL);//LABEL $zero_err
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(I_DIVS, NULL, NULL, NULL);//DIVS
        add_instruction(I_JUMP, &end, NULL, NULL);//JUMP $end$of$parni$mlaticka
        add_instruction(I_LABEL, &convert_second, NULL, NULL);//LABEL $convert_second
        add_instruction(I_INT2FLOAT, &tmpop2, &tmpop2, NULL);//INT2FLOAT LF@$tmp$op2 LF@$tmp$op2
        add_instruction(I_EQ, &second_float_zero, &tmpop2, &const_float_0);// EQ LF@second_float_zero LF@$tmp$op2 float@0x0p+0
        add_instruction(I_JUMPIFNEQ, &no_zero_err_3, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
        add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
        add_instruction(I_LABEL, &no_zero_err_3, NULL, NULL);//LABEL $zero_err
        add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
        add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
        add_instruction(I_DIVS, NULL, NULL, NULL);//DIVS
        add_instruction(I_LABEL, &end, NULL, NULL);//LABEL $end$of$parni$mlaticka

    }
    else // if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
        string_t str_tmpop1 = insert_tmp(sym_tb, VAR);
        string_t str_tmpop2 = insert_tmp(sym_tb, VAR);
        string_t str_second_int_zero = insert_tmp(sym_tb, VAR);
        string_t str_second_float_zero = insert_tmp(sym_tb, VAR);
        string_t str_zero_err = insert_tmp(sym_tb, UNDEF);//$zero_err
        data_t *tmpop1 = search(sym_tb, str_tmpop1);
        data_t *tmpop2 = search(sym_tb, str_tmpop2);
        data_t *second_int_zero = search(sym_tb, str_second_int_zero);
        data_t *second_float_zero = search(sym_tb, str_second_float_zero);
        data_t *zero_err = search(sym_tb, str_zero_err);

        data_t d;
        d.param_cnt = 0;
        d.type = CONST;
        d.value = INTEGER;
        d.id = string_create("9");
        insert(sym_tb, &d);
        data_t *const_9 = search(sym_tb, d.id);
        string_free(d.id);
        d.id = string_create("0");
        insert(sym_tb, &d);
        data_t *const_int_0 = search(sym_tb, d.id);
        d.value = FLOAT;
        insert(sym_tb, &d);
        data_t *const_float_0 = search(sym_tb, d.id);
        string_free(d.id);
        d.value = BOOL;
        d.id = string_create("true");
        insert(sym_tb, &d);
        data_t *true_const = search(sym_tb, d.id);
        string_free(d.id);


        add_var(&tmpop1);//DEFVAR LF@$tmp$op1
        add_var(&tmpop2);//DEFVAR LF@$tmp$op2
        add_instruction(I_POPS, &tmpop2, NULL, NULL);//POPS LF@$tmp$op2
        add_instruction(I_POPS, &tmpop1, NULL, NULL);//POPS LF@$tmp$op1
        
        if (symb1->value == INTEGER && symb2->value == INTEGER)
        {
            add_var(&second_int_zero);
            add_instruction(I_EQ, &second_int_zero, &tmpop2, &const_int_0);// EQ LF@second_int_zero LF@$tmp$op2 int@0 
            add_instruction(I_JUMPIFEQ, &zero_err, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
            add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
            add_instruction(I_LABEL, &zero_err, NULL, NULL);//LABEL $zero_err
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
            add_instruction(I_IDIVS, NULL, NULL, NULL);//IDIVS
            
            res->value = INTEGER;
        }
        else if (symb1->value == FLOAT && symb2->value == FLOAT)
        {
            add_var(&second_float_zero);
            add_instruction(I_EQ, &second_float_zero, &tmpop2, &const_float_0);// EQ LF@second_float_zero LF@$tmp$op2 float@0x0p+0
            add_instruction(I_JUMPIFEQ, &zero_err, &second_float_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
            add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
            add_instruction(I_LABEL, &zero_err, NULL, NULL);//LABEL $zero_err
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
            add_instruction(I_DIVS, NULL, NULL, NULL);//DIVS

            res->value = FLOAT;
        }
        else if (symb1->value == FLOAT && symb2->value == INTEGER)
        {
            add_var(&second_float_zero);
            add_instruction(I_INT2FLOAT, &tmpop2, &tmpop2, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
            add_instruction(I_EQ, &second_float_zero, &tmpop2, &const_float_0);// EQ LF@second_float_zero LF@$tmp$op2 float@0x0p+0
            add_instruction(I_JUMPIFEQ, &zero_err, &second_float_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
            add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
            add_instruction(I_LABEL, &zero_err, NULL, NULL);//LABEL $zero_err
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
            add_instruction(I_DIVS, NULL, NULL, NULL);//DIVS

            res->value = FLOAT;
        }
        else if (symb1->value == INTEGER && symb2->value == FLOAT)
        {
            add_var(&second_int_zero);
            add_instruction(I_INT2FLOAT, &tmpop1, &tmpop1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
            add_instruction(I_EQ, &second_int_zero, &tmpop2, &const_int_0);// EQ LF@second_int_zero LF@$tmp$op2 int@0 
            add_instruction(I_JUMPIFNEQ, &zero_err, &second_int_zero, &true_const);// JUMPIFEQ $zero_err LF@zero_div bool@true
            add_instruction(I_EXIT, &const_9, NULL, NULL);//EXIT int@9
            add_instruction(I_LABEL, &zero_err, NULL, NULL);//LABEL $zero_err
            add_instruction(I_PUSHS, &tmpop1, NULL, NULL);//PUSHS LF@$tmp$op1
            add_instruction(I_PUSHS, &tmpop2, NULL, NULL);//PUSHS LF@$tmp$op2
            add_instruction(I_DIVS, NULL, NULL, NULL);//DIVS

            res->value = FLOAT;
        }
        else
        {
            return ERR_SEM_CPBLT;
        }
    }
    add_text("######################## END PARNI MLATICKA NA DELENI\n");

    set_expr_type(res);

    return SUCCESS;
}

int parse_arit_op(table_item_t *sym_tb, stack_t* sem_stack, int arit_op)
{
    int res;
    string_t op1 = stack_top(sem_stack);
    data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    string_t op2 = stack_top(sem_stack);
    data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);

    if (arit_op == ADD)// handle ADD and CONCAT
    {
        res = sem_action_add(sym_tb, symb1, symb2);
    }
    else if (arit_op == SUB || arit_op == MUL) // SUB, MUL
    {
        res = sem_action_sub_mul(sym_tb, symb1, symb2, arit_op);
    }
    else //DIV or IDIV, handle DIV with zero
    {
        res = sem_action_div(sym_tb, symb1, symb2);
    }


    string_t result = string_create(get_expr_type()->id->string);
    stack_push(sem_stack, result);

    return res;
}

void cmp_instr(int logic_op, data_t *op1, data_t *op2, data_t *tmp, bool pop_operands)
{
    if (logic_op == EQUAL)
    {
        add_instruction(I_EQS, NULL, NULL, NULL);// EQS
    }
    else if (logic_op == NOT_EQUAL)
    {
        add_instruction(I_EQS, NULL, NULL, NULL);// EQS
        add_instruction(I_NOTS, NULL, NULL, NULL);// NOTS
    }
    else if (logic_op == LESS)
    {
        add_instruction(I_LTS, NULL, NULL, NULL);// LTS
    }
    else if (logic_op == GREATER)
    {
        add_instruction(I_GTS, NULL, NULL, NULL);// GTS
    }
    else if (logic_op == LESS_EQ)
    {
        if (pop_operands)
        {
            add_instruction(I_POPS, &op2, NULL, NULL);//POPS LF@op2
            add_instruction(I_POPS, &op1, NULL, NULL);//POPS LF@op1
        }
        add_instruction(I_LT, &tmp, &op1, &op2);//LT LF@tmp LF@op1 LF@op2
        add_instruction(I_PUSHS, &tmp, NULL, NULL);//PUSHS LF@tmp
        add_instruction(I_EQ, &tmp, &op1, &op2);//EQ LF@tmp LF@op1 LF@op2
        add_instruction(I_PUSHS, &tmp, NULL, NULL);//PUSHS LF@tmp
        add_instruction(I_ORS, NULL, NULL, NULL);// ORS
    }
    else if (logic_op == GREATER_EQ)
    {
        if (pop_operands)
        {
            add_instruction(I_POPS, &op2, NULL, NULL);//POPS LF@op2
            add_instruction(I_POPS, &op1, NULL, NULL);//POPS LF@op1
        }
        add_instruction(I_GT, &tmp, &op1, &op2);//GT LF@tmp LF@op1 LF@op2
        add_instruction(I_PUSHS, &tmp, NULL, NULL);//PUSHS LF@tmp
        add_instruction(I_EQ, &tmp, &op1, &op2);//EQ LF@tmp LF@op1 LF@op2
        add_instruction(I_PUSHS, &tmp, NULL, NULL);//PUSHS LF@tmp
        add_instruction(I_ORS, NULL, NULL, NULL);// ORS
    }
}

int sem_action_cmp(table_item_t *sym_tb, data_t *symb1, data_t *symb2, int logic_op)
{
    string_t str_res = insert_tmp(sym_tb, VAR);
    data_t *res = search(sym_tb, str_res);

    string_t str_op1 = insert_tmp(sym_tb, VAR);
    string_t str_op2 = insert_tmp(sym_tb, VAR);
    string_t str_tmp = insert_tmp(sym_tb, VAR);
    data_t *op1 = search(sym_tb, str_op1);
    data_t *op2 = search(sym_tb, str_op2);
    data_t *tmp = search(sym_tb, str_tmp);
    add_var(&op1);// DEFVAR LF@op1
    add_var(&op2);// DEFVAR LF@op2
    add_var(&tmp);// DEFVAR LF@tmp

    add_text("######################## BEGIN PARNI MLATICKA COMPARE\n");
    if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
        string_t str_type1 = insert_tmp(sym_tb, VAR);
        string_t str_type2 = insert_tmp(sym_tb, VAR);
        string_t str_same = insert_tmp(sym_tb, VAR);
        string_t str_first_int = insert_tmp(sym_tb, VAR);
        string_t str_first_float = insert_tmp(sym_tb, VAR);
        string_t str_first_string = insert_tmp(sym_tb, VAR);
        string_t str_second_int = insert_tmp(sym_tb, VAR);
        string_t str_second_float = insert_tmp(sym_tb, VAR);
        string_t str_int_and_int = insert_tmp(sym_tb, VAR);
        string_t str_float_and_float = insert_tmp(sym_tb, VAR);
        string_t str_int_and_float = insert_tmp(sym_tb, VAR);
        string_t str_float_and_int = insert_tmp(sym_tb, VAR);
        string_t str_strings = insert_tmp(sym_tb, VAR);
        //LABELS
        string_t str_cmp_int_int = insert_tmp(sym_tb, UNDEF);
        string_t str_cmp_float_float = insert_tmp(sym_tb, UNDEF);
        string_t str_cmp_int_float = insert_tmp(sym_tb, UNDEF);
        string_t str_cmp_float_int = insert_tmp(sym_tb, UNDEF);
        string_t str_cmp_string = insert_tmp(sym_tb, UNDEF);
        string_t str_end = insert_tmp(sym_tb, UNDEF);

        data_t d;
        d.param_cnt = 0;
        d.type = CONST;
        d.value = STRING;
        d.id = string_create("int");
        insert(sym_tb, &d);
        data_t *const_int = search(sym_tb, d.id);
        string_free(d.id);
        d.id = string_create("float");
        insert(sym_tb, &d);
        data_t *const_float = search(sym_tb, d.id);
        string_free(d.id);
        d.id = string_create("string");
        insert(sym_tb, &d);
        data_t *const_string = search(sym_tb, d.id);
        string_free(d.id);
        d.value = BOOL;
        d.id = string_create("true");
        insert(sym_tb, &d);
        data_t *const_true = search(sym_tb, d.id);
        string_free(d.id);
        d.id = string_create("false");
        insert(sym_tb, &d);
        data_t *const_false = search(sym_tb, d.id);
        string_free(d.id);
        d.value = INTEGER;
        d.id = string_create("4");
        insert(sym_tb, &d);
        data_t *const_4 = search(sym_tb, d.id);
        string_free(d.id);

        data_t *type1 = search(sym_tb, str_type1);
        data_t *type2 = search(sym_tb, str_type2);
        data_t *same = search(sym_tb, str_same);
        data_t *first_int = search(sym_tb, str_first_int);
        data_t *first_float = search(sym_tb, str_first_float);
        data_t *first_string = search(sym_tb, str_first_string);
        data_t *second_int = search(sym_tb, str_second_int);
        data_t *second_float = search(sym_tb, str_second_float);
        data_t *int_and_int = search(sym_tb, str_int_and_int);
        data_t *float_and_float = search(sym_tb, str_float_and_float);
        data_t *int_and_float = search(sym_tb, str_int_and_float);
        data_t *float_and_int = search(sym_tb, str_float_and_int);
        data_t *strings = search(sym_tb, str_strings);
        data_t *cmp_int_int = search(sym_tb, str_cmp_int_int);
        data_t *cmp_float_float = search(sym_tb, str_cmp_float_float);
        data_t *cmp_int_float = search(sym_tb, str_cmp_int_float);
        data_t *cmp_float_int = search(sym_tb, str_cmp_float_int);
        data_t *cmp_string = search(sym_tb, str_cmp_string);
        data_t *end = search(sym_tb, str_end);

        add_var(&type1);// DEFVAR LF@type1
        add_var(&type2);// DEFVAR LF@type2
        add_var(&same);// DEFVAR LF@same
        add_var(&first_int);// DEFVAR LF@first_int 
        add_var(&first_float);// DEFVAR LF@first_float
        add_var(&first_string);// DEFVAR LF@first_string
        add_var(&second_int);// DEFVAR LF@second_int 
        add_var(&second_float);// DEFVAR LF@second_float
        add_var(&int_and_int);// DEFVAR LF@int_and_int
        add_var(&float_and_float);// DEFVAR LF@float_and_float
        add_var(&int_and_float);// DEFVAR LF@int_and_float
        add_var(&float_and_int);// DEFVAR LF@float_and_int
        add_var(&strings);// DEFVAR LF@strings

        add_instruction(I_POPS, &op2, NULL, NULL);//POPS LF@op2
        add_instruction(I_POPS, &op1, NULL, NULL);//POPS LF@op1
        add_instruction(I_TYPE, &type1, &op1, NULL);//TYPE LF@type1 LF@op1
        add_instruction(I_TYPE, &type2, &op2, NULL);//TYPE LF@type2 LF@op2
        add_instruction(I_EQ, &same, &type1, &type2);// EQ LF@same LF@type1 LF@type2
        add_instruction(I_EQ, &first_int, &type1, &const_int);// EQ LF@first_int LF@type1 string@int 
        add_instruction(I_EQ, &first_float, &type1, &const_float);// EQ LF@first_float LF@type1 string@float
        add_instruction(I_EQ, &first_string, &type1, &const_string);// EQ LF@first_string LF@type1 string@string
        add_instruction(I_EQ, &second_int, &type2, &const_int);// EQ LF@second_int LF@type2 string@int 
        add_instruction(I_EQ, &second_float, &type2, &const_float);// EQ LF@second_float LF@type2 string@float
        add_instruction(I_AND, &int_and_int, &same, &first_int);// AND LF@int_and_int LF@same LF@first_int
        add_instruction(I_AND, &float_and_float, &same, &first_float);// AND LF@float_and_float LF@same LF@first_float
        add_instruction(I_AND, &int_and_float, &first_int, &second_float);// AND LF@int_and_float LF@first_int LF@second_float
        add_instruction(I_AND, &float_and_int, &first_float, &second_int);// AND LF@float_and_int LF@first_float LF@second_int
        add_instruction(I_AND, &strings, &same, &first_string);// AND LF@strings LF@same LF@first_string
        add_instruction(I_JUMPIFEQ, &cmp_int_int, &int_and_int, &const_true);// JUMPIFEQ cmp_int_int LF@int_and_int bool@true
        add_instruction(I_JUMPIFEQ, &cmp_float_float, &float_and_float, &const_true);// JUMPIFEQ cmp_float_float LF@float_and_float bool@true
        add_instruction(I_JUMPIFEQ, &cmp_int_float, &int_and_float, &const_true);// JUMPIFEQ cmp_int_float LF@int_and_float bool@true
        add_instruction(I_JUMPIFEQ, &cmp_float_int, &float_and_int, &const_true);// JUMPIFEQ cmp_float_int LF@float_and_int bool@true
        add_instruction(I_JUMPIFEQ, &cmp_string, &strings, &const_true);// JUMPIFEQ cmp_string LF@strings bool@true
        if (logic_op == EQUAL)
            add_instruction(I_PUSHS, &const_false, NULL, NULL);// PUSHS bool@false
        else if (logic_op == NOT_EQUAL)
            add_instruction(I_PUSHS, &const_true, NULL, NULL);// PUSHS bool@true
        else
            add_instruction(I_EXIT, &const_4, NULL, NULL);// EXIT int@4
        add_instruction(I_LABEL, &cmp_int_int, NULL, NULL);// LABEL cmp_int_int
        cmp_instr(logic_op, op1, op2, tmp, true);
        add_instruction(I_JUMP, &end, NULL, NULL);// JUMP end
        add_instruction(I_LABEL, &cmp_float_float, NULL, NULL);// LABEL cmp_float_float
        cmp_instr(logic_op, op1, op2, tmp, true);
        add_instruction(I_JUMP, &end, NULL, NULL);// JUMP end
        add_instruction(I_LABEL, &cmp_int_float, NULL, NULL);// LABEL cmp_int_float
        add_instruction(I_POPS, &op2, NULL, NULL);//POPS LF@$op2
        add_instruction(I_POPS, &op1, NULL, NULL);//POPS LF@$op1
        add_instruction(I_INT2FLOAT, &op1, &op1, NULL);// INT2FLOAT LF@op1 LF@op1
        cmp_instr(logic_op, op1, op2, tmp, false);
        add_instruction(I_JUMP, &end, NULL, NULL);// JUMP end
        add_instruction(I_LABEL, &cmp_float_int, NULL, NULL);// LABEL cmp_float_int
        add_instruction(I_INT2FLOATS, NULL, NULL, NULL);// INT2FLOATS
        cmp_instr(logic_op, op1, op2, tmp, true);
        add_instruction(I_JUMP, &end, NULL, NULL);// JUMP end
        add_instruction(I_LABEL, &cmp_string, NULL, NULL);// LABEL cmp_string
        cmp_instr(logic_op, op1, op2, tmp, true);
        add_instruction(I_LABEL, &end, NULL, NULL);// LABEL end
    }
    else // if (symb1->value == UNDEF || symb2->value == UNDEF)
    {
        if (symb1->value == INTEGER && symb1->value == INTEGER)
        {
            cmp_instr(logic_op, op1, op2, tmp, true);
        }
        else if (symb1->value == FLOAT && symb1->value == FLOAT)
        {
            cmp_instr(logic_op, op1, op2, tmp, true);
        }
        else if (symb1->value == FLOAT && symb1->value == INTEGER)
        {
            add_instruction(I_INT2FLOATS, NULL, NULL, NULL);//INT2FLOATS
            cmp_instr(logic_op, op1, op2, tmp, true);
        }
        else if (symb1->value == INTEGER && symb1->value == FLOAT)
        {
            add_var(&op1);//DEFVAR LF@$tmp$op1
            add_var(&op2);//DEFVAR LF@$tmp$op2
            add_instruction(I_POPS, &op2, NULL, NULL);//POPS LF@$tmp$op2
            add_instruction(I_POPS, &op1, NULL, NULL);//POPS LF@$tmp$op1
            add_instruction(I_INT2FLOAT, &op1, &op1, NULL);//INT2FLOAT LF@$tmp$op1 LF@$tmp$op1
            cmp_instr(logic_op, op1, op2, tmp, false);
        }
        else if (symb1->value == STRING && symb1->value == STRING)
        {
            cmp_instr(logic_op, op1, op2, tmp, true);
        }
        else
        {
            if (logic_op == EQUAL)
            {
                data_t d;
                d.param_cnt = 0;
                d.type = CONST;
                d.value = BOOL;
                d.id = string_create("false");
                insert(sym_tb, &d);
                data_t *const_false = search(sym_tb, d.id);
                string_free(d.id);

                add_instruction(I_PUSHS, &const_false, NULL, NULL);// PUSHS bool@false
            }
            else if (logic_op == NOT_EQUAL)
            {
                data_t d;
                d.param_cnt = 0;
                d.type = CONST;
                d.value = BOOL;
                d.id = string_create("true");
                insert(sym_tb, &d);
                data_t *const_true = search(sym_tb, d.id);
                string_free(d.id);

                add_instruction(I_PUSHS, &const_true, NULL, NULL);// PUSHS bool@true
            }
            else
                return ERR_SEM_CPBLT;
        }
    }
    add_text("######################## END PARNI MLATICKA COMPARE\n");

    res->value = BOOL;
    set_expr_type(res);

    return SUCCESS;
}

int parse_logic_op(table_item_t *sym_tb, stack_t* sem_stack, int logic_op)
{
    int res;
    string_t op1 = stack_top(sem_stack);
    data_t *symb1 = search(sym_tb, op1);
    stack_pop(sem_stack);
    string_t op2 = stack_top(sem_stack);
    data_t *symb2 = search(sym_tb, op2);
    stack_pop(sem_stack);

    res = sem_action_cmp(sym_tb, symb1, symb2, logic_op);

    string_t result = string_create(get_expr_type()->id->string);
    stack_push(sem_stack, result);

    return res;
}
