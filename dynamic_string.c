/**
 * @file dynamic_string.c
 * @project Compiler of language IFJ18. School project for subjects IFJ and IAL.
 * @brief Dynamic string for eazier handle of C strings.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#include "dynamic_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error_handle.h"


string_t string_create(char *str)
{
    int size;

    string_t string = malloc(sizeof(struct string_s));
    if (string == NULL)
    {
        free(string);
        mem_error();
        return NULL;
    }

    if (str == NULL)
    {
        size = STRING_BLOCK;
        string->strlen = 1;
    }
    else
    {
        string->strlen = strlen(str) + 1;
        size = (string->strlen / STRING_BLOCK + 1) * STRING_BLOCK;
    }

    char* new_str = malloc(sizeof(char)*size);
    if (new_str == NULL)
    {
        free(string);
        free(str);
        mem_error();
        return NULL;
    }

    string->string = new_str;
    string->alloclen = size;

    if (str == NULL)
        string->string[0] = '\0';
    else
        strcpy(string->string, str);

    return string;
}

void string_free(string_t str)
{
    free(str->string);
    free(str);
}

void string_replace(string_t str, string_t replace)
{
    //must realloc
    if (replace->strlen > str->alloclen)
    {
        str->string = realloc(str->string, replace->alloclen);
        if (str->string == NULL)
        {
            mem_error();
            return;
        }
        strcpy(str->string, replace->string);
    }
    else //enough space
    {
        strcpy(str->string, replace->string);
    }
}

void string_replace_ch(string_t str, char *replace)
{
    if (strlen(replace) < str->alloclen)
    {
        strcpy(str->string, replace);
    }
    else
    {
        string_t new = string_create(replace);
        string_replace(str, new);
        string_free(new);
    }
}

void string_append(string_t str, string_t append)
{
    //must realloc
    if (str->alloclen < (str->strlen + append->strlen))
    {
        int size = (append->strlen / STRING_BLOCK + 1) * STRING_BLOCK + str->alloclen;
        str->string = realloc(str->string, size);
        str->alloclen = size;
        
        if (str->string == NULL)
        {
            mem_error();
            return;
        }

        strcat(str->string, append->string);
    }
    else //enough space
    {
        strcat(str->string, append->string);
    }

    str->strlen += append->strlen;
}

void string_append_ch(string_t str, char *append)
{
    if (strlen(append)+str->strlen < str->alloclen)
    {
        strcat(str->string, append);
        str->strlen += strlen(append);
    }
    else
    {
        string_t new = string_create(append);
        string_append(str, new);
        string_free(new);
    }
}

/*
string_t string_split(string_t str)
{
    int shift;
    char tmp[str->strlen];
    for (shift = 0; shift < str->strlen; shift++)
    {
        if (str->string[shift] != ' ')
        {
            tmp[shift] = str->string[shift];
        }
        else 
        {
            tmp[shift] = '\0';
            shift++;
            break;
        }
    }

    for (int i = shift; i < str->strlen; i++)
        str->string[i-shift] = str->string[i];

    return string_create(tmp);
}
*/

char* string_get(string_t str)
{
    return str->string;
}

bool string_is_equal(string_t str1, string_t str2)
{
    return strcmp(str1->string, str2->string) == 0;
}

bool string_is_empty(string_t str)
{
    return str->string[0] == '\0';
}
