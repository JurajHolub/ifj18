#include <stdio.h>
#include "scanner.h"
#include "expressions_parser.h"
#include "symtable.h"
#include "error_handle.h"
#include "dynamic_string.h"

#define TEST_CNT 72

int test_cnt = 0;
int token_cnt = -1;
int sym_cnt = -1;
token_t t;

int expected[TEST_CNT] = {




    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    SUCCESS,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
    ERR_SYNTAX,
};

char *tests[TEST_CNT] =
{
    "id+id-id+id-id",
    "id*id/id/id",
    "id+id/id*id+id/id",
    "(id+id)/(id+id/id)",
    "(((id)))",
    "id=id",
    "id=(id+id)/id",
    "id=foo() ",
    "id=foo(id) ",
    "id=foo(id,id) ",
    "id=foo(id,id,id) ",
    "id=foo ",
    "id=foo id ",
    "id=foo id,id ",
    "id=foo id,id,id ",
    "id=id+foo() / id",
    "id=id+foo(id) / id ",
    "id=id+foo(id,id) / id",
    "id=id+foo(id,id,id) / id",
    "id=id+foo / id",
    "id=id+foo id / id ",
    "id=id+foo id,id / id",
    "id=id+foo id,id,id / id",
    "id=foo(id+id/id) ",
    "id=foo(id+id/id, id+id/id) ",
    "id=foo(id+id/id, id+id/id, id+id/id) ",
    "id=foo id+id/id ",
    "id=foo id+id/id, id+id/id ",
    "id=foo id+id/id, id+id/id, id+id/id ",
    "id=foo(id=id+id/id) ",
    "id=foo(id=id+id/id, id=id+id/id) ",
    "id=foo(id=id+id/id, id=id+id/id, id=id+id/id) ",
    "id=foo id=id+id/id ",
    "id=foo id=id+id/id, id=id+id/id",
    "id=foo id=id+id/id, id=id+id/id, id=id+id/id",
    "id=id+foo(id,id) / foo(id,id,id) / id",
    "id=id+foo id,id / foo id,id,id / id",
    "id=id+foo(id=id+id,id=id/id) / foo(id=id+id/id,id=id+id/id,id=id+id/id) / id",
    "id=id+foo id=id+id, id=id/id / foo idid+id/id, id=id+id/id, id=id+id/id / id",
    "id+-id",
    "id+id/",
    "((id+id)",
    "id=+",
    "id=foo(",
    "id=foo)",
    "id=foo id )"
    "id=foo ( id",
    "id=foo(id,id",
    "id=foo(id,id,)",
    "id=foo(,)",
    "id=id(id)",
    "id=id(id, id)",
    "id=id id",
    "id=id id, id",
    "id == id",
    "id+id != id/id",
    "id < foo (id,id)",
    "foo < id",
    "foo() > id",
    "foo(id) <= foo (id,id)",
    "(id > id) == id ",
    "(id < foo) == id ",
    "(id >= foo id) != id ",
    "((id > id) == id ) != id",
    "id == - id",
    "id == + id",
    "foo - == id",
    "id - == id",
    "id < / foo id",
    "id < == id",
    "foo (id==id)",
    "foo id==id",
    "id = id < id",
};
/*
data_t datas[TEST_CNT][50] = {
    {
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""}
    },
    {
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""}
    },
    {
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""},
        {.data_type = VAR, .id = ""}
    },
// (id+id)/(id+id/id)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// (((id)))
    {
        {.data_type = VAR, .id = ""},//id
    },
// id=id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=(id+id)/id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo() 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
    },
// id=foo(id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id,id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id,id,id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
    },
// id=foo id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id,id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id,id,id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo() / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo(id) / id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo(id,id) / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo(id,id,id) / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo id / id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo id,id / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo id,id,id / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id+id/id, id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id+id/id, id+id/id, id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id+id/id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id+id/id, id+id/id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id+id/id, id+id/id, id+id/id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id=id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id=id+id/id, id=id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id=id+id/id, id=id+id/id, id=id+id/id) 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id=id+id/id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id=id+id/id, id=id+id/id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo id=id+id/id, id=id+id/id, id=id+id/id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo(id,id) / foo(id,id,id) / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo id,id / foo id,id,id / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo(id=id+id,id=id/id) / foo(id=id+id/id,id=id+id/id,id=id+id/id) / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id+foo id=id+id, id=id/id / foo id=id+id/id, id=id+id/id, id=id+id/id / id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id+-id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id+id/
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// ((id+id)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=+
    {
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
    },
// id=foo)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
    },
// id=foo ( id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id,id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(id,id,)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=foo(,)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
    },
// id=id(id)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id(id, id)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id=id id, id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id == id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id+id != id/id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id < foo (id,id)
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo < id
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo() > id
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo(id) <= foo (id,id)
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// (id > id) == id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// (id < foo) == id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// (id >= foo id) != id 
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// ((id > id) == id ) != id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id == - id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id == + id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo - == id
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id - == id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id < / foo id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id < == id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo (id==id)
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// foo id==id
    {
        {.data_type = FUN, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
// id = id < id
    {
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
        {.data_type = VAR, .id = ""},//id
    },
};
*/

struct tmp_t {
    char* attribute;
    int type;
};

struct tmp_t tokens[TEST_CNT][50] = {
// id+id-id+id-id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = SUB, .attribute = ""},//-
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = SUB, .attribute = ""},//-
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id*id/id/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = MUL, .attribute = ""},// *
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id+id/id*id+id/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = MUL, .attribute = ""},// *
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// (id+id)/(id+id/id)
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// (((id)))
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=(id+id)/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo() 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id,id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id,id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id,id,id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo() / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id) / id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id) / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id,id) / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id / id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id,id / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id, id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id, id+id/id, id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id, id+id/id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id, id+id/id, id+id/id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id, id=id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id, id=id+id/id, id=id+id/id) 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id 
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id, id=id+id/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id, id=id+id/id, id=id+id/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id) / foo(id,id,id) / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id / foo id,id,id / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id=id+id,id=id/id) / foo(id=id+id/id,id=id+id/id,id=id+id/id) / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id=id+id, id=id/id / foo idid+id/id, id=id+id/id, id=id+id/id / id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = VAR, .attribute = "foo"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id+-id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = SUB, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id+id/
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = EOL, .attribute = ""}
    },
// ((id+id)
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=+
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = ADD, .attribute = ""},//+
        {.type = EOL, .attribute = ""}
    },
// id=foo(
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = EOL, .attribute = ""}
    },
// id=foo)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo ( i
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id,)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(,)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = DELIM, .attribute = ""},// ,
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id(id)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id(id, id)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id=id id, id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id == id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id+id != id/id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = "id"},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id < foo (id,id)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// foo < id
    {
        {.type = VAR, .attribute = "foo"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// foo() > id
    {
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = GREATER, .attribute = ""},// >
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// foo(id) <= foo (id,id)
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = LESS_EQ, .attribute = ""},// <=
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// (id > id) == id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// (id < foo) == id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = "foo"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// (id >= foo id) != id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = GREATER_EQ, .attribute = ""},// >=
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// ((id > id) == id ) != id
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = GREATER, .attribute = ""},// >
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id == - id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = SUB, .attribute = ""},// -
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id == + id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// foo - == id
    {
        {.type = VAR, .attribute = "foo"},//id
        {.type = SUB, .attribute = ""},// -
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id - == id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = SUB, .attribute = ""},// -
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id < / foo id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id < == id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// foo (id==id)
    {
        {.type = VAR, .attribute = "foo"},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// foo id==id
    {
        {.type = VAR, .attribute = "foo"},//id
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
// id = id < id
    {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = "id"},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
    },
};

token_t* get_token()
{

    if (token_cnt != -1)
        string_free(t.attribute);
    token_cnt++;
    t.type = tokens[test_cnt][token_cnt].type;
    t.attribute = string_create(tokens[test_cnt][token_cnt].attribute);
    return &t;
}

int main()
{
    table_item_t *hash_tb = get_hash_table();
    data_t d;
    d.data_type = VAR;
    d.id = string_create("id");
    d.value = string_create(NULL);
    d.fun_type = VAR;
    d.param_cnt = 0;
    d.param_id = string_create(NULL);
    insert(hash_tb, &d);
    d.data_type = FUN;
    string_free(d.id);
    d.id = string_create("foo");
    insert(hash_tb, &d);
    string_free(d.id);
    string_free(d.value);
    string_free(d.param_id);

    for (test_cnt =0; test_cnt < TEST_CNT; test_cnt++)
    {
        printf("TEST CASE %d:", test_cnt);
        int res = parse_expression(hash_tb);
        if (res == expected[test_cnt])
            printf(" succes\n");
        else
        {
            printf(" fail\n");
            printf("    \"%s\"\n", tests[test_cnt]);
            printf("    expected:%d\n    result:%d\n", expected[test_cnt], res);
        }
        token_cnt = -1;
        sym_cnt = -1;
    }

    destroy_hash_table(hash_tb);

    return 0;
}
