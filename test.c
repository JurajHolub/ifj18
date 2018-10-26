/**
 * @file test.c
 * @author Matej Parobek(xparob00)
 * @date 2019-10-25
 */

#include "test.h"
#include "top_down_parser.h"

/**
 * @brief supported lexemes on stdin
 *
 * structure used to check input to program
 */
static char *lexemes[] = {"not", "-", "+", "*", "/", "!=", "<", "int", "str", "nil", "(", ")", "eol", "==", ">",
                          "<=", ">=", "float", "def", "do", "else", "end", "if", "then", "while", "=", "FUN", ",",
                          "id", "EOF"};

/**
 * number of supported lexemes
 *
 * auxiliary constant for work with lexemes array
 */
const int lexemes_nm = 30;

token_t *tokens[2] = {NULL, NULL};
int returned_token = 0;

void test_of_test(void);

/**
 * @brief auxiliary function clear dynamic allocated tokens and its attributes
 */
void clean(void)
{
    if (tokens[0] != NULL)
    {
        if (tokens[0]->attribute == NULL)
        {
            free((void *)tokens[0]->attribute);
        }
        free((void *)tokens[1]);
    }
    if (tokens[1] != NULL)
    {
        if (tokens[1]->attribute == NULL)
        {
            free((void *)tokens[1]->attribute);
        }
        free((void *)tokens[2]);
    }
}

int main()
{
    //parse();

    test_of_test();
    clean();
    return 0;
}

token_t *get_token(void)
{
    //max 2 tokens can be returned
    assert(returned_token >= 0 && returned_token <= 2);
    //if token was returned from scanner, there is no need to get new token. We return last token the function got
    //saved in global array
    if (returned_token > 0)
    {
        //we change the tokens positions, because token on position 1 is last token the function got
        //on position 0 is the token stored in case, that scanner returns 2 tokens
        token_t *tmp = tokens[1];
        tokens[1] = tokens[0];
        tokens[0] = tmp;
        //decrement number of returned_tokens, because last returned token will be used now
        returned_token--;
        //last returned token will be given to parser
        return tmp;
    }
    //there are no returned tokens, so the oldest saved token is deleted and on its place is saved last token and
    //on the place of last token will be saved new token in next steps
    if (tokens[0] != NULL)
    {
        if (tokens[0]->attribute == NULL)
        {
            free((void *)tokens[0]->attribute);
        }
        free((void *)tokens[0]);
    }
    tokens[0] = tokens[1];
    //allocation of new token
    tokens[1] = (token_t *)malloc(sizeof(token_t));
    if (tokens[1] == NULL)
    {
        fprintf(stderr, "Malloc fail\n");
        clean();
        exit(69);
    }
    //data will be achieved and inserted in token
    read_token(tokens[1]);
    return tokens[1];
}

void ret_token(token_t *token)
{
    //dummy check if no invalid token is returned
    assert(token != NULL);
    //counter of returned tokens incrementation
    returned_token++;
}

void read_token(token_t *token)
{
    //counter in lexemes array (test.h)
    int i;
    //longest lexeme, that program support and can read from stdin
    char input[6];

    printf("Gimme lexeme:  ");
    //Function don't end until correct lexeme is read
    while (1)
    {
        //getting lexeme from stdin (max 5 characters without \0) and searching in array of supported lexemes
        scanf("%5s", input);
        for (i = 0; i < lexemes_nm; i++)
        {
            if (!strcmp(input, lexemes[i]))
            {
                break;
            }
        }
        //correct lexeme, there can't be a whitespace after lexeme
        if ('\n' == getchar() && i < lexemes_nm)
        {
            //all lexemes except EOF are represented with following positive numbers, which is -1
            //so if we find a lexeme in array of lexemes on position i, i is code of this lexeme
            //(lexemes are represented with int codes in this program), too
            token->type = i < lexemes_nm - 1 ? i : EOF;
            //this lexemes need attribute
            if (i == INTEGER || i == STRING || i == FLOAT || i == VAR) //need attribute
            {
                token->attribute = get_attribute();
            }
            //other not
            else
            {
                token->attribute = NULL;
            }
            break;
        }
        //unknown lexeme
        else
        {
            printf("Gimme correct lexeme:  ");
        }
    }
}

char *get_attribute(void)
{
    //auxiliary variables to control state of allocated memory
    int allocated = 10,
        used = 0;

    //there is one more character allocated on the beginning (allocated == 10 but malloc allocates 11 characters), for \0
    char *ret = (char *)malloc(11 * sizeof(char)); //10 chars + \0
    if (ret == NULL)
    {
        fprintf(stderr, "Malloc fail");
        exit(99);
    }

    printf("Gimme attribute:  ");
    char c = getchar();
    //reading the whole input
    while (c != '\n')
    {
        //if there is more then 10 characters on input, user gets warning
        if (allocated == used)  //TODO
        {
            fprintf(stderr, "Cannot read more\n");
        }
        //and only first 10 characters will be saved, other are ignored
        //in the future program saves all characters from stdin
        if (used < allocated)
        {
            ret[used] = c;
        }
        used++;
        //inserting \0 behind string, after every saved character
        if (used <= allocated)
        {
            ret[used] = '\0';
        }
        c = getchar();
    }
    return ret;
}

void test_of_test(void)
{
    printf("TEST\n");
    printf("------------------------------------------------------------\n\n");


    printf("----------------------\n");
    printf("Get token\n");
    printf("----------------------\n\n");

    token_t *test = get_token();

    printf("\nLexeme: %s\n", lexemes[test->type]);
    if (test->attribute != NULL)
    {
        printf("Lexeme attribute: %s\n", test->attribute);
    }
    putchar('\n');


    printf("----------------------\n");
    printf("Return token\n");
    printf("----------------------\n\n");

    ret_token(test);


    printf("----------------------\n");
    printf("Get token\n");
    printf("----------------------\n\n");

    test = get_token();

    printf("Lexeme: %s\n", lexemes[test->type]);
    if (test->attribute != NULL)
    {
        printf("Lexeme attribute: %s\n", test->attribute);
    }
}
