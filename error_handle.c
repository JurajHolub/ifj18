/**
 * @file error_handle.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @brief Error handling of compiler.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include "error_handle.h"

void mem_error()
{
    fprintf(stderr, "Error: memory not allocated.\n");
}
