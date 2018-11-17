/**
 * @file main.c
 * @author(s) Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @date September 2018
 * @brief Dummy hello world of project.
 */

#include <stdio.h>
#include "expressions_parser.h"
#include "scanner.h"
#include "symtable.h"
#include <stdlib.h>
#include "semantic_parser.h"
#include "dynamic_string.h"
#include "test.h"
#include "gen_out.h"
#include "list.h"
#include "global_interface.h"
#include "error_handle.h"

int main(int argc, char **argv)
{
    
    //table_item_t* hash_tb = get_main_st();
    //create_global_frame();

    //data_t d;
    //d.id = string_create("10");
    //d.type = CONST;
    //d.value = INTEGER;
    //d.param_cnt = 0;
    //insert(hash_tb, &d);
    //data_t* var1 = search(hash_tb, d.id);
    //string_free(d.id);
    //add_var(&var1);

    //d.id = string_create("20");
    //d.type = CONST;
    //d.value = INTEGER;
    //d.param_cnt = 0;
    //insert(hash_tb, &d);
    //data_t* var2 = search(hash_tb, d.id);
    //string_free(d.id);
    //add_var(&var2);

    //add_prolog_inst(I_IFJCODE18, NULL, NULL, NULL);
    //add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    //add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);

    //add_instruction(I_POPFRAME, NULL, NULL, NULL);

    create_global_frame();

    add_prolog_inst(I_IFJCODE18, NULL, NULL, NULL);
    add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);

    //creating symbol table entry for var int for testing purposes
    data_t *ste_ptr_test = malloc(sizeof(data_t));
    //inserting data from token to symbol table entry
    ste_ptr_test->type = CONST;
    ste_ptr_test->value = INTEGER;
    ste_ptr_test->id = string_create("tmp4");
    ste_ptr_test->param_cnt = 0;

    set_expr_type(ste_ptr_test);

    int ret = syntax_tests(argc, argv);

    add_instruction(I_POPFRAME, NULL, NULL, NULL);

    if (ret == SUCCESS)
        gen_program();

    free_global_frame();
    free(ste_ptr_test);
    remove_all_st();

    return ret;
}
