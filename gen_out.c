#include "gen_out.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "error_handle.h"
#include "scanner.h"

gen_list_t *global_frame = NULL;
gen_list_t *local_frame = NULL;

void create_global_frame()
{
    global_frame = gen_list_create();
}

void free_global_frame()
{
    gen_list_free(global_frame);
}

void create_local_frame()
{
    local_frame = gen_list_create();
}

void free_local_frame()
{
    gen_list_append(global_frame, local_frame);
    local_frame = NULL;
}

inst_t* inst_create()
{
    inst_t *inst = malloc(sizeof(inst_t));
    if (inst == NULL)
    {
        mem_error();
        return NULL;
    }
    
    inst->op1 = NULL;
    inst->op2 = NULL;
    inst->op3 = NULL;

    return inst;
}

void set_instruction(int instruction, data_t **op1, data_t **op2 , data_t **op3)
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

void set_var(data_t *var)
{
    inst_t *inst = inst_create();
    inst->instruction = I_DEFVAR;
    inst->op1 = var;
    inst->op2 = NULL;
    inst->op3 = NULL;

    if (local_frame == NULL)
        gen_list_insert_first(global_frame, inst);
    else
        gen_list_insert_first(local_frame, inst);
}

void gen_program()
{
    while (!gen_list_empty(global_frame))
    {
        gen_instruction(global_frame->first->inst);
        gen_list_delete_first(global_frame);
    }
}

char* symb_type(data_t* data)
{
    if (data->data_type == INTEGER)
        return "int";
    else if (data->data_type == FLOAT)
        return "float";
    else if (data->data_type == STRING)
        return "string";
    else
        return "nil";
}

void gen_instruction(inst_t *inst)
{
    switch (inst->instruction)
    {
        case I_MOVE:
            printf("MOVE LF@%s %s@%s\n", inst->op1->id->string, symb_type(inst->op1), inst->op2->id->string);
        case I_CREATEFRAME:
        case I_PUSHFRAME:
        case I_POPFRAME:
        case I_DEFVAR:
        case I_CALL:
        case I_RETURN:
        case I_PUSHS:
        case I_POPS:
        case I_CLEARS:
        case I_ADD:
        case I_SUB:
        case I_MUL:
        case I_DIV:
        case I_IDIV:
        case I_ADDS:
        case I_SUBS:
        case I_MULS:
        case I_DIVS:
        case I_IDIVS:
        case I_LT:
        case I_GT:
        case I_EQ:
        case I_LTS:
        case I_GTS:
        case I_EQS:
        case I_AND:
        case I_OR:
        case I_NOT:
        case I_ANDS:
        case I_ORS:
        case I_NOTS:
        case I_INT2FLOAT:
        case I_FLOAT2INT:
        case I_INT2CHAR:
        case I_STRI2INT:
        case I_INT2FLOATS:
        case I_FLOAT2INTS:
        case I_INT2CHARS:
        case I_STRI2INTS:
        case I_READ:
        case I_WRITE:
            ;
    }
}

/*
void MOVE(char *frame, char *var, char *,char *symb)
{
    printf("MOVE LF@%s LF@%s\n", var, symb)
}

void MOVE(data_t *var, data_t *symb)
{
    printf("MOVE ");
    VAR(var);
    printf(" ");
    VAR(symb);
    printf("\n");
}

void ADDS()
{
    printf("ADDS\n");
}

void VAR(data_t *var)
{
    if (symb->is_const) // it is constant
    {
        switch (symb->data_type)
        {   //we use atio,atof because we are sure there is valid value
            case INTEGER: 
                printf("int@%d", atoi(symb->val)); 
                break;
            case FLOAT: 
                printf("float@%e", atof(symb->val));
                break;
            case STRING: 
                printf("string@%s", symb->val);
                break;
            case NIL: 
                printf("nil@nil");
                break;
        }
    }
    else    // it is variable
    {
        printf("LF@%s", symb->id);
    }
}
*/
