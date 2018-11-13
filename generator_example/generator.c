#include <stdio.h>
#include "../expressions_parser.h"
#include "../scanner.h"
#include "../symtable.h"
#include <../stdlib.h>
#include "../semantic_parser.h"
#include "../dynamic_string.h"
#include "../test.h"
#include "../gen_out.h"
#include "../list.h"
#include "../global_interface.h"

int main(int argc, char **argv)
{
    table_item_t* hash_tb = get_main_st(); // get global symbol table for main body
    // get global symbol table for functions, there is already saved library functions
    table_item_t* fun_hash_tb = get_fun_st(); 

    create_global_frame(); // create global frame (main body) for generator

    data_t d;
    d.id = string_create("var_1");
    d.type = VAR;
    d.value = UNDEF;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* var = search(hash_tb, d.id);
    string_free(d.id);
    add_var(&var); // generate variable var_1, all variables will be at the begining of actual frame

    d.id = string_create("10");
    d.type = CONST;
    d.value = INTEGER;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* val = search(hash_tb, d.id);
    string_free(d.id);

    // add instruction to generator -> this instruction will be generated at the start of
    // of actual frame (even before definition af all variables)
    add_prolog_inst(I_IFJCODE18, NULL, NULL, NULL);
    add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);
    
    // add instruction to generator -> this instruction will be generated at the
    // actual end of actual part of program
    add_instruction(I_MOVE, &var, &val, NULL);

    // createa local frame (function body) all prolog_instruction and definition
    // of variables will be generated at the begining of this frame
    create_local_frame(); 
    // you can generate any text (functionality same like add_prolog_inst)
    add_prolog_text("####### BEGIN FUNCTION #########\n");
    set_local_st(); //at the start of new local frame call this function it create new symbol table
    hash_tb = get_local_st(); // now this return actual symbol table of last local frame

    d.id = string_create("$function_1$definition");
    d.type = UNDEF;
    d.value = UNDEF;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* fun_jmp = search(hash_tb, d.id);
    string_free(d.id);
    add_prolog_inst(I_JUMP, &fun_jmp, NULL, NULL);

    d.id = string_create("function_1");
    d.type = DEF_FUN;
    d.value = NIL;
    d.param_cnt = 1;
    insert(fun_hash_tb, &d);
    data_t* fun = search(fun_hash_tb, d.id); // save and search function to right hash table
    string_free(d.id);
    add_prolog_inst(I_LABEL, &fun, NULL, NULL);
    
    add_prolog_inst(I_CREATEFRAME, NULL, NULL, NULL);
    add_prolog_inst(I_PUSHFRAME, NULL, NULL, NULL);

    d.id = string_create("param");
    d.type = VAR;
    d.value = STRING;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* param = search(hash_tb, d.id);
    string_free(d.id);
    add_var(&param);

    add_instruction(I_POPS, &param, NULL, NULL);
    add_instruction(I_WRITE, &param, NULL, NULL);

    d.id = string_create("\\010");
    d.type = CONST;
    d.value = STRING;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* const_str = search(hash_tb, d.id);
    string_free(d.id);
    add_instruction(I_WRITE, &const_str, NULL, NULL);
    
    d.id = string_create("nil");
    d.type = CONST;
    d.value = NIL;
    d.param_cnt = 0;
    insert(hash_tb, &d);
    data_t* nil = search(hash_tb, d.id);
    string_free(d.id);
    add_instruction(I_PUSHS, &nil, NULL, NULL);

    add_instruction(I_RETURN, NULL, NULL, NULL);
    add_instruction(I_LABEL, &fun_jmp, NULL, NULL);

    // you can generate any text (functionality same like add__instruction)
    add_text("####### END FUNCTION #########\n");

    free_local_frame();// end of local frame (end of function) -> return to main body generator
    hash_tb = get_main_st(); // return to main -> use main body symbol table
    
    add_instruction(I_PUSHS, &var, NULL, NULL);
    add_instruction(I_CALL, &fun, NULL, NULL);

    add_instruction(I_CLEARS, NULL, NULL, NULL);
    add_instruction(I_POPFRAME, NULL, NULL, NULL);

    gen_program(); // generate whole program
    free_global_frame(); // free whole list where is actualy saved generator

    remove_all_st(); // remove all global symbol tables (for main body, functions, lokal frames)

    return 0;
}
// ############# program in ifj18 ###################
// var_1="hello_world"
// def function_1(param1)
//     print param1, "\n"
//     print "\n"
// end
// function_1 var_1
// 
// ############# program in ifjcode18 ###################
// .IFJcode18
// CREATEFRAME
// PUSHFRAME
// DEFVAR LF@var_1
// MOVE LF@var_1 string@hello_world
// JUMP $function$definition
// LABEL function_1
// CREATEFRAME
// PUSHFRAME
// DEFVAR LF@param1
// POPS LF@param1
// PUSHS string@\010
// PUSHS LF@param1
// CALL print
// CLEARS
// PUSHS nil@nil
// POPFRAME
// RETURN
// LABEL $function$definition
// PUSHS LF@var_1
// PUSHS LF@var_1
// CALL function_1
// CLEARS
// POPFRAME

