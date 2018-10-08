#ifndef _GEN_OUT_H_IFJ_18_
#define _GEN_OUT_H_IFJ_18_

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

#endif // _GEN_OUT_H_IFJ_18_
