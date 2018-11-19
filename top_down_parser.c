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
#include <string.h>


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
    if (analysis_result == SUCCESS)
    {
        token_t *token = get_token();
        if (token->type != EOF)
        {
            analysis_result = ERR_SYNTAX;
        }
    }
    return analysis_result;
}

int program_list(void) {
    token_t *token = get_token();
    table_item_t *global_symtable = get_main_st();      //TODO doc

    //rule <program_list> -> EOF
    if (token->type == EOF) //TODO
    {
        ret_token(token);
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
        }

        return analysis_result;
    }

    //rule <program_list> -> <statement> <program_list>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(global_symtable, true);

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
                data_t ste_newfc;

                //inserting data from token to symbol table entry
                ste_newfc.type = FUN;
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

int generate_function_prologue(data_t *ste_newfc, data_t **ste_params, data_t **fc_end_label)
{
    //calling semantic analysis, which actualize entry in symbol table for L value, too
    //creating local frame
    create_local_frame();
    add_prolog_text("####### BEGIN USER FUNCTION ######\n");
    //label function end
    string_t str_label_fcend = insert_tmp(get_main_st(), UNDEF);
    *fc_end_label = search(get_main_st(), str_label_fcend);
    //generating jump to function end
    add_prolog_inst(I_JUMP, fc_end_label, NULL, NULL);
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
        add_instruction(I_POPS, ste_params + i, NULL, NULL);
    }

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
                data_t *fc_end_label;
                analysis_result = generate_function_prologue(ste_newfc, ste_ptrptr_params_array, &fc_end_label);
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
                data_t *fc_end_label;
                analysis_result = generate_function_prologue(ste_newfc, ste_ptrptr_params_array, &fc_end_label);
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
        int analysis_result = statement(local_symtable, false);

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

int statement(table_item_t *symtable, bool main_body_st)
{
    token_t *token = get_token();

    //rule <statement> -> <if_statement>
    if (token->type == IF)
    {
        ret_token(token);
        return if_statement(symtable, main_body_st);
    }

    //rule <statement> -> <while_statement>
    else if(token->type == WHILE)
    {
        ret_token(token);
        return while_statement(symtable, main_body_st);
    }

    //rule <statement> -> <assignment> EOL
    else
    {
        //expansion of non terminal symbol <assignment>
        ret_token(token);
        int analysis_result = assignment(symtable, main_body_st);

        if (analysis_result == SUCCESS)
        {
            token = get_token();
            analysis_result = token->type == EOL ? 0 : ERR_SYNTAX;
        }
        return analysis_result;
    }
}

int if_statement(table_item_t *symtable, bool main_body_if)
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
                    return if_body(symtable, main_body_if);
                }
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int if_body(table_item_t *symtable, bool main_body_if)
{
    token_t *token = get_token();

    //rule <if_body> -> else EOL <else-body>
    if (token->type == ELSE)
    {
        token = get_token();
        if (token->type == EOL)
        {
            //expansion of non terminal symbol <else_body>
            return else_body(symtable, main_body_if);
        }
    }

    //rule <if_body> -> <stat> <if_body>
    else
    {
        //expansion of non terminal symbol <statement>
        ret_token(token);
        int analysis_result = statement(symtable, main_body_if);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <if_body>
            analysis_result = if_body(symtable, main_body_if);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int else_body(table_item_t *symtable, bool main_body_else)
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
        int analysis_result = statement(symtable, main_body_else);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol <else_body>
            analysis_result = else_body(symtable, main_body_else);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int while_statement(table_item_t *symtable, bool main_body_while)
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
                    return while_body(symtable, main_body_while);
                }
            }
        }
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int while_body(table_item_t *symtable, bool main_body_while)
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
        int analysis_result = statement(symtable, main_body_while);

        if (analysis_result == SUCCESS)
        {
            //expansion of non terminal symbol while_body>
            analysis_result = while_body(symtable, main_body_while);
        }
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int generate_assignment(table_item_t *symtable, data_t *ste_ptr_Lvalue)
{
    //getting Rvalue
    add_instruction(I_POPS, &ste_ptr_Lvalue, NULL, NULL);

    //creating symbol table entry for string constant "bool"
    data_t ste_const_boolString;
    //inserting data from token to symbol table entry
    ste_const_boolString.type = CONST;
    ste_const_boolString.value = STRING;
    ste_const_boolString.id = string_create("bool");   //TODO malloc errors
    ste_const_boolString.param_cnt = 0;
    insert(symtable, &ste_const_boolString);
    data_t *ste_ptr_const_boolString = search(symtable, ste_const_boolString.id);
    //clear memory
    string_free(ste_const_boolString.id);

    //generating auxiliary variable for saving data type of Rvalue
    string_t str_var_typeString = insert_tmp(symtable, VAR);
    data_t *ste_ptr_var_typeString = search(symtable, str_var_typeString);

    add_var(&ste_ptr_var_typeString);

    //saving data type of Rvalue
    add_instruction(I_TYPE, &ste_ptr_var_typeString, &ste_ptr_Lvalue, NULL);

    //label for conditional jump
    string_t str_label_goodType = insert_tmp(symtable, UNDEF);
    data_t *ste_ptr_label_goodType = search(symtable, str_label_goodType);

    //conditional jump if Rvalue is not bool type
    add_instruction(I_JUMPIFNEQ, &ste_ptr_label_goodType, &ste_ptr_const_boolString, &ste_ptr_var_typeString);

    //creating symbol table entry for integer constant 4 for error code, of Rvalue is bool
    data_t ste_const_4Int;
    //inserting data from token to symbol table entry
    ste_const_4Int.type = CONST;
    ste_const_4Int.value = INTEGER;
    ste_const_4Int.id = string_create("4");   //TODO malloc errors
    ste_const_4Int.param_cnt = 0;
    insert(symtable, &ste_const_4Int);
    data_t *ste_ptr_const_4Int = search(symtable, ste_const_4Int.id);
    //clear memory
    string_free(ste_const_4Int.id);

    //exit with error
    add_instruction(I_EXIT, &ste_ptr_const_4Int, NULL, NULL);

    //successful assignment
    add_instruction(I_LABEL, &ste_ptr_label_goodType, NULL, NULL);

    return SUCCESS;
}

int assignment(table_item_t *symtable, bool main_body_assig)
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
        ste_Lvalue.type = VAR;
        ste_Lvalue.value = UNDEF;
        ste_Lvalue.id = string_create(NULL);   //TODO malloc errors
        string_append(ste_Lvalue.id, token->attribute);
        ste_Lvalue.param_cnt = 0;

        token = get_token();
        next_token = get_token();

        //rule <assignment> -> ID = <function_call>
        if ((token->type == VAR || token->type == FUN) &&
        (next_token->type == VAR || next_token->type == INTEGER || next_token->type == STRING ||
         next_token->type == FLOAT || next_token->type == NIL || next_token->type == LEFT_B))
        {
            //expansion of non terminal symbol <function_call>
            ret_token(next_token);
            ret_token(token);
            int analysis_result = function_call(symtable, !main_body_assig);

            //actualize entry in symbol table for L value and call semantic analysis
            if (analysis_result == SUCCESS)
            {
                ste_Lvalue.value = UNDEF; //JURAJ EDIT: set return value to UNDEF, type is still VAR
                analysis_result = sem_action_assig(symtable, &ste_Lvalue);

                //generating assignment
                if (analysis_result == SUCCESS)
                {
                    //getting symbol table entry of L value from symbol table
                    data_t *ste_ptr_Lvalue = search(symtable, ste_Lvalue.id);
                    generate_assignment(symtable, ste_ptr_Lvalue);
                }
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


            //calling semantic analysis, which actualize entry in symbol table for L value, too
            if (analysis_result == SUCCESS)
            {
                //data_t *ste_ptr_Rvalue = get_expr_type();
                //ste_Lvalue.value = ste_ptr_Rvalue->value;

                analysis_result = sem_action_assig(symtable, &ste_Lvalue);

                //generating assignment
                if (analysis_result == SUCCESS)
                {
                    //getting symbol table entry of L value from symbol table
                    data_t *ste_ptr_Lvalue = search(symtable, ste_Lvalue.id);
                    generate_assignment(symtable, ste_ptr_Lvalue);
                }
            }
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
        if ((token->type == VAR || token->type == FUN) &&
            (next_token->type == LEFT_B ||
             next_token->type == VAR ||
             next_token->type == INTEGER || next_token->type == STRING ||
             next_token->type == FLOAT || next_token->type == NIL))
        {
            //expansion of non terminal symbol <function_call>
            ret_token(next_token);
            ret_token(token);
            return function_call(symtable, !main_body_assig);
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

int generate_function_call (table_item_t *symtable, data_t *ste_ptr_callfc, params_t params)
{
    for (int i = (ste_ptr_callfc->param_cnt) - 1; i >= 0; i--)
    {
       data_t *aux = search(symtable, (params->params)[i].id);
       add_instruction(I_PUSHS, &aux, NULL, NULL);
    }

    if (strcmp(ste_ptr_callfc->id->string, "print") == 0)
    {
        //creating symbol table entry for integer constant 4 for error code, of Rvalue is bool
        data_t ste_const_paramsNmInt;
        //inserting data from token to symbol table entry
        ste_const_paramsNmInt.type = CONST;
        ste_const_paramsNmInt.value = INTEGER;

        char param_cnt[10];
        sprintf(param_cnt, "%d", ste_ptr_callfc->param_cnt); //int to string
        ste_const_paramsNmInt.id = string_create(param_cnt);
        //ste_const_paramsNmInt.id = string_create("4");
        ste_const_paramsNmInt.param_cnt = 0;
        insert(symtable, &ste_const_paramsNmInt);
        data_t *ste_ptr_const_paramsNmInt = search(symtable, ste_const_paramsNmInt.id);
        //clear memory
        string_free(ste_const_paramsNmInt.id);

        //push paramters number for function print
        add_instruction(I_PUSHS, &ste_ptr_const_paramsNmInt, NULL, NULL);
    }

    data_t *aux = search(get_fun_st(), ste_ptr_callfc->id);
    add_instruction(I_CALL, &aux, NULL, NULL);
    return SUCCESS;

}

int function_call(table_item_t *symtable, bool accept_undef)
{
    token_t *token = get_token();

    //rule <function_call> -> ID <call_params> EOL
    if (token->type == VAR)
    {
        //creating symbol table entry
        data_t ste_callfc;

        //inserting data from token to symbol table entry
        ste_callfc.type = UNDEF_FUN;
        ste_callfc.value = UNDEF;
        ste_callfc.id = string_create(NULL);
        string_append(ste_callfc.id, token->attribute);
        ste_callfc.param_cnt = 0;

        struct params_s params;
        params.array_size = PARAMS_SIZE;
        params.params = malloc(PARAMS_SIZE * sizeof(data_t));
        if (params.params == NULL)
        {
            return ERR_COMPILER;
        }
        //expansion of non terminal symbol <call_params>
        int analysis_result = call_params(symtable, &ste_callfc, &params);
        if (analysis_result == SUCCESS)
        {
            analysis_result = sem_action_callfc(symtable, &ste_callfc, &params, accept_undef);
            if (analysis_result == SUCCESS)
            {
                data_t *ste_ptr_callfc = search(get_fun_st(), ste_callfc.id);
                if (strcmp(ste_ptr_callfc->id->string, "print") == 0)
                {
                    ste_ptr_callfc->param_cnt = ste_callfc.param_cnt;
                }
                analysis_result = generate_function_call(symtable, ste_ptr_callfc, &params);
            }
        }

        //clear memory
        for (int i = 0; i < ste_callfc.param_cnt; i++)
        {
            string_free((params.params)[i].id);
        }
        free(params.params);
        string_free(ste_callfc.id);
        return analysis_result;
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int call_params(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params)
{
    token_t *token = get_token();

    //rule <call_params> -> (<call_param>)
    if (token->type == LEFT_B)
    {
        //expansion of non terminal symbol <call_param>
        int analysis_result = call_param(symtable, ste_ptr_callfc, params);
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
        return call_param(symtable, ste_ptr_callfc, params);
    }

    //rule failed, bad syntax
    syntax_error(token->type);
    return ERR_SYNTAX;
}

int call_param(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params)
{
    token_t *token = get_token();

    //rule <call_param> -> ID <call_param_list>
    if (token->type == VAR || token->type == INTEGER || token->type == STRING ||
        token->type == FLOAT || token->type == NIL || token->type == LEFT_B)
    {
        if (ste_ptr_callfc->param_cnt == (params->array_size) - 1)
        {
            token_t *tmp = realloc(params->params, (params->array_size + PARAMS_SIZE) * sizeof(token_t));
            {
                if (tmp == NULL)
                {
                    return ERR_COMPILER;
                }
                (params->array_size) += PARAMS_SIZE;
            }
        }

        (params->params)[ste_ptr_callfc->param_cnt].type = token->type == VAR ? VAR : CONST;
        (params->params)[ste_ptr_callfc->param_cnt].value = token->type == VAR ? UNDEF : token->type;
        (params->params)[ste_ptr_callfc->param_cnt].id = string_create(token->attribute->string);
        (params->params)[ste_ptr_callfc->param_cnt].param_cnt = 0;
        (ste_ptr_callfc->param_cnt)++;
        return call_param_list(symtable, ste_ptr_callfc, params);

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

int call_param_list(table_item_t *symtable, data_t *ste_ptr_callfc, params_t params) {
    token_t *token = get_token();

    //rule <call_param_list> -> , ID <call_param_list>
    if (token->type == DELIM)
    {
        token = get_token();
        if (token->type == VAR || token->type == INTEGER || token->type == STRING ||
            token->type == FLOAT || token->type == NIL || token->type == LEFT_B)
        {
            if (ste_ptr_callfc->param_cnt == (params->array_size) - 1)
            {
                token_t *tmp = realloc(params->params, (params->array_size + PARAMS_SIZE) * sizeof(token_t));
                {
                    if (tmp == NULL)
                    {
                        return ERR_COMPILER;
                    }
                    (params->array_size) += PARAMS_SIZE;
                }
            }

            (params->params)[ste_ptr_callfc->param_cnt].type = token->type == VAR ? VAR : CONST;
            (params->params)[ste_ptr_callfc->param_cnt].value = token->type == VAR ? UNDEF : token->type;
            (params->params)[ste_ptr_callfc->param_cnt].id = string_create(token->attribute->string);
            (params->params)[ste_ptr_callfc->param_cnt].param_cnt = 0;
            (ste_ptr_callfc->param_cnt)++;
            return call_param_list(symtable, ste_ptr_callfc, params);
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
