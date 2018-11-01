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

/**
 * @brief Constructor of string.
 * @param str Input ptr to string witch will be converted.
 * @return Dynamicly allocated string.
 */
string_t string_create(char *str);
/**
 * @brief Destructor of string. Free allocated string and his metadata.
 * @param str Destroyed string.
 */
void string_free(string_t str);

/**
 * @brief Replace one string by other.
 * @param str Destination string
 * @param replace Source string.
 * @pre Expect string created by string_create()
 */
void string_replace(string_t str, string_t replace);
/**
 * @brief Replace one string by char*.
 * @param str Destination string
 * @param replace Source char*.
 * @pre Expect string created by string_create()
 */
void string_replace_ch(string_t str, char *replace);
/**
 * @brief Append strings.
 * @param str Destination string
 * @param append Appended string.
 * @pre Expect string created by string_create()
 */
void string_append(string_t str, string_t append);
/**
 * @brief Append char* to string.
 * @param str Destination string
 * @param append Appended char*.
 * @pre Expect string created by string_create()
 */
void string_append_ch(string_t str, char *append);
//
//TODO string_t string_split(string_t str);

/**
 * @brief Compare two strings and return true if they are equal.
 * @pre Expect strings created by string_create()
 */
bool string_is_equal(string_t str1, string_t str2);
/**
 * @brief Check if string is empty -> "". String cannot be NULL if there is no
 * value it is empty string "".
 * @pre Expect strings created by string_create()
 */
bool string_is_empty(string_t str);

/**
 * @ return Value of string in char*.
 */
char* string_get(string_t str);

#endif // _DYNAMIC_STRING_H_IFJ_18_
