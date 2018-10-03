#include "scanner.h"
#include <stdio.h>

// Some simulation of scanner for testing purposes of parser.  

// int + int * int / int 
token_t tokens[] = {
    {.tab_idx = _integer, .key = "int"},
    {.tab_idx = _add, .key = "+"},
    {.tab_idx = _integer, .key = "int"},
    {.tab_idx = _mul, .key = "*"},
    {.tab_idx = _integer, .key = "int"},
    {.tab_idx = _div, .key = "/"},
    {.tab_idx = _integer, .key = "float"},
    {.tab_idx = _end, .key = ""}
};
int cnt = -1;

// return always constant test aritmetic expression
token_t* get_token()
{
    cnt++;
    if (cnt == 8)
    {
    //    printf("get_token(NULL)\n");
        return NULL;
    }

    //printf("get_token(%s)\n", tokens[cnt].key);
    return &tokens[cnt];
}
