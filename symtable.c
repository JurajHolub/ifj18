/**
 * @file symtable.h
 * @brief Declaration symbol table implemented by hash table.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "error_handle.h"


void print_table(table_item_t* table)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        printf("table[%d]=", i);
        for (list_t *j =table[i].head; j!=NULL; j = j->next)
        {
            printf("%s, ", j->data->id);
        }
        printf("\n");
    }
}

table_item_t* get_hash_table()
{
    table_item_t *table = calloc(HASH_SIZE, sizeof(table_item_t));
    if (table == NULL)
    {
        mem_error();
        return NULL;
    }

    for (int i = 0; i <HASH_SIZE;i++)
        table[i].head = NULL;

    return table;
}

void destroy_hash_table(table_item_t* hash_table)
{
    list_t *list;
    
    for (int i = 0; i < HASH_SIZE; i++)
    {
        list = hash_table[i].head;
        while (list != NULL)
            list_destroy_first(&list);
    }

    free(hash_table);
}

void list_destroy_first(list_t **list)
{
    list_t *deleted = *list;
    *list = deleted->next;
    data_destroy(deleted->data);
    free(deleted);
}

void data_destroy(data_t *data)
{
    if (data->id != NULL)
        free(data->id);
    if (data->value != NULL)
        free(data->value);
    if (data->param_id != NULL)
        free(data->param_id);
    
    free(data);
}

unsigned long hash_fun(char *key)
{
    unsigned long hash = 5381;
    
    for (int i = 0; key[i] != '\0'; i++)
        hash = ((hash << 5) + hash) + key[i];

    return hash;
}

void insert(table_item_t *table, data_t *data)
{
    if (table == NULL)
        return;

    unsigned long hash = hash_fun(data->id);
    int idx = hash % HASH_SIZE;

    if (table[idx].head == NULL)
        table[idx].head = list_insert_first(data);
    else
        table[idx].head = list_insert(table[idx].head, data);
}

char* insert_tmp(table_item_t *table, int type, char *value)
{
    static int tmp_count = 0; // one counter for all tmp variables => not colisions
    
    char *id = malloc(sizeof(char)*16);
    if (id == NULL)
    {
        mem_error();
        return NULL;
    }

    sprintf(id, "$tmp_%010d", tmp_count);
    tmp_count++;

    data_t data;
    data.data_type = type;
    data.id = id;
    data.value = value;
    data.fun_type = VAR;
    data.param_cnt = 0;
    data.param_id = NULL;

    insert(table, &data);

    data_t * ret = search(table, id);

    free(id);

    return ret->id;
}

data_t* search(table_item_t *table, char *key)
{
    if (table == NULL)
        return NULL;

    unsigned long hash = hash_fun(key);
    int idx = hash % HASH_SIZE;

    data_t *data = list_search(table[idx].head, key);
    if (data == NULL) // search faild, should not happend
        return NULL;

    return data;
}

data_t* list_search(list_t *list, char *key)
{
    for (list_t *i = list; i != NULL; i = i->next)
        if (strcmp(i->data->id, key) == 0)
            return i->data;

    return NULL;
}

list_t* list_insert_first(data_t *data) 
{
    list_t *new = malloc(sizeof(list_t));
    if (new == NULL)
    {
        mem_error();
        return NULL;
    }
    new->data = data_copy(data);
    new->next = NULL;

    return new;
}       

list_t* list_insert(list_t *list, data_t *data) 
{
    list_t *new = malloc(sizeof(list_t));
    if (new == NULL)
    {
        mem_error();
        return list;
    }
    else
    {
        new->data = data_copy(data);
        new->next = list;
        return new;
    }
}        

char* cpy_string(char *src)
{
    if (src == NULL)
        return NULL;

    char* dst = malloc(sizeof(char)*strlen(src));
    if (dst == NULL)
    {
        mem_error();
    }
    else
    {
        strcpy(dst, src);
    }

    return dst;
}

data_t* data_copy(data_t *src)
{
    data_t* dst = malloc(sizeof(data_t));
    if (dst == NULL)
    {
        mem_error();
    }

    dst->data_type = src->data_type;
    dst->id = cpy_string(src->id);
    dst->value =  cpy_string(src->value);
    dst->fun_type = src->fun_type;
    dst->param_cnt = src->param_cnt;
    dst->param_id = cpy_string(src->param_id);

    return dst;
}
