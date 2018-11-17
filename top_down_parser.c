/**
 * @author Matej Parobek (xparob00)
 * @file top_down_parser.c
 * @date 2018-10-16
 * @brief Implementation of top-down parser
 */
#define SEMANTIC
#include "dynamic_string.h"
#include "top_down_parser.h"
#include "error_handle.h"
#include "global_interface.h"
#include "semantic_parser.h"
#include "gen_out.h"


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
            //expansion of non terminal symbol <program_list> EOL
            analysis_result = program_list();

            //token = get_token();
            //if(token->type !=  EOF)
            //{
            //    analysis_result = ERR_SYNTAX;
            //}
        }

        return analysis_result;
    }

    //rule <program_list> -> <statement> <program_list>
    else
    {
        //expansion of non terminal symbol <statement> EOL
        ret_token(token);
        int analysis_result = statement(global_symtable);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <program_list>
            analysis_result = program_list();

            token = get_token();
            if(token->type !=  EOF)
            {
                analysis_result = ERR_SYNTAX;
            }
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
                data_t ste_newfc;

                //inserting data from token to symbol table entry
                ste_newfc.type = UNDEF_FUN;
                ste_newfc.value = NIL;
                ste_newfc.id = string_create(NULL);
                string_append(ste_newfc.id, tmp->attribute);
                ste_newfc.param_cnt = 0;

                //crating list of parameters saved in string separated by blank space
                string_t str_params = string_create(NULL);

                //creating local symbol table
                set_local_st();

                //expansion of non terminal symbol <params>
                int analysis_result = params(&ste_newfc, str_params);

                //clear memory
                string_free(ste_newfc.id);
                string_free(str_params);

                return analysis_result;
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

//#ifdef SEMANTIC

int generate_function_prologue(data_t *ste_newfc, data_t **ste_params, data_t *fc_end_label)
{
    //calling semantic analysis, which actualize entry in symbol table for L value, too
    //creating local frame
    create_local_frame();
    add_prolog_text("####### BEGIN USER FUNCTION ######\n");
    //label function end
    string_t str_label_fcend = insert_tmp(get_main_st(), UNDEF);
    fc_end_label = search(get_main_st(), str_label_fcend);
    //generating jump to function end
    add_prolog_inst(I_JUMP, &fc_end_label, NULL, NULL);
    ////generating function label
    data_t *ste_ptr_label_fcbeg = search(get_fun_st(), ste_newfc->id);
    add_prolog_inst(I_LABEL, &ste_ptr_label_fcbeg, NULL, NULL);
    //generating new stack frame
    add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    //generating push of stack frame
    add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);

    //generate parameters
    for (int i = 0; i < ste_newfc->param_cnt; i++)
    {
        add_var(ste_params + i);
        add_instruction(I_POPS, ste_params + 1, NULL, NULL);
    }

    //clear memory
    //string_free(str_label_fcend);

    return SUCCESS;
}


int generate_function_epilogue(data_t *fc_end_label)
{
    add_instruction(I_POPFRAME, NULL, NULL, NULL);
    add_instruction(I_RETURN, NULL, NULL, NULL);
    add_instruction(I_LABEL, &fc_end_label, NULL, NULL);
    add_text("####### END USER FUNCTION ######\n");
    free_local_frame();

    return SUCCESS;
}
//#endif

int params(data_t *ste_newfc, string_t str_params)
{
    token_t *token = get_token();
    int analysis_result;

    //rule <params> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //semantic analysis
            data_t **ste_ptrptr_params_array = malloc(ste_newfc->param_cnt * sizeof(data_t));
            if (ste_ptrptr_params_array == NULL)
            {
                return ERR_COMPILER;
            }
            analysis_result = sem_action_fcdef(ste_newfc, str_params, ste_ptrptr_params_array);
            if (analysis_result == SUCCESS)
            {
                //generating function prologue
                string_t str_fun_end_label = insert_tmp(get_main_st(), UNDEF);
                data_t *fc_end_label = search(get_main_st(), str_fun_end_label);
                analysis_result = generate_function_prologue(ste_newfc, ste_ptrptr_params_array, fc_end_label);
                if (analysis_result == SUCCESS)
                {
                    //expansion of non terminal symbol <function_body>
                    analysis_result = function_body();
                    if (analysis_result == SUCCESS)
                    {
                        analysis_result = generate_function_epilogue(fc_end_label);
                    }
                }
            }
            return analysis_result;
        }
    }

    //rule <params> -> <param> <param_list>
    else if (token->type == VAR)
    {
        //appending parameter in parameters list and separating by blank space and increasing parameters number
        if ((ste_newfc->param_cnt)++ != 0)
            string_append_ch(str_params, " ");
        string_append(str_params, token->attribute);

        //expansion of non terminal symbol <param_list>
        return param_list(ste_newfc, str_params);
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int param_list(data_t *ste_newfc, string_t str_params)
{
    token_t *token = get_token();
    int analysis_result;

    //rule <params_list> -> ) EOL <function_body>
    if(token->type == RIGHT_B)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //semantic analysis
            data_t **ste_ptrptr_params_array = malloc(ste_newfc->param_cnt * sizeof(data_t));
            if (ste_ptrptr_params_array == NULL)
            {
                return ERR_COMPILER;
            }
            analysis_result = sem_action_fcdef(ste_newfc, str_params, ste_ptrptr_params_array);
            if (analysis_result == SUCCESS)
            {
                //generating function prologue
                string_t str_fun_end_label = insert_tmp(get_main_st(), UNDEF);
                data_t *fc_end_label = search(get_main_st(), str_fun_end_label);
                analysis_result = generate_function_prologue(ste_newfc, ste_ptrptr_params_array, fc_end_label);
                if (analysis_result == SUCCESS)
                {
                    //expansion of non terminal symbol <function_body>
                    analysis_result = function_body();
                    if (analysis_result ==SUCCESS)
                    {
                        analysis_result = generate_function_epilogue(fc_end_label);
                    }
                }
            }
            return analysis_result;
        }
    }

    //rule <param_list> -> , <param> <param_list>
    else if (token->type == DELIM)
    {
        token = get_token();
        if (token->type == VAR)
        {
            //appending parameter in parameters list and separating by blank space and increasing parameters number
            if ((ste_newfc->param_cnt)++ != 0)
                string_append_ch(str_params, " ");
            string_append(str_params, token->attribute);

            //expansion of non terminal symbol <param_list>
            return param_list(ste_newfc, str_params);
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
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
        //creating symbol table entry for L value
        data_t ste_Lvalue;
        //inserting data from token to symbol table entry
        ste_Lvalue.type = UNDEF;
        ste_Lvalue.value = NIL;
        ste_Lvalue.id = string_create(NULL);   //TODO malloc errors
        string_append(ste_Lvalue.id, token->attribute);
        ste_Lvalue.param_cnt = 0;

        token = get_token();
        next_token = get_token();

        //rule <assignment> -> ID = <function_call>
        if ((token->type == VAR || token->type == FUN) && (next_token->type == VAR || next_token->type == LEFT_B))
        {
            //expansion of non terminal symbol <function_call>
            ret_token(next_token);
            ret_token(token);
            int analysis_result = function_call(symtable);

            //actualize entry in symbol table for L value and call semantic analysis
            if (analysis_result == SUCCESS)
            {
                ste_Lvalue.type = UNDEF;
//#ifdef SEMANTIC
                analysis_result = sem_action_assig(symtable, &ste_Lvalue);
//#endif
                //TODO generate assignment of return value
            }
            //clear memory
            string_free(ste_Lvalue.id);
            return analysis_result;
        }

        //rule <assignment> -> ID = <expression>
        else if(token->type != FUN) //TODO
        {
            //expansion of non terminal symbol <expression>
            ret_token(next_token);
            ret_token(token);
            int analysis_result = parse_expression(symtable);

//#ifdef SEMANTIC
            //calling semantic analysis, which actualize entry in symbol table for L value, too
            if (analysis_result == SUCCESS)
            {
                data_t *ste_ptr_Rvalue = get_expr_type();
                ste_Lvalue.type = VAR;//ste_ptr_Rvalue->type;
                ste_Lvalue.value = ste_ptr_Rvalue->value;

                analysis_result = sem_action_assig(symtable, &ste_Lvalue);

                if (analysis_result == SUCCESS)
                {
                    //getting symbol table entry of L value from symbol table
                    data_t *ste_ptr_Lvalue = search(symtable, ste_Lvalue.id);

                    //generating assignment
                    add_instruction(I_POPS, &ste_ptr_Lvalue, NULL, NULL);

                    //creating symbol table entry for string constant "bool"
                    data_t ste_const_boolString;
                    //inserting data from token to symbol table entry
                    ste_const_boolString.type = CONST;
                    ste_const_boolString.value = STRING;
                    ste_const_boolString.id = string_create("bool");   //TODO malloc errors
                    ste_const_boolString.param_cnt = 0;
                    insert(symtable, &ste_const_boolString);
                    //clear memory
                    data_t *ste_ptr_const_boolString = search(symtable, ste_const_boolString.id);
                    string_free(ste_const_boolString.id);

                    //wasting memmory
                    string_t str_var_typeString = insert_tmp(symtable, VAR);
                    data_t *ste_ptr_var_typeString = search(symtable, str_var_typeString);

                    add_var(&ste_ptr_var_typeString);
                    add_instruction(I_TYPE, &ste_ptr_var_typeString, &ste_ptr_Lvalue, NULL);

                    //wasting memmory
                    string_t str_label_goodType = insert_tmp(symtable, UNDEF);
                    data_t *ste_ptr_label_goodType = search(symtable, str_label_goodType);

                    add_instruction(I_JUMPIFNEQ, &ste_ptr_label_goodType, &ste_ptr_const_boolString, &ste_ptr_var_typeString);

                    //creating symbol table entry for string constant "bool"
                    data_t ste_const_4Int;
                    //inserting data from token to symbol table entry
                    ste_const_4Int.type = CONST;
                    ste_const_4Int.value = INTEGER;
                    ste_const_4Int.id = string_create("4");   //TODO malloc errors
                    ste_const_4Int.param_cnt = 0;
                    insert(symtable, &ste_const_4Int);
                    //clear memory
                    data_t *ste_ptr_const_4Int = search(symtable, ste_const_4Int.id);
                    string_free(ste_const_4Int.id);

                    add_instruction(I_EXIT, &ste_ptr_const_4Int, NULL, NULL);

                    add_instruction(I_LABEL, &ste_ptr_label_goodType, NULL, NULL);

                }
            }
//#endif
            //clear memory
            string_free(ste_Lvalue.id);

            return analysis_result;
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
            int ret = parse_expression(symtable);
            return ret;
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
