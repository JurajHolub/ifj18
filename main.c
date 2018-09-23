/**
 * @file main.c
 * @author(s) Juraj Holub <xholub40@stud.fit.vutbr.cz>
 * @date September 2018
 * @brief Dummy hello world of project.
 */

#include <stdio.h>

/**
 * @brief Some dumy code style example function witch print input parameter.
 * @param str Printed string.
 */
void dummy(char *str)
{
    printf("%s", str);
}

int main(int argc, char **argv)
{
    dummy("Hello world!\n");

    return 1;
}
