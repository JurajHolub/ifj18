/**
 * @author Matej Parobek (xparob00)
 * @file top_down_parser.c
 * @date 2018-10-16
 * @brief Implementation of top-down parser
 */

#include "dynamic_string.h"
#include "top_down_parser.h"
#include "error_handle.h"
#include "global_interface.h"


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
    //TODO free resources
    int analysis_result = program_list();
    remove_all_st();
    return analysis_result;
}

int program_list(void) {
    token_t *token = get_token();
    table_item_t *global_symtable = get_main_st();      //TODO doc

    //rule <program_list> -> EOF
    if (token->type == EOF) //TODO
    {
       return SUCCESS;
    }

    //rule <program_list> -> <function_def> <program_list>
    else if (token->type == DEF)
    {
        //expansion of non terminal symbol <function_def>
        ret_token(token);
        int analysis_result = function_def();

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <program_list>
            analysis_result = program_list();
        }
        return analysis_result;
    }

    //rule <program_list> -> <statement> <program_list>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(global_symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <program_list>
            analysis_result = program_list();
        }
        return analysis_result;
    }
}

int function_def(void) {
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
                //creating symbol table entry
                data_t symtable_data;

                //inserting data from token to symbol table entry
                symtable_data.type = UNDEF_FUN;
                symtable_data.value = NIL;
                symtable_data.id = string_create(NULL);
                string_append(symtable_data.id, tmp->attribute);
                symtable_data.param_cnt = 0;

                //creating local symbol table for function
                set_local_st();

                //expansion of non terminal symbol <params>
                int analysis_result = params(&symtable_data);

                //semantic analysis
                if (analysis_result == SUCCESS)
                {
                    //TODO semantic action
                }

                //clear memory
                string_free(symtable_data.id);

                return analysis_result;
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int params(data_t *symtable_data)
{
    token_t *token = get_token();

    //rule <params> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //expansion of non terminal symbol <function_body>
            return function_body();
        }
    }

    //rule <params> -> <param> <param_list>
    else if (token->type == VAR)
    {
        //expansion of non terminal symbol <param>
        int analysis_result = param(symtable_data, token->attribute);   //TODO malloc errors

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <param_list>
            analysis_result = param_list(symtable_data);
        }

        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int param_list(data_t *symtable_data)
{
    token_t *token = get_token();

    //rule <params_list> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //expansion of non terminal symbol <function_body>
            return function_body();
        }
    }

    //rule <param_list> -> , <param> <param_list>
    else if (token->type == DELIM)
    {
        token = get_token();
        if (token->type == VAR)
        {
            //expansion of non terminal symbol <param>
            int analysis_result = param(symtable_data, token->attribute);   //TODO malloc errors

            if (analysis_result == SUCCESS)
            {
                //expansion of non terminal symbol <param_list>
                analysis_result = param_list(symtable_data);
            }

            return analysis_result;
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int param(data_t *symtable_data, string_t param)
{

    /*-**************************************************************************
        insert function parameter in global symbol table in function parameters
    *****************************************************************************/
    /*char *string_blankspace = " ";
    //separating parameters by blank space
    if (symtable_data->id->strlen != 1)
    {
        string_append_ch(param, string_blankspace);
    }
    string_append(symtable_data->id, param);
    symtable_data->param_cnt++;*/

    //TODO generate parameters


    /*-**************************************************************************
        insert function parameters in local symbol table as local variables
     *****************************************************************************/

    //creating symbol table entry
    data_t local_symtable_data;

    //inserting data from token to symbol table entry
    local_symtable_data.type = UNDEF;
    local_symtable_data.value = NIL;
    local_symtable_data.id = string_create(NULL);   //TODO malloc errors
    string_append(local_symtable_data.id, param);
    local_symtable_data.param_cnt = 0;

    //inserting symbol table entry to symbol table
    insert(get_local_st(), &local_symtable_data);

    //clear memory
    string_free(local_symtable_data.id);

    return SUCCESS;
}

int function_body()
{
    token_t *token = get_token();
    table_item_t *local_symtable = get_local_st();

    //rule <function-body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return SUCCESS;
        }
    }

    //rule <function-body> -> <stat> <function_body>
    else
    {
        ret_token(token);

        //expansion of non terminal symbol <statement>
        int analysis_result = statement(local_symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <function_body>
            analysis_result = function_body();
        }

        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int statement(table_item_t *symtable)
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
        //expansion of non terminal symbol <assignment>
        ret_token(token);
        int analysis_result = assignment(symtable);

        if (analysis_result == SUCCESS)
        {
            token = get_token();
            analysis_result = token->type == EOL ? 0 : ERR_SYNTAX;
        }
        return analysis_result;
    }
}

int if_statement(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <if_statement> -> if <expression> then EOL <if_body>
    if (token->type == IF)
    {
        //expansion of non terminal symbol <expression>
        int analysis_result = parse_expression(symtable);
        if (analysis_result != SUCCESS)
        {
            return analysis_result;
        }
        else
        {
            token = get_token();
            if (token->type == THEN)
            {
                token = get_token();
                if (token->type == EOL)
                {
                    //expansion of non terminal symbol <if_body>
                    return if_body(symtable);
                }
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int if_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <if_body> -> else EOL <else-body>
    if (token->type == ELSE)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //expansion of non terminal symbol <else_body>
            return else_body(symtable);
        }
    }

    //rule <if_body> -> <stat> <if_body>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <if_body>
            analysis_result = if_body(symtable);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int else_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <else_body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return SUCCESS;
        }
    }

    //rule <else_body> -> <stat> <else_body>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <else_body>
            analysis_result = else_body(symtable);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int while_statement(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <while_statement> -> while <expression> do EOL <while_body>
    if (token->type == WHILE)
    {
        //expansion of non terminal symbol <expression>
        int analysis_result = parse_expression(symtable);
        if (analysis_result != SUCCESS)
        {
            return analysis_result;
        }
        else
        {
            token = get_token();
            if (token->type == DO)
            {
                token = get_token();
                if (token->type == EOL)
                {
                    //expansion of non terminal symbol <while_body>
                    return while_body(symtable);
                }
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int while_body(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <while_body> -> end EOL
    if (token->type == END)
    {
        token = get_token();
        if (token->type == EOL)
        {
            return SUCCESS;
        }
    }

    //rule <while_body> -> <stat> <while_body>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol while_body>
            analysis_result = while_body(symtable);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int assignment(table_item_t *symtable)
{
    token_t *token = get_token();
    token_t *next_token = get_token();
    if (token == NULL)
    {
        return ERR_LEX;
    }

    /*-***************************************
         rules:
          <assignment> -> ID = <function_call>
          <assignment> -> ID = <expression>
    ******************************************/
    if (token->type == VAR && next_token->type == ASSIG)
    {
        token = get_token();
        next_token = get_token();

        //rule <assignment> -> ID = <function_call>
        if ((token->type == VAR || token->type == FUN) && (next_token->type == VAR || next_token->type == LEFT_B))
        {
            //expansion of non terminal symbol <function_call>
            ret_token(next_token);
            ret_token(token);
            return function_call(symtable);
        }

        //rule <assignment> -> ID = <expression>
        else if(token->type != FUN) //TODO
        {
            //expansion of non terminal symbol <expression>
            ret_token(next_token);
            ret_token(token);
            return parse_expression(symtable);
        }
    }

    /*-***************************************
     rules:
      <assignment> -> <function_call>
      <assignment> -> <expression>
    ******************************************/
    else
    {
        //rule <assignment> -> <function_call>
        if ((token->type == VAR || token->type == FUN) && (next_token->type == VAR || next_token->type == LEFT_B))
        {
            //expansion of non terminal symbol <function_call>
            ret_token(next_token);
            ret_token(token);
            return function_call(symtable);
        }

        //rule <assignment> -> <expression>
        else if(token->type != FUN) //TODO
        {
            //expansion of non terminal symbol <expression>
            ret_token(next_token);
            ret_token(token);
            return parse_expression(symtable);
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int function_call(table_item_t *symtable)
{
    token_t *token = get_token();

    //rule <function_call> -> ID <call_params> EOL
    if (token->type == VAR)
    {
        //expansion of non terminal symbol <call_params>
        return call_params();
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int call_params(void)
{
    token_t *token = get_token();

    //rule <call_params> -> (<call_param>)
    if (token->type == LEFT_B)
    {
        //expansion of non terminal symbol <call_param>
        int analysis_result = call_param();
        if (analysis_result != SUCCESS)
        {
            return analysis_result;
        }
        else
        {
            token = get_token();
            if (token->type == RIGHT_B)
            {
                return SUCCESS;
            }
        }
    }

    //rule <call_params> -> <call_param>
    else
    {
        ret_token(token);
        return call_param();
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int call_param(void)
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
        return SUCCESS;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int call_param_list(void)
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
        return SUCCESS;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}
