#include <stdio.h>
#include "scanner.h"
#include "expressions_parser.h"
#include "symtable.h"
#include "error_handle.h"

#define TEST_CNT 72

int test_cnt = 0;
int token_cnt = -1;
int sym_cnt = -1;

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

token_t tokens[TEST_CNT][50] = {
// id+id-id+id-id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = SUB, .attribute = ""},//-
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = SUB, .attribute = ""},//-
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id*id/id/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = MUL, .attribute = ""},//*
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id+id/id*id+id/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = MUL, .attribute = ""},// *
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// (id+id)/(id+id/id)
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// (((id)))
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=(id+id)/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo() 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id,id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id,id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id,id,id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo() / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id) / id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id) / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id,id) / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id / id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id,id / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id, id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id+id/id, id+id/id, id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id, id+id/id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id+id/id, id+id/id, id+id/id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id, id=id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(id=id+id/id, id=id+id/id, id=id+id/id) 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id 
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id, id=id+id/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo id=id+id/id, id=id+id/id, id=id+id/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id,id) / foo(id,id,id) / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id,id / foo id,id,id / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo(id=id+id,id=id/id) / foo(id=id+id/id,id=id+id/id,id=id+id/id) / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id+foo id=id+id, id=id/id / foo idid+id/id, id=id+id/id, id=id+id/id / id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},//=
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id+-id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = SUB, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id+id/
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = EOL, .attribute = ""}
    },
// ((id+id)
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=+
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = ADD, .attribute = ""},//+
        {.type = EOL, .attribute = ""}
    },
// id=foo(
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = EOL, .attribute = ""}
    },
// id=foo)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo ( i
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=foo(id,id,)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=foo(,)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = DELIM, .attribute = ""},// ,
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id(id)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id(id, id)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// id=id id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id=id id, id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id == id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id+id != id/id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ADD, .attribute = ""},//+
        {.type = VAR, .attribute = ""},//id
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = ""},//id
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id < foo (id,id)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// foo < id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// foo() > id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = GREATER, .attribute = ""},// >
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// foo(id) <= foo (id,id)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = LESS_EQ, .attribute = ""},// <=
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = DELIM, .attribute = ""},// ,
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// (id > id) == id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// (id < foo) == id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// (id >= foo id) != id 
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = GREATER_EQ, .attribute = ""},// >=
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// ((id > id) == id ) != id
    {
        {.type = LEFT_B, .attribute = ""},// (
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = GREATER, .attribute = ""},// >
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = NOT_EQUAL, .attribute = ""},// !=
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id == - id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = SUB, .attribute = ""},// -
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id == + id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = ADD, .attribute = ""},// +
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// foo - == id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = SUB, .attribute = ""},// -
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id - == id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = SUB, .attribute = ""},// -
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id < / foo id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = DIV, .attribute = ""},// /
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id < == id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// foo (id==id)
    {
        {.type = VAR, .attribute = ""},//id
        {.type = LEFT_B, .attribute = ""},// (
        {.type = VAR, .attribute = ""},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = RIGHT_B, .attribute = ""},// )
        {.type = EOL, .attribute = ""}
    },
// foo id==id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = VAR, .attribute = ""},//id
        {.type = EQUAL, .attribute = ""},// ==
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
// id = id < id
    {
        {.type = VAR, .attribute = ""},//id
        {.type = ASSIG, .attribute = ""},// =
        {.type = VAR, .attribute = ""},//id
        {.type = LESS, .attribute = ""},// <
        {.type = VAR, .attribute = ""},//id
        {.type = EOL, .attribute = ""}
    },
};

// TODO 


token_t* get_token()
{
    token_cnt++;
    return &tokens[test_cnt][token_cnt];
}

data_t* search(char *key)
{
    sym_cnt++;
    return &datas[test_cnt][sym_cnt];
}

int main()
{

    for (test_cnt =0; test_cnt < 58; test_cnt++)
    {
        printf("TEST CASE %d:", test_cnt);
        int res = parse_expression();
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

    return 0;
}
