/**
 * @author Matej Parobek (xparob00)
 * @file semantic_parser_td.c
 * @date 2018-11-15
 * @brief Implementation of semantic actions used in top-down parsing
 */

#include "semantic_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include "error_handle.h"
#include "global_interface.h"
#include "gen_out.h"

int sem_action_assig(table_item_t *vars_symtable, data_t *l_value_entry)
{
    //getting function symbol table
    table_item_t *func_symtable = get_fun_st();

    //assigned L value identifier can not be identical with identifier of declared function
    if (NULL != search(func_symtable, l_value_entry->id))
    {
        return ERR_SEM_DEF;
    }

    //TODO check bool

    //inserting l value in symbol table and generating variable definition if the variable is new
    data_t *aux_symtable_entry = search(vars_symtable, l_value_entry->id);
    if (aux_symtable_entry == NULL)   //l value variable is not in symbol table and must be created
    {
        insert(vars_symtable, l_value_entry);

        //generating variable definition
        aux_symtable_entry = search(vars_symtable, l_value_entry->id);
        add_var(&aux_symtable_entry);
    }
    else    //l value variable is in symbol table and will be updated
    {
        aux_symtable_entry->value = l_value_entry->value;
        aux_symtable_entry->type = l_value_entry->type;
    }

    return SUCCESS;
}