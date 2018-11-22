/**
 * @file gen_out.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @date November 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Generator of 3AD code.
 */

#ifndef _GEN_OUT_H_IFJ_18_
#define _GEN_OUT_H_IFJ_18_

#include "symtable.h"

/**
 * @brief Enum of all instruction which can be generated.
 */
enum instruction_e {
    I_IFJCODE18, // .IFJcode18
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
    I_EXIT,
    I_BREAK,
    I_DPRINT,
    GEN_TEXT, // any text (e.g. commentary or lib fun)
    I_UNDEF
};

/**
 * @brief One instruction in list of all instruction fo program. List will be
 * generated linear as is.
 */
typedef struct inst_s {
    int instruction; ///< Type of instruction.
    data_t *op1; ///< Data from symbol table for operand of instruction.
    data_t *op2; ///< Data from symbol table for operand of instruction.
    data_t *op3; ///< Data from symbol table for operand of instruction.
    char *text; ///< Special message to generate (typicaly used for comments).
} inst_t;

/**
 * @brief Create dynamicly one instruction for generator and initialize it.
 */
inst_t* inst_create();

void inst_free(inst_t *inst);

/**
 * @brief Add one instruction to the end of generated program. If actual frame
 * is globale -> instruction is at the end of main body of program. If actual
 * frame is local (function body) -> instruction is at the end of function body.
 * @pre First must be called create_global_frame().
 * @param instruction Type of instruction (one of enum instruction_e).
 * @param op1 First operand of 3 addres code. If instruction has less then 1
 * operand then parameter is set to NULL. Else it is ptr to item in symbol table.
 * @param op2 Second operand of 3 addres code. If instruction has less then 2
 * operand then parameter is set to NULL. Else it is ptr to item in symbol table.
 * @param op3 Third operand of 3 addres code. If instruction has less then 3
 * operand then parameter is set to NULL. Else it is ptr to item in symbol table.
 */
void add_instruction(int instruction, data_t **op1, data_t **op2 , data_t **op3);
/**
 * @brief Add one variable at the start of actual frame (global or local, it
 * depends where you call it). Only instruction in actual frame whitch will be
 * still before this variable are instruction added with add_prolog_inst().
 */
void add_var(data_t **var);
/**
 * @brief Add instruction at the start of actual frame (global or local, it
 * depends where you call it). Instruction added to generator with this function
 * will be always at the begining of actual frame body (also before definitions
 * of all variables), typical use: CREATEFRAME, PUSHFRAME, LABEL ...
 */
void add_prolog_inst(int instruction, data_t **op1, data_t **op2 , data_t **op3);
/**
 * @brief Add any text at the start of actual frame. Behave like add_prolog_inst().
 * @pre Expect not dynamicly allocated chars.
 */
void add_prolog_text(char *txt);
/**
 * @brief Add any text at the end of actual frame. Behave like add_instruction().
 * @pre Expect not dynamicly allocated chars.
 */
void add_text(char *txt);
/**
 * @brief Create frame for main body of program. After call of this function,
 * every instruction added to generator through set_instruction() will be
 * generated as a part of main body.
 * @pre Must be called only 1 time (at the start of adding instruction to
 * generator).
 */
void create_global_frame();
/**
 * @brief Destroy list of instruction for main body. It is called only 1 time,
 * after succes code generating.
 */
void free_global_frame();
/**
 * @brief Create local frame (frame for body of function). After call of this
 * function every instruction added to generator with set_instruction will be
 * generated as a part of this function body.
 * @post Expect call of free_local_frame() at the end of function body.
 */
void create_local_frame();
/**
 * @brief End of local frame. Add whole local frame to suitable place in main
 * body of program.
 */
void free_local_frame();

/**
 * @brief Generate whole program.
 */
void gen_program();

/**
 * @brief Generate 1 instruction.
 */
void gen_instruction(inst_t *inst);

#endif // _GEN_OUT_H_IFJ_18_
