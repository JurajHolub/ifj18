/**
 * @file main.c
 * @author(s) Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @date September 2018
 * @brief Dummy hello world of project.
 */

#include <stdio.h>
#include "stack.h"
#include "expressions_parser.h"
#include "scanner.h"

int main(int argc, char **argv)
{
    
    //call of scanner (not implemented yet, so it return hard coded values)
    token_t *token = get_token(); 
    // testing parser of arithmetic and logic expressions
    printf("Parse succes: %d\n", parse_expression(token));

    return 0;
}
