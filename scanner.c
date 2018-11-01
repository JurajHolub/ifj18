#include "scanner.h"
#include <stdio.h>

// Some simulation of scanner for testing purposes of parser.  

token_t t;
int cnt = -1;

struct tmp_t {
    char* attribute;
    int type;
};

struct tmp_t tokens[] = {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},//-
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
};

// return always constant test aritmetic expression
token_t* get_token()
{
    if (cnt != -1)
        string_free(t.attribute);
    cnt++;
    t.type = tokens[cnt].type;
    t.attribute = string_create(tokens[cnt].attribute);
    //printf("get_token(%s)\n", t.attribute->string);
    return &t;
//    cnt++;
//
//    //printf("get_token(%s)\n", tokens[cnt].key);
//    return &tokens[cnt];
}
