#include <stdio.h>
#include "scanner.h"
#include "expressions_parser.h"
#include "symtable.h"

#define TEST_CNT 39

int test_cnt = 0;
int token_cnt = -1;
int sym_cnt = -1;

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
    "id=id+foo id=id+id, id=id/id / foo idid+id/id, id=id+id/id, id=id+id/id / id"
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
        {.data_type = ASSIG, .id = ""},//=
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
    },
// id=id+foo id=id+id, id=id/id / foo idid+id/id, id=id+id/id, id=id+id/id / id
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
    }
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
    }
};

// TODO test for comparations

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

    for (test_cnt =0; test_cnt < TEST_CNT; test_cnt++)
    {
        printf("TEST CASE %d:", test_cnt);
        int res = parse_expression();
        if (res == 1)
            printf(" succes\n");
        else
        {
            printf(" fail\n");
            printf("    \"%s\"\n", tests[test_cnt]);
            printf("    expected:1\n    result:%d\n", res);
        }
        token_cnt = -1;
        sym_cnt = -1;
    }

    return 0;
}
