/**
 * @author Matej Parobek (xparob00)
 * @file top_down_parser.c
 * @date 2018-10-16
 * @brief Implementation of top-down parser
 */

#include "dynamic_string.h"
#include "top_down_parser.h"

//symbol table used for definitions and semantic analysis in main program body
table_item_t *global_symtable;

/*void create_data(data_t *data, int data_type, char *id, char *value)
{
    char *alloc_id = NULL,
         *alloc_value = NULL;
    alloc_id = (char *)malloc(sizeof(char) * (strlen(id) + 1));
    if (alloc_id == NULL)
    {
        fprintf(stderr, "Compiler runtime error: unable to allocate memory");
        //TODO abbort program
        exit(99);
    }
    strcpy(alloc_id, id);
    if (value != NULL)
    {
        alloc_value = (char *)malloc(sizeof(char) * (strlen(value) + 1));
        if (alloc_value == NULL)
        {
            fprintf(stderr, "Compiler runtime error: unable to allocate memory");
            //TODO abbort program
            exit(99);
        }
        strcpy(alloc_value, value);
    }
    data->data_type = data_type;
    data->id = alloc_id;
    data->value = alloc_value;
    data->param_cnt = 0;
    data->param_id = NULL;
}*/


void syntax_error(int token_type)
{
    //TODO hups
    static char *string_tokens[] = {"NOT", "SUB", "ADD", "MUL", "DIV", "NOT_EQUAL", "LESS", "INTEGER", "STRING", "NIL", "LEFT_B", "RIGHT_B", "EOL", "EQUAL", "GREATER", "LESS_EQ", "GREATER_EQ", "FLOAT", "DEF", "DO", "ELSE", "END", "IF", "THEN", "WHILE", "ASSIG", "FUN", "DELIM", "VAR"};
    if (token_type == EOF)
    {
        setbuf(stdout, 0);
        fprintf(stderr, "Unexpected end of file\n");
    }
    else if (token_type == EOL)
    {
        setbuf(stdout, 0);
        fprintf(stderr, "Unexpected end of line\n");
    }
    else
    {
        setbuf(stdout, 0);
        fprintf(stderr, "Unexpected token: %s\n", string_tokens[token_type]);
    }
}

int parse(void)
{
    global_symtable = get_hash_table();
    //TODO free resources
    int ret = program_list() ? 0 : 2;
    destroy_hash_table(global_symtable);
    return ret;
}

bool program_list(void) {
    token_t *token = get_token();

    //rule <program_list> -> EOF
    if (token->type == EOF) //TODO
    {
       return true;
    }

    //rule <program_list> -> <function_def> <program_list>
    else if (token->type == DEF)
    {
        ret_token(token);
        return  function_def() && program_list();
    }

    //rule <program_list> -> <statement> <program_list>
    else if (token->type == IF || token->type == WHILE || token->type == VAR)  //TODO VAR == ID????
    {
        ret_token(token);
        return statement(global_symtable) && program_list();
    }
    else
    {
        syntax_error(token->type);
        return false;
    }
}

bool function_def(void) {
    token_t *token = get_token();

    //rule <function_def> -> def ID(<params>
    if (token->type == DEF)
    {
        if ((token = get_token())->type == VAR)
        {
            //if the whole rule is checked symbol table entry will be created and data inserted, but we need data
            //from this token, which will be in the next statement overwritten, so we need to save it
            token_t *tmp = token;
            if ((token = get_token())->type == LEFT_B)
            {
                //creating symbol table entry
                data_t symtable_data;

                //inserting data from token to symbol table entry
                symtable_data.data_type = FUN;
                symtable_data.id = string_create(NULL);
                string_append(symtable_data.id, tmp->attribute); //TODO ako je alokovany a mazany token
                symtable_data.value = string_create(NULL); //TODO je to dobre
                symtable_data.param_cnt = 0;
                symtable_data.param_id = string_create(NULL);
                //create_data(symtable_data, tmp->type, tmp->attribute, NULL);

                //inserting symbol table entry to symbol table
                insert(global_symtable, &symtable_data);

                //clear memory

                string_free(symtable_data.id);
                string_free(symtable_data.value);
                string_free(symtable_data.param_id);

                //getting data entry for function from symbol table

                data_t *symtable_data_ptr = search(global_symtable, tmp->attribute);

                //creating local symtable for function body definitions and semantic analysis
                table_item_t *function_symtable = get_hash_table();

                return params(function_symtable, symtable_data_ptr);
            }
        }
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;

}

bool params(table_item_t *local_symtable, data_t *symtable_data)
{
    token_t *token = get_token();

    //rule <params> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            bool ret = function_body(local_symtable);
            destroy_hash_table(local_symtable);
            return ret;
        }
    }

    //rule <params> -> <param> <param_list>
    else if (token->type == VAR)  //TODO VAR  premenovat na ID
    {
        return param(symtable_data, token->attribute) && param_list(local_symtable, symtable_data);
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool param_list(table_item_t *local_symtable, data_t *symtable_data)
{
    token_t *token = get_token();

    //rule <params_list> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            bool ret = function_body(local_symtable);
            destroy_hash_table(local_symtable);
            return ret;
        }
    }

    //rule <param_list> -> , <param> <param_list>
    else if (token->type == DELIM)
    {
        token = get_token();
        if (token->type == VAR)
        {
            return param(symtable_data, token->attribute) && param_list(local_symtable, symtable_data);
        }
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool param(data_t *symtable_data, string_t param) {

    char *string_blankspace = " ";
    string_append_ch(param, string_blankspace); //TODO blank space je aj za poslednym argumentom, vieme s tym pracovat
    string_append(symtable_data->param_id, param);
    symtable_data->param_cnt++;
    /*//auxiliary variables to work with dynamic array
    int param_length = strlen(param),   //inserted string length
        param_list_length;              //allocated memory length

    //no memory was allocated
    if (symtable_data->param_id == NULL)
    {
            param_list_length = 0;
    }
    //memory was allocated
    else
    {
        param_list_length = strlen(symtable_data->param_id);
    }

    //new array where fits content of old array and the new inserted parameter + \0
    char *tmp_param_list = (char *)malloc((param_length + param_list_length + 1) * sizeof(char));
    if (tmp_param_list == NULL)
    {
        fprintf(stderr, "Compiler runtime error: unable to allocate memory");
        //TODO abbort program
        exit(99);
    }
    //old array copied in new array, if there was an old array
    if (symtable_data->param_id != NULL)
    {
        strcpy(tmp_param_list, symtable_data->param_id);
        //parameters must be separated by ' '
        tmp_param_list[param_list_length] = ' ';
    }
    //+1 is the extra whitespace before parameter
    strcpy(tmp_param_list + param_list_length + 1, param);

    free(symtable_data->param_id);
    symtable_data->param_id = tmp_param_list;
     */
    return true;  //TODO musi to tu byt
}

bool function_body(table_item_t *local_symtable)
{
    token_t *token = get_token();

    //rule <function-body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return true;
        }
    }

    //rule <function-body> -> <stat> <function_body>
    else if(token->type == IF || token->type == WHILE || token->type == VAR)
    {
        ret_token(token);
        return statement(local_symtable) && function_body(local_symtable);
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool statement(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <statement> -> <if_statement>
    if (token->type == IF)
    {
        ret_token(token);
        return if_statement(symtable);
    }

    //rule <statement> -> <while_statement>
    else if(token->type == WHILE)
    {
        ret_token(token);
        return while_statement(symtable);
    }

    //rule <statement> -> <function_call> or <statement> -> <expression>
    else if(token->type == VAR)
    {
        //TODO co je napr. a+b patri to do statement (zatial ano)
        token_t *next_token = get_token();

        //rule <statement> -> <function_call>
        if (next_token->type == VAR)
        {
            ret_token(next_token);
            ret_token(token);
            return function_call(symtable);
        }

        //rule <statement> -> <expression>
        else
        {
            ret_token(next_token);  //TODO ???
            ret_token(token);
            bool expression_good = parse_expression(symtable) == 0;
            token = get_token();
            return expression_good && (token->type == EOL);
        }
    }
    //TODO tento else if
    /*else if (token->type == INTEGER ||
             token->type == STRING ||
             token->type == FLOAT ||
             token->type == NOT ||
             token->type == SUB ||
             token->type == ADD)
    {
        ret_token(token);
        return (bool)parse_expression(symtable);
    }*/

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool if_statement(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <if_statement> -> if <expression> then EOL <if_body>
    if (token->type == IF)
    {
        if (parse_expression(symtable) == 0)
        {
            token = get_token();
            if (token->type == THEN)
            {
                token = get_token();
                if (token->type == EOL)
                {
                    return if_body(symtable);
                }
            }
        }
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool if_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <if_body> -> else EOL <else-body>
    if (token->type == ELSE)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return else_body(symtable);
        }
        else
        {
            syntax_error(token->type);
            return false;
        }
    }

    //rule <if_body> -> <stat> <if_body>
    else
    {
        ret_token(token);
        return statement(symtable) && if_body(symtable);
    }
}

bool else_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <else_body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return true;
        }
        else
        {
            syntax_error(token->type);
            return false;
        }
    }

    //rule <else_body> -> <stat> <else_body>
    else
    {
        ret_token(token);
        return statement(symtable) && else_body(symtable);
    }
}

bool while_statement(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <while_statement> -> while <expression> do EOL <while_body>
    if (token->type == WHILE)
    {
        if (parse_expression(symtable) == 0)
        {
            token = get_token();
            if (token->type == DO)
            {
                token = get_token();
                if (token->type == EOL)
                {
                    return while_body(symtable);
                }
            }
        }
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool while_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <while_body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return true;
        }
        else
        {
            syntax_error(token->type);
            return false;
        }
    }

    //rule <while_body> -> <stat> <while_body>
    else
    {
        ret_token(token);
        return statement(symtable) && while_body(symtable);
    }
}

bool function_call(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <function_call> -> ID <call_params> EOL
    if (token->type == VAR)
    {
        if (call_params() == true)
        {
            token = get_token();
            if (token->type == EOL)
            {
                return true;
            }
        }
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool call_params(void)
{
    token_t *token = get_token();

    //rule <call_params> -> (<call_param>)
    if (token->type == LEFT_B)
    {
        if (call_param() == true)
        {
            token = get_token();
            if (token->type == RIGHT_B)
            {
                return true;
            }
        }

        //rule failed, bad syntax
        //TODO free resources
        syntax_error(token->type);
        return false;
    }

    //rule <call_params> -> <call_param>
    else
    {
        ret_token(token);
        return call_param();
    }
}

bool call_param(void)
{
    token_t *token = get_token();

    //rule <call_param> -> ID <call_param_list>
    if (token->type == VAR)
    {
        return call_param_list();
    }

    //rule <call_param> -> ɛ
    else if (token->type == RIGHT_B || token->type == EOL)
    {
        ret_token(token);
        return true;
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}

bool call_param_list(void)
{
    token_t *token = get_token();

    //rule <call_param_list> -> , ID <call_param_list>
    if (token->type == DELIM)
    {
        token = get_token();
        if (token->type == VAR)
        {
            return call_param_list();
        }
    }

    //rule <call_param_list> -> ɛ
    else if (token->type == RIGHT_B || token->type == EOL)
    {
        ret_token(token);
        return true;
    }

    //rule failed, bad syntax
    //TODO free resources
    syntax_error(token->type);
    return false;
}
