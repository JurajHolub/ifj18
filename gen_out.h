#ifndef _GEN_OUT_H_IFJ_18_
#define _GEN_OUT_H_IFJ_18_

#include "symtable.h"

enum instruction_e {
    I_MOVE,
    I_CREATEFRAME,
    I_PUSHFRAME,
    I_POPFRAME,
    I_DEFVAR,
    I_CALL,
    I_RETURN,
    I_PUSHS,
    I_POPS,
    I_CLEARS,
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_IDIV,
    I_ADDS,
    I_SUBS,
    I_MULS,
    I_DIVS,
    I_IDIVS,
    I_LT,
    I_GT,
    I_EQ,
    I_LTS,
    I_GTS,
    I_EQS,
    I_AND,
    I_OR,
    I_NOT,
    I_ANDS,
    I_ORS,
    I_NOTS,
    I_INT2FLOAT,
    I_FLOAT2INT,
    I_INT2CHAR,
    I_STRI2INT,
    I_INT2FLOATS,
    I_FLOAT2INTS,
    I_INT2CHARS,
    I_STRI2INTS,
    I_READ,
    I_WRITE,
    I_CONCAT,
    I_STRLEN,
    I_GETCHAR,
    I_SETCHAR,
    I_TYPE,
    I_LABEL,
    I_JUMP,
    I_JUMPIFEQ,
    I_JUMPIFNEQ,
    I_JUMPIFEQS,
    I_JUMPIFNEQS,
    I_BREAK,
    I_DPRINT
};

typedef struct inst_s {
    int instruction;
    data_t *op1;
    data_t *op2;
    data_t *op3;
} inst_t;

inst_t* inst_create();
void inst_free(inst_t *inst);

void set_instruction(int instruction, data_t **op1, data_t **op2 , data_t **op3);
void set_var(data_t **var);

void create_global_frame();
void free_global_frame();
void create_local_frame();
void free_local_frame();

void gen_program();
void gen_instruction(inst_t *inst);

/*

void MOVE(char *var, char *symb);
void CREATEFRAME();
void PUSHFRAME();
void POPFRAME();
void DEFVAR(char *var);
void CALL(char *label);
void RETURN();

void PUSHS(char *var, char *symb);
void POPS(char *var);
void CLEARS();

void ADD(char *var, char *symb1, char *symb2);
void SUB(char *var, char *symb1, char *symb2);
void MUL(char *var, char *symb1, char *symb2);
void DIV(char *var, char *symb1, char *symb2);
void IDIV(char *var, char *symb1, char *symb2);

void ADDS();
void SUBS();
void MULS();
void DIVS();
void IDIVS();

void LT(char *var, char *symb1, char *symb2);
void GT(char *var, char *symb1, char *symb2);
void EQ(char *var, char *symb1, char *symb2);

void LTS();
void GTS();
void EQS();

void AND(char *var, char *symb1, char *symb2);
void OR(char *var, char *symb1, char *symb2);
void NOT(char *var, char *symb1, char *symb2);

void ANDS();
void ORS();
void NOTS();

void INT2FLOAT(char *var, char *symb);
void FLOAT2INT(char *var, char *symb);
void INT2CHAR(char *var, char *symb);
void STRI2INT(char *var, char *symb1, char *symb2);

void INT2FLOATS();
void FLOAT2INTS();
void INT2CHARS();
void STRI2INTS();

void READ(char *var, char *type);
void WRITE(char *symb);


printf("PUSHS %s", value);
printf("POPS %s", var);
*/

#endif // _GEN_OUT_H_IFJ_18_
