/**
 * @file gen_out.c
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @date November 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Generator of 3AD code.
 */

#include "gen_out.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "error_handle.h"
#include "scanner.h"

// list of global fram instructions
gen_list_t *global_frame = NULL;
// list of actual local frame instructions
gen_list_t *local_frame = NULL;

/***************************** LIBRARY FUNCTION ******************************/
char *lib_fun_inputs =
"###############################################################################\n"
"#                            BEGIN LIB FUN INPUTS\n"
"###############################################################################\n"
"JUMP $inputs$def\n"
"LABEL inputs\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@$inputs$ret\n"
"READ LF@$inputs$ret string\n"
"PUSHS LF@$inputs$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $inputs$def\n"
"###############################################################################\n"
"#                            END LIB FUN INPUTS\n"
"###############################################################################\n";
char *lib_fun_inputi =
"###############################################################################\n"
"#                            BEGIN LIB FUN INPUTI\n"
"###############################################################################\n"
"JUMP $inputi$def\n"
"LABEL inputi\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@$inputi$ret\n"
"READ LF@$inputi$ret int\n"
"PUSHS LF@$inputi$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $inputi$def\n"
"###############################################################################\n"
"#                            END LIB FUN INPUTI\n"
"###############################################################################\n";
char *lib_fun_inputf =
"###############################################################################\n"
"#                            BEGIN LIB FUN INPUTF\n"
"###############################################################################\n"
"JUMP $inputf$def\n"
"LABEL inputf\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@$inputf$ret\n"
"READ LF@$inputf$ret float\n"
"PUSHS LF@$inputf$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $inputf$def\n"
"###############################################################################\n"
"#                            END LIB FUN INPUTF\n"
"###############################################################################\n";

char *lib_fun_lenght =
"###############################################################################\n"
"#                            BEGIN LIB FUN LENGTH\n"
"###############################################################################\n"
"JUMP $length$def\n"
"LABEL length\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@type\n"
"DEFVAR LF@s\n"
"DEFVAR LF@$length$ret\n"
"POPS LF@s\n"
"TYPE LF@type LF@s\n"
"JUMPIFEQ $noerror LF@type string@string\n"
"EXIT int@4\n"
"LABEL $noerror\n"
"STRLEN LF@$length$ret LF@s\n"
"PUSHS LF@$length$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $length$def\n"
"###############################################################################\n"
"#                            END LIB FUN LENGTH\n"
"###############################################################################\n";
char *lib_fun_substr =
"###############################################################################\n"
"#                            BEGIN LIB FUN SUBSTR\n"
"###############################################################################\n"
"JUMP $substr$def\n"
"LABEL substr\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@s\n"
"DEFVAR LF@i\n"
"DEFVAR LF@n\n"
"DEFVAR LF@underflow\n"
"DEFVAR LF@overflow\n"
"DEFVAR LF@wrongN\n"
"DEFVAR LF@len\n"
"DEFVAR LF@err\n"
"DEFVAR LF@$substr$ret\n"
"DEFVAR LF@cnt\n"
"DEFVAR LF@tmp\n"
"DEFVAR LF@limit\n"
"DEFVAR LF@n_limit\n"
"DEFVAR LF@type_s\n"
"DEFVAR LF@type_i\n"
"DEFVAR LF@type_n\n"
"DEFVAR LF@err$string\n"
"DEFVAR LF@err$i\n"
"DEFVAR LF@err$n\n"
"DEFVAR LF@err_n_i\n"
"DEFVAR LF@err_type\n"
"POPS LF@s\n"
"POPS LF@i\n"
"POPS LF@n\n"
"TYPE LF@type_s LF@s\n"
"TYPE LF@type_i LF@i\n"
"TYPE LF@type_n LF@n\n"
"JUMPIFNEQ $substr$convert$i LF@type_i string@float\n"
"FLOAT2INT LF@i LF@i\n"
"LABEL $substr$convert$i\n"
"JUMPIFNEQ $substr$convert$n LF@type_n string@float\n"
"FLOAT2INT LF@n LF@n\n"
"LABEL $substr$convert$n\n"
"TYPE LF@type_i LF@i\n"
"TYPE LF@type_n LF@n\n"
"EQ LF@err$string LF@type_s string@string\n"
"EQ LF@err$i LF@type_i string@int\n"
"EQ LF@err$n LF@type_n string@int\n"
"AND LF@err_n_i LF@err$i LF@err$n\n"
"AND LF@err_type LF@err_n_i LF@err$string\n"
"JUMPIFEQ $substr$no$err LF@err_type bool@true\n"
"EXIT int@4\n"
"LABEL $substr$no$err\n"
"STRLEN LF@len LF@s\n"
"LT LF@underflow LF@i int@0 # i < 0\n"
"GT LF@overflow LF@i LF@len # i > length(s)\n"
"LT LF@wrongN LF@n int@0 # n < 0\n"
"OR LF@err LF@underflow LF@overflow # i < 0 || i > length(s)\n"
"OR LF@err LF@err LF@wrongN # n < 0 || (i < 0 || i > length(s))\n"
"JUMPIFNEQ $substr$no_error LF@err bool@true # n < 0 || (i < 0 || i > length(s))\n"
"PUSHS nil@nil\n"
"RETURN\n"
"LABEL $substr$no_error\n"
"SUB LF@n_limit LF@len LF@i # length(s) - i\n"
"GT LF@limit LF@n LF@n_limit # n > length(s) - i\n"
"JUMPIFEQ $substr$limit_if LF@limit bool@false\n"
"SUB LF@n LF@len LF@i # n = length(s)-i\n"
"LABEL $substr$limit_if\n"
"MOVE LF@cnt int@0\n"
"MOVE LF@$substr$ret string@\n"
"LABEL $substr$forloop\n"
"JUMPIFEQ $substr$hardjmp LF@cnt LF@n\n"
"GETCHAR LF@tmp LF@s LF@i\n"
"CONCAT LF@$substr$ret LF@$substr$ret LF@tmp\n"
"ADD LF@cnt LF@cnt int@1\n"
"ADD LF@i LF@i int@1\n"
"JUMP $substr$forloop\n"
"LABEL $substr$hardjmp\n"
"PUSHS LF@$substr$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $substr$def\n"
"###############################################################################\n"
"#                            END LIB FUN SUBSTR\n"
"###############################################################################\n";
char *lib_fun_ord =
"###############################################################################\n"
"#                            BEGIN LIB FUN ORD \n"
"###############################################################################\n"
"JUMP $ord$def\n"
"LABEL ord\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@s\n"
"DEFVAR LF@i\n"
"DEFVAR LF@$ord$ret\n"
"DEFVAR LF@err\n"
"DEFVAR LF@underflow\n"
"DEFVAR LF@overflow\n"
"DEFVAR LF@len\n"
"DEFVAR LF@type_s\n"
"DEFVAR LF@type_i\n"
"POPS LF@s\n"
"POPS LF@i\n"
"TYPE LF@type_s LF@s\n"
"TYPE LF@type_i LF@i\n"
"JUMPIFNEQ $err$ord LF@type_s string@string\n"
"JUMPIFEQ $ord$ok LF@type_i string@int\n"
"JUMPIFEQ $ord$convert LF@type_i string@float\n"
"LABEL $err$ord\n"
"EXIT int@4\n"
"LABEL $ord$convert\n"
"FLOAT2INT LF@i LF@i\n"
"LABEL $ord$ok\n"
"STRLEN LF@len LF@s\n"
"SUB LF@len LF@len int@1\n"
"LT LF@underflow LF@i int@0\n"
"GT LF@overflow LF@i LF@len\n"
"OR LF@err LF@underflow LF@overflow\n"
"JUMPIFNEQ $ord$no_error LF@err bool@true\n"
"PUSHS nil@nil\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $ord$no_error\n"
"STRI2INT LF@$ord$ret LF@s LF@i\n"
"PUSHS LF@$ord$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $ord$def\n"
"###############################################################################\n"
"#                            END LIB FUN ORD\n"
"###############################################################################\n";
char *lib_fun_chr =
"###############################################################################\n"
"#                            BEGIN LIB FUN CHR \n"
"###############################################################################\n"
"JUMP $chr$def\n"
"LABEL chr\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@i\n"
"DEFVAR LF@$chr$ret\n"
"DEFVAR LF@type\n"
"POPS LF@i\n"
"TYPE LF@type LF@i\n"
"JUMPIFEQ $convert$chr LF@type string@float\n"
"JUMPIFEQ $noerror$chr LF@type string@string\n"
"EXIT int@4\n"
"LABEL $convert$chr\n"
"FLOAT2INT LF@i LF@i\n"
"LABEL $noerror$chr\n"
"INT2CHAR LF@$chr$ret LF@i\n"
"PUSHS LF@$chr$ret\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $chr$def\n"
"###############################################################################\n"
"#                            END LIB FUN CHR\n"
"###############################################################################\n";
char *lib_fun_print =
"###############################################################################\n"
"#                            BEGIN LIB FUN PRINT \n"
"###############################################################################\n"
"JUMP $print$def\n"
"LABEL print\n"
"CREATEFRAME\n"
"PUSHFRAME\n"
"DEFVAR LF@cnt\n"
"DEFVAR LF@s\n"
"DEFVAR LF@i\n"
"DEFVAR LF@out\n"
"POPS LF@cnt\n"
"MOVE LF@i int@0\n"
"MOVE LF@out string@\n"
"LABEL $print$forloop\n"
"JUMPIFEQ $print$hardjmp LF@i LF@cnt\n"
"POPS LF@s\n"
"WRITE LF@s\n"
"ADD LF@i LF@i int@1\n"
"JUMP $print$forloop\n"
"LABEL $print$hardjmp\n"
"PUSHS nil@nil\n"
"POPFRAME\n"
"RETURN\n"
"LABEL $print$def\n"
"###############################################################################\n"
"#                            END LIB FUN PRINT\n"
"##############################################################################\n";

void create_global_frame()
{
    global_frame = gen_list_create();

    add_text(lib_fun_inputs);
    add_text(lib_fun_inputi);
    add_text(lib_fun_inputf);
    add_text(lib_fun_lenght);
    add_text(lib_fun_substr);
    add_text(lib_fun_ord);
    add_text(lib_fun_chr);
    add_text(lib_fun_print);
}

void free_global_frame()
{
    if (global_frame != NULL)
    {
        gen_list_free(global_frame);
        global_frame = NULL;
    }
}

void create_local_frame()
{
    local_frame = gen_list_create();
}

void free_local_frame()
{
    if (local_frame != NULL)
    {
        gen_list_deactive(local_frame);
        gen_list_append(global_frame, local_frame);

        local_frame = NULL;
    }
}

inst_t* inst_create()
{
    inst_t *inst = malloc(sizeof(inst_t));
    if (inst == NULL)
    {
        mem_error();
        return NULL;
    }
    
    inst->instruction = I_UNDEF;
    inst->op1 = NULL;
    inst->op2 = NULL;
    inst->op3 = NULL;
    inst->text = NULL;

    return inst;
}

void inst_free(inst_t *inst)
{
    if (inst != NULL)
    {
        free(inst);
    }
}
void add_instruction(int instruction, data_t **op1, data_t **op2 , data_t **op3)
{
    inst_t *inst = inst_create();
    inst->instruction = instruction;
    
    if (op1 != NULL)
        inst->op1 = *op1;
    else
        inst->op1 = NULL;
    
    if (op2 != NULL)
        inst->op2 = *op2;
    else
        inst->op2 = NULL;

    if (op3 != NULL)
        inst->op3 = *op3;
    else
        inst->op3 = NULL;

    if (local_frame == NULL)
    {
        gen_list_insert_last(global_frame, inst);
    }
    else
        gen_list_insert_last(local_frame, inst);
}

void add_text(char *txt)
{
    inst_t *inst = inst_create();
    inst->instruction = GEN_TEXT;
    inst->op1 = NULL;
    inst->op2 = NULL;
    inst->op3 = NULL;
    inst->text = txt;

    if (local_frame == NULL)
    {
        gen_list_insert_last(global_frame, inst);
    }
    else
        gen_list_insert_last(local_frame, inst);
}

void add_prolog_text(char *txt)
{
    inst_t *inst = inst_create();
    inst->instruction = GEN_TEXT;
    inst->op1 = NULL;
    inst->op2 = NULL;
    inst->op3 = NULL;
    inst->text = txt;

    if (local_frame == NULL)
    {
        if (!gen_list_active(global_frame))
        {
            gen_list_insert_first(global_frame, inst);
            gen_list_first(global_frame);
        }
        else
        {
            gen_list_post_insert(global_frame, inst);
            gen_list_succ(global_frame);
        }
    }
    else
    {
        if (!gen_list_active(local_frame))
        {
            gen_list_insert_first(local_frame, inst);
            gen_list_first(local_frame);
        }
        else
        {
            gen_list_post_insert(local_frame, inst);
            gen_list_succ(local_frame);
        }
    }
}

void add_prolog_inst(int instruction, data_t **op1, data_t **op2 , data_t **op3)
{
    inst_t *inst = inst_create();
    inst->instruction = instruction;
    
    if (op1 != NULL)
        inst->op1 = *op1;
    else
        inst->op1 = NULL;
    
    if (op2 != NULL)
        inst->op2 = *op2;
    else
        inst->op2 = NULL;

    if (op3 != NULL)
        inst->op3 = *op3;
    else
        inst->op3 = NULL;

    if (local_frame == NULL)
    {
        if (!gen_list_active(global_frame))
        {
            gen_list_insert_first(global_frame, inst);
            gen_list_first(global_frame);
        }
        else
        {
            gen_list_post_insert(global_frame, inst);
            gen_list_succ(global_frame);
        }
    }
    else
    {
        if (!gen_list_active(local_frame))
        {
            gen_list_insert_first(local_frame, inst);
            gen_list_first(local_frame);
        }
        else
        {
            gen_list_post_insert(local_frame, inst);
            gen_list_succ(local_frame);
        }
    }
}

void add_var(data_t **var)
{
    inst_t *inst = inst_create();
    inst->instruction = I_DEFVAR;
    inst->op1 = *var;
    inst->op2 = NULL;
    inst->op3 = NULL;
    inst->text = NULL;

    if (local_frame == NULL)
    {
        if (!gen_list_active(global_frame))
            gen_list_insert_first(global_frame, inst);
        else
            gen_list_post_insert(global_frame, inst);
    }
    else
    {
        if (!gen_list_active(local_frame))
            gen_list_insert_first(local_frame, inst);
        else
            gen_list_post_insert(local_frame, inst);
    }
}

void gen_program()
{
    while (!gen_list_empty(global_frame))
    {
        gen_instruction(global_frame->first->inst);
        gen_list_delete_first(global_frame);
    }
}

string_t get_symb(data_t* data)
{
    char id[data->id->strlen+20];

    if (data->type == VAR)
    {
        sprintf(id, "LF@%s", data->id->string);
        return string_create(id);
    }
    else // CONST
    {
        if (data->value == INTEGER)
        {
            sprintf(id, "int@%d", atoi(data->id->string));
            return string_create(id);
        }
        else if (data->value == FLOAT)
        {
            sprintf(id, "float@%a", (double)atof(data->id->string));
            return string_create(id);
        }
        else if (data->value == STRING)
        {
            sprintf(id, "string@%s", data->id->string);
            return string_create(id);
        }
        else if (data->value == BOOL)
        {
            sprintf(id, "bool@%s", data->id->string);
            return string_create(id);
        }
        else
        {
            return string_create("nil@nil");
        }
    }
}

void gen_instruction(inst_t *inst)
{
    string_t op1 = NULL;
    string_t op2 = NULL;
    string_t op3 = NULL;
    if (inst->op1 != NULL)
        op1 = get_symb(inst->op1);
    if (inst->op2 != NULL)
        op2 = get_symb(inst->op2);
    if (inst->op3 != NULL)
        op3 = get_symb(inst->op3);

    switch (inst->instruction)
    {
        case GEN_TEXT:
            printf("%s", inst->text);
            break;
        case I_IFJCODE18:
            printf(".IFJcode18\n");
            break;
        case I_MOVE:
            printf("MOVE %s %s\n", op1->string, op2->string);
            break;
        case I_CREATEFRAME:
            printf("CREATEFRAME\n");
            break;
        case I_PUSHFRAME:
            printf("PUSHFRAME\n");
            break;
        case I_POPFRAME:
            printf("POPFRAME\n");
            break;
        case I_DEFVAR:
            printf("DEFVAR %s\n", op1->string);
            printf("MOVE %s nil@nil\n", op1->string);
            break;
        case I_LT:
            printf("LT %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_GT:
            printf("GT %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_EQ:
            printf("EQ %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_CALL:
            printf("CALL %s\n", inst->op1->id->string);
            break;
        case I_RETURN:
            printf("RETURN\n");
            break;
        case I_ADD:
            printf("ADD %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_SUB:
            printf("SUB %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_MUL:
            printf("MUL %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_DIV:
            printf("DIV %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_IDIV:
            printf("IDIV %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_AND:
            printf("AND %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_OR:
            printf("OR %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_NOT:
            printf("NOT %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_INT2FLOAT:
            printf("INT2FLOAT %s %s\n", op1->string, op2->string);
            break;
        case I_FLOAT2INT:
            printf("FLOAT2INT %s %s\n", op1->string, op2->string);
            break;
        case I_INT2CHAR:
            printf("INT2CHAR %s %s\n", op1->string, op2->string);
            break;
        case I_STRI2INT:
            printf("STRI2INT %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_PUSHS:
            printf("PUSHS %s\n", op1->string);
            break;
        case I_POPS:
            printf("POPS %s\n", op1->string);
            break;
        case I_CLEARS:
            printf("CLEARS\n");
            break;
        case I_ADDS:
            printf("ADDS\n");
            break;
        case I_SUBS:
            printf("SUBS\n");
            break;
        case I_MULS:
            printf("MULS\n");
            break;
        case I_DIVS:
            printf("DIVS\n");
            break;
        case I_IDIVS:
            printf("IDIVS\n");
            break;
        case I_LTS:
            printf("LTS\n");
            break;
        case I_GTS:
            printf("GTS\n");
            break;
        case I_EQS:
            printf("EQS\n");
            break;
        case I_ANDS:
            printf("ANDS\n");
            break;
        case I_ORS:
            printf("ORS\n");
            break;
        case I_NOTS:
            printf("NOTS\n");
            break;
        case I_INT2FLOATS:
            printf("INT2FLOATS\n");
            break;
        case I_FLOAT2INTS:
            printf("FLOAT2INTS\n");
            break;
        case I_INT2CHARS:
            printf("INT2CHARS\n");
            break;
        case I_STRI2INTS:
            printf("STRI2INTS\n");
            break;
        case I_JUMPIFEQS:
            printf("JUMPIFEQS %s\n", inst->op1->id->string);
            break;
        case I_JUMPIFNEQS:
            printf("JUMPIFNEQS %s\n", inst->op1->id->string);
            break;
        case I_CONCAT:
            printf("CONCAT %s %s %s\n", op1->string, op2->string, op3->string);
            break;
        case I_TYPE:
            printf("TYPE %s %s\n", op1->string, op2->string);
            break;
        case I_LABEL:
            printf("LABEL %s\n", inst->op1->id->string);
            break;
        case I_JUMP:
            printf("JUMP %s\n", inst->op1->id->string);
            break;
        case I_JUMPIFEQ:
            printf("JUMPIFEQ %s %s %s\n", inst->op1->id->string, op2->string, op3->string);
            break;
        case I_JUMPIFNEQ:
            printf("JUMPIFNEQ %s %s %s\n", inst->op1->id->string, op2->string, op3->string);
            break;
        case I_EXIT:
            printf("EXIT %s\n", op1->string);
            break;
        case I_BREAK:
            printf("BREAK\n");
            break;
        case I_DPRINT:
            printf("DPRINT\n");
            break;
        case I_UNDEF:
            fprintf(stderr, "Undefined instruction in generator.\n");
            break;
    }

    if (inst->op1 != NULL)
        string_free(op1);
    if (inst->op2 != NULL)
        string_free(op2);
    if (inst->op3 != NULL)
        string_free(op3);
}
