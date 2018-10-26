/**
 * @file dynamic_string.h
 * @project Compiler of language IFJ18. School project from subjects IFJ and IAL.
 * @brief Dynamic string for eazier handle of C strings.
 * @date October 2018
 * @author Juraj Holub <xholub40@stud.fit.vutbr.cz>
 */

#ifndef _DYNAMIC_STRING_H_IFJ_18_
#define _DYNAMIC_STRING_H_IFJ_18_

#include <stdbool.h>

#define STRING_BLOCK 10

typedef struct string_s {
    char *string; ///< Value of string.
    int strlen; ///< Actual lenght of string.
    int alloclen; ///< Allocated size of string.
} *string_t;

string_t string_create(char *str);
void string_free(string_t str);

void string_replace(string_t str, string_t replace);
void string_replace_ch(string_t str, char *replace);
void string_append(string_t str, string_t append);
void string_append_ch(string_t str, char *append);
//string_t string_split(string_t str);

bool string_is_equal(string_t str1, string_t str2);
bool string_is_empty(string_t str);

char* string_get(string_t str);

#endif // _DYNAMIC_STRING_H_IFJ_18_
