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

int main(int argc, char **argv)
{

    return syntax_tests(argc, argv);

    //string_t var1 = string_create("var0");
    //string_t tmp1 = string_create("$tmp_01"); 

    //table_item_t *hash_tb = get_hash_table();
    //data_t d;
    //d.value = string_create(NULL);
    //d.fun_type = VAR;
    //d.param_cnt = 0;
    //d.param_id = string_create(NULL);
    //d.data_type = VAR;
    //d.id = var1;
    //insert(hash_tb, &d);

    //string_free(d.value);
    //d.value = string_create("10");
    //d.data_type = INTEGER;
    //d.id = tmp1;
    //insert(hash_tb, &d);
    //string_free(d.param_id);
    //string_free(d.value);

    //data_t *op1, *op2;

    //create_global_frame();

    //op1 = search(hash_tb, var1);
    //op2 = search(hash_tb, tmp1);
    //set_instruction(I_MOVE, &op1, &op2, NULL); 

    //create_local_frame();
    //set_instruction(I_MOVE, &op1, &op2, NULL); 
    //free_local_frame();

    //gen_program();

    //free_global_frame();

    //string_free(var1);
    //string_free(tmp1);
    //
    //destroy_hash_table(hash_tb);

    //return syntax_tests(argc, argv);
    return 0;
}
