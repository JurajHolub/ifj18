/**
 * @author Matej Parobek (xparob00)
 * @file top_down_parser.c
 * @date 2018-10-16
 * @brief Implementation of top-down parser
 */

#include "dynamic_string.h"
#include "top_down_parser.h"
#include "error_handle.h"

//symbol table used for definitions and semantic analysis in main program body
table_item_t *global_symtable;


void syntax_error(int token_type)
{
    static char *string_tokens[] = {"NOT", "SUB", "ADD", "MUL", "DIV", "NOT_EQUAL", "LESS", "INTEGER", "STRING", "NIL", "LEFT_B", "RIGHT_B", "EOL", "EQUAL", "GREATER", "LESS_EQ", "GREATER_EQ", "FLOAT", "DEF", "DO", "ELSE", "END", "IF", "THEN", "WHILE", "ASSIG", "FUN", "DELIM", "VAR"};
    if (token_type == EOF)
    {
        setbuf(stderr, 0);
        setbuf(stdout, 0);
        fprintf(stderr, "Unexpected end of file\n");
    }
    else if (token_type == EOL)
    {
        setbuf(stderr, 0);
        setbuf(stdout, 0);
        fprintf(stderr, "Unexpected end of line\n");
    }
    else
    {
        setbuf(stderr, 0);
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
    else
    {
        ret_token(token);
        return statement(global_symtable) && program_list();
    }
}

bool function_def(void) {
    token_t *token = get_token();

    //rule <function_def> -> def ID(<params>
    if (token->type == DEF)
    {
        token = get_token();
        if (token->type == VAR || token->type ==FUN)
        {
            //if the whole rule is checked symbol table entry will be created and data inserted, but we need data
            //from this token, which will be in the next statement overwritten, so we need to save it
            token_t *tmp = token;
            if ((token = get_token())->type == LEFT_B)
            {
                if (NULL != search(global_symtable, tmp->attribute))
                {
                    setbuf(stdout, 0);
                    setbuf(stderr, 0);
                    fprintf(stderr, "Semantic error");
                    return !ERR_SEM_DEF;
                }

                //creating symbol table entry
                data_t symtable_data;

                //inserting data from token to symbol table entry
                symtable_data.data_type = FUN;
                symtable_data.id = string_create(NULL);
                string_append(symtable_data.id, tmp->attribute);
                symtable_data.value = string_create(NULL);
                symtable_data.param_cnt = 0;
                symtable_data.param_id = string_create(NULL);

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
        //TODO moze sa v tejto funkcii vyskytnut chyba
        return param(symtable_data, local_symtable, token->attribute) && param_list(local_symtable, symtable_data);
    }

    //rule failed, bad syntax
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
            return param(symtable_data, local_symtable, token->attribute) && param_list(local_symtable, symtable_data);
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return false;
}

bool param(data_t *symtable_data,table_item_t *local_symtable, string_t param)
{

    /*-**************************************************************************
        insert function parameter in global symbol table in function parameters
    *****************************************************************************/
    char *string_blankspace = " ";
    //separating parameters by blank space
    if (symtable_data->param_id->strlen != 1)
    {
        string_append_ch(param, string_blankspace);
    }
    string_append(symtable_data->param_id, param);
    symtable_data->param_cnt++;


    /*-**************************************************************************
        insert function parameters in local symbol table as local variables
     *****************************************************************************/

    //creating symbol table entry
    data_t local_symtable_data;

    //inserting data from token to symbol table entry
    local_symtable_data.data_type = VAR;
    local_symtable_data.id = string_create(NULL);
    string_append(local_symtable_data.id, param);
    local_symtable_data.value = string_create(NULL);
    local_symtable_data.param_cnt = 0;
    local_symtable_data.param_id = string_create(NULL);

    //inserting symbol table entry to symbol table
    insert(local_symtable, &local_symtable_data);

    //clear memory
    string_free(local_symtable_data.id);
    string_free(local_symtable_data.value);
    string_free(local_symtable_data.param_id);


    return true;
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

    //rule <statement> -> <assignment> EOL
    else
    {
        ret_token(token);
        //TODO
        bool result = assignment(symtable);
        if (result != true)
        {
            return result;
        }

        token = get_token();
        if (token->type == EOL)
        {
            return true;
        }
    }

    //rule failed, bad syntax
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

bool assignment(table_item_t *symtable) {
    token_t *token = get_token();
    token_t *next_token = get_token();
    if (token == NULL)
    {
        return !ERR_LEX;
    }

    /*-***************************************
         rules:
          <assignment> -> ID = <function_call>
          <assignment> -> ID = <expression>
    ******************************************/
    if (token->type == VAR && next_token->type == ASSIG)
    {
        //by assignment have to be updated or created symbol table entry for L-value variable
        data_t *l_value = search(symtable, token->attribute);
        if (l_value == NULL)
        {
            //creating symbol table entry
            data_t symtable_data;

            //inserting data from token to symbol table entry
            symtable_data.data_type = VAR;
            symtable_data.id = string_create(NULL);
            string_append(symtable_data.id, token->attribute);
            symtable_data.value = string_create(NULL); //TODO ma tam byt nil?
            symtable_data.param_cnt = 0;
            symtable_data.param_id = string_create(NULL);

            //inserting symbol table entry to symbol table
            insert(global_symtable, &symtable_data);

            //clear memory
            string_free(symtable_data.id);
            string_free(symtable_data.value);
            string_free(symtable_data.param_id);
        }

        //TODO
        else if (l_value->data_type == FUN)
        {
            setbuf(stderr, 0);
            setbuf(stdout, 0);
            fprintf(stderr, "Semantic error\n");
            return !ERR_SEM_DEF;
        }

        next_token = get_token();

        //rule <assignment> -> ID = <function_call>
        if (token->type == VAR || token->type == FUN)
        {
            data_t *id_data = search(symtable, next_token->attribute);
            if (id_data != NULL && id_data->data_type == FUN)
            {
                ret_token(next_token);
                //TODO
                bool result = function_call(symtable);
                //TODO set data type of variable
                return result;
            }
        }
        //rule <assignment> -> ID = <expression>
        if(token->type != FUN)
        {
            ret_token(next_token);
            return !parse_expression(symtable);
        }
        else
        {
            setbuf(stderr, 0);
            setbuf(stdout, 0);
            fprintf(stderr, "Semantic error undefined function");
            return false;
        }
    }

    /*-***************************************
     rules:
      <assignment> -> <function_call>
      <assignment> -> <expression>
    ******************************************/
    else
    {
        //we don't analyze token after first ID in line or first function ID in line in this rule
        ret_token(next_token);

        //rule <assignment> -> <function_call>
        if (token->type == VAR || token->type == FUN )
        {
            data_t *id_data = search(symtable, token->attribute);
            if (id_data != NULL && id_data->data_type == FUN)
            {
                ret_token(token);
                return function_call(symtable);
            }
        }

        //rule <assignment> -> <expression>
        //TODO move to bottom up parser
        if (token->type != FUN)
        {
            ret_token(token);
            return !parse_expression(symtable);
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return false;
}

bool function_call(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <function_call> -> ID <call_params> EOL
    if (token->type == VAR)
    {
        if (call_params() == true)
        {
                return true;
        }
    }

    //rule failed, bad syntax
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
    syntax_error(token->type);
    return false;
}
