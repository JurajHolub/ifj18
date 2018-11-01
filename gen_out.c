#include "gen_out.h"
#include <stdio.h>
#include <stdlib.h>

void MOVE(char *frame, char *var, char *,char *symb)
{
    printf("MOVE LF@%s LF@%s\n", var, symb)
}

void MOVE(data_t *var, data_t *symb)
{
    printf("MOVE ");
    VAR(var);
    printf(" ");
    VAR(symb);
    printf("\n");
}

void ADDS()
{
    printf("ADDS\n");
}

void VAR(data_t *var)
{
    if (symb->is_const) // it is constant
    {
        switch (symb->data_type)
        {   //we use atio,atof because we are sure there is valid value
            case INTEGER: 
                printf("int@%d", atoi(symb->val)); 
                break;
            case FLOAT: 
                printf("float@%e", atof(symb->val));
                break;
            case STRING: 
                printf("string@%s", symb->val);
                break;
            case NIL: 
                printf("nil@nil");
                break;
        }
    }
    else    // it is variable
    {
        printf("LF@%s", symb->id);
    }
}
