/**
 * @file list.c
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @brief Declaration of list used in generating of output code.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "error_handle.h"

gen_list_t* gen_list_create()
{
    gen_list_t *list = malloc(sizeof(gen_list_t));
    if (list == NULL)
    {
        mem_error();
        return NULL;
    }
    
    list->first = NULL;
    list->act = NULL;
    list->last = NULL;

    return list;
}

void gen_list_insert_first(gen_list_t *list, inst_t *inst)
{
    gen_item_t *item = malloc(sizeof(gen_item_t));
    if (item == NULL)
    {
        mem_error();
        return;
    }

    if (list->first == NULL)
        list->last = item;

    item->next = list->first;
    list->first = item;

    item->inst = inst;
}

void gen_list_insert_last(gen_list_t *list, inst_t *inst)
{
    gen_item_t *item = malloc(sizeof(gen_item_t));
    if (item == NULL)
    {
        mem_error();
        return;
    }

    if (list->last == NULL)
    {
        list->first = item;
        list->last = item;
    }
    else
    {
        list->last->next = item;
        list->last = item;
    }

    item->next = NULL;

    item->inst = inst;
}

void gen_list_append(gen_list_t *dst, gen_list_t *appended)
{
    if (appended->first != NULL)
    {
        if (dst->first == NULL)
        {
            dst->first = appended->first;
        }
        if (dst->last == dst->act)
            dst->act->next = appended->first;

        dst->last->next = appended->first;
        dst->last = appended->last;
    }

    free(appended);
}

void gen_list_delete_first(gen_list_t *list)
{
    if (list->first != NULL)
    {

        gen_item_t *deleted = list->first;
        if (list->first == list->last)
            list->last = NULL;
        if (list->first == list->act)
            list->act = NULL;
        list->first = deleted->next;
        
        inst_free(deleted->inst);
        free(deleted);
    }
}

bool gen_list_empty(gen_list_t *list)
{
    return list->first == NULL;    
}

void gen_list_free(gen_list_t *list)
{
    while (!gen_list_empty(list))
    {
        gen_list_delete_first(list);
    }

    free(list);
    list = NULL;
}

void gen_list_first(gen_list_t *list)
{
    list->act = list->first;
}
void gen_list_succ(gen_list_t *list)
{
    if (list->act != NULL)
        list->act = list->act->next;
}
void gen_list_post_insert(gen_list_t *list, inst_t *inst)
{
    if (list->act != NULL)
    {
        gen_item_t *item = malloc(sizeof(gen_item_t));
        if (item == NULL)
        {
            mem_error();
            return;
        }

        if (list->last == list->act)
        {
            list->last = item;
        }

        item->next = list->act->next;
        list->act->next = item;

        item->inst = inst;
    }
}

bool gen_list_active(gen_list_t *list)
{
    return list->act != NULL;
}

void gen_list_deactive(gen_list_t *list)
{
    list->act = NULL;
}
