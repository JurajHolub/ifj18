/**
 * @file main.c
 * @project Compiler of language IFJ18. School project for subjects IFJ and IAL.
 * @author(s) Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @date September 2018
 * @brief Program main, execute syntax directed analyse and free all dynamic
 * allocated sources.
 */

#include <stdio.h>
#include "expressions_parser.h"
#include "scanner.h"
#include "symtable.h"
#include <stdlib.h>
#include "semantic_parser.h"
#include "dynamic_string.h"
#include "gen_out.h"
#include "list.h"
#include "global_interface.h"
#include "error_handle.h"
#include "top_down_parser.h"

int main(int argc, char **argv)
{
    
    create_global_frame();

    add_prolog_inst(I_IFJCODE18, NULL, NULL, NULL);
    add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);
    
    int ret = parse();
    
    add_instruction(I_POPFRAME, NULL, NULL, NULL);

    if (ret == SUCCESS)
        gen_program();

    free_global_frame();

    remove_all_st();

    free_scanner();

    return ret;
}
