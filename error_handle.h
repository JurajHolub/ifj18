#ifndef _ERROR_H_IFJ_18_
#define _ERROR_H_IFJ_18_

/**
 * @file error_handle.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @brief Error handling of compiler.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */


#define SUCCESS 0
#define ERR_LEX 1
#define ERR_SYNTAX 2
#define ERR_SEM_DEF 3
#define ERR_SEM_CPBLT 4
#define ERR_SEM_FUN 5
#define ERR_SEM_ELSE 6
#define ERR_DIV 9
#define ERR_COMPILER 99

/**
 * @brief Print malloc fail message.
 */
void mem_error();

#endif // _ERROR_H_IFJ_18_
