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

int main(int argc, char **argv)
{
    
    //call of scanner (not implemented yet, so it return hard coded values)
    // testing parser of arithmetic and logic expressions

    table_item_t *hash_tb = get_hash_table();
    data_t d;
    d.value = string_create(NULL);
    d.fun_type = VAR;
    d.param_cnt = 0;
    d.param_id = string_create(NULL);
    d.data_type = INTEGER;
    d.id = string_create("var0");
    insert(hash_tb, &d);
    string_free(d.id);
    d.id = string_create("var1");
    insert(hash_tb, &d);
    string_free(d.id);
    string_free(d.value);
    string_free(d.param_id);

    printf("Parse succes: %d\n", parse_expression(hash_tb));

    destroy_hash_table(hash_tb);

    

    return 0;
}
