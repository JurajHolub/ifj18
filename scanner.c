#include "scanner.h"
#include <stdio.h>

// Some simulation of scanner for testing purposes of parser.  

// (float == int) != (int + int * int / float ) 
token_t tokens[] = {
    //{.tab_idx = _integer, .key = "int"},
    {.type = INTEGER, .attribute = "37"},
    {.type = ADD, .attribute = ""},
    {.type = INTEGER, .attribute = "40"},
    //{.tab_idx = _less, .key = "<"},
    //{.tab_idx = _mul, .key = "*"},
    //{.tab_idx = _integer, .key = "int"},
    //{.tab_idx = _less, .key = "<"},
    //{.tab_idx = _integer, .key = "int"},
    //{.tab_idx = _leftB, .key = "("},
    //{.tab_idx = _integer, .key = "int"},
    //{.tab_idx = _add, .key = "*"},
    //{.tab_idx = _integer, .key = "int"},
    //{.tab_idx = _integer, .key = "int"},
    //{.tab_idx = _rightB, .key = ")"},
    //{.tab_idx = _div, .key = "/"},
    //{.tab_idx = _float, .key = "float"},
    //{.tab_idx = _rightB, .key = ")"},
    {.type = LINE_END, .attribute = ""}
};
int cnt = -1;

// return always constant test aritmetic expression
token_t* get_token()
{
    cnt++;

    //printf("get_token(%s)\n", tokens[cnt].key);
    return &tokens[cnt];
}
