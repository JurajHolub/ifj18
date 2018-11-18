/**
 * @author Matej Parobek (xparob00)
 * @file semantic_parser_td.c
 * @date 2018-11-15
 * @brief Implementation of semantic actions used in top-down parsing
 */

#include "semantic_parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int sem_action_fcdef(data_t *ste_ptr_newfc, string_t str_params, data_t **params_array) {
    //TODO prehladat vsetky tabulky
    //detection of variable to function redefinition
    if (NULL != search(get_main_st(), ste_ptr_newfc->id))
    {
        return ERR_SEM_DEF;
    }
    else
    {
        //detection of function redefinition
        data_t *func_in_tab = search(get_fun_st(), ste_ptr_newfc->id);
        if (func_in_tab == NULL)
        {
            insert(get_fun_st(), ste_ptr_newfc);
        }
            //detection of bad parameters number in function call before definition
        else if (func_in_tab->type == UNDEF_FUN)
        {
            if (func_in_tab->param_cnt != ste_ptr_newfc->param_cnt)
            {
                return ERR_SEM_DEF;
            }
            else
            {
                func_in_tab->type = DEF_FUN;
            }
        }

        //checking parameters, if they don't redefine function or other parameter and parsing to function symbol table
        //pointers to symbol table entries of parameters are saved in array for generator
        int i = 0;
        char *param = strtok(str_params->string, " ");
        while (param != NULL)
        {
            //creating symbol table entry
            data_t ste_newparam;

            //inserting data from token to symbol table entry
            ste_newparam.type = VAR;
            ste_newparam.value = UNDEF;
            ste_newparam.id = string_create(param);
            ste_newparam.param_cnt = 0;

            if (NULL != search(get_fun_st(), ste_newparam.id) || NULL != search(get_local_st(), ste_newparam.id))
            {
                return ERR_SEM_DEF;
            }

            insert(get_local_st(), &ste_newparam);
            assert(i < ste_ptr_newfc->param_cnt);
            params_array[i] = search(get_local_st(), ste_newparam.id);

            param = strtok(NULL, " ");
            i++;
        }
        return SUCCESS;
    }
}

int sem_action_callfc(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params, bool accept_undef)
{
    //check function definition
    data_t *found = search(get_fun_st(), ste_ptr_callfc->id);
    if (found != NULL)
    {
        if (strcmp(found->id->string, "print") != 0)
        {
            if (found->param_cnt != ste_ptr_callfc->param_cnt)
            {
                return ERR_SEM_FUN;
            }
        }
    }
    else
    {
        if (accept_undef)
        {
            insert(get_fun_st(), ste_ptr_callfc);
        }
        else
        {
            return ERR_SEM_DEF;
        }
    }

    //check function parameters
    for (int i = 0; i < ste_ptr_callfc->param_cnt; i++)
    {
        if ((params->params[i]).type == VAR)
        {
            if (NULL == search(symtable, (params->params)[i].id))
            {
                return ERR_SEM_DEF;
            }
        }
        else if (((params->params)[i].type == CONST) && ((params->params)[i].value == INTEGER || (params->params)[i].value == STRING ||
                 (params->params)[i].value == FLOAT || (params->params)[i].value == NIL))
        {
            insert(symtable, (params->params) + i);
        }
        else
        {
            return ERR_SEM_ELSE;
        }
    }

    return SUCCESS;
}

