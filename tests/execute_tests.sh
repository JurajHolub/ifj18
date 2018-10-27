#most handsome and smart script
cp ../expressions_parser.c .
cp ../expressions_parser.h .
cp ../symtable.h .
cp ../symtable.c .
cp ../scanner.h .
cp ../error_handle.c .
cp ../error_handle.h .
cp ../semantic_parser.c .
cp ../semantic_parser.h .
cp ../stack.h .
cp ../stack.c .
cp ../dynamic_string.h .
cp ../dynamic_string.c .
make
rm -rf *.o
rm expressions_parser.c
rm expressions_parser.h
rm symtable.h
rm symtable.c
rm scanner.h
rm error_handle.c
rm error_handle.h
rm semantic_parser.c
rm semantic_parser.h
rm stack.c
rm stack.h
rm dynamic_string.c
rm dynamic_string.h
./tests_expressions_parser #2> /dev/null
rm tests_expressions_parser
