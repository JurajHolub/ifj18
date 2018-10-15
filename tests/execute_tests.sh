#most handsome and smart script
cp ../expressions_parser.c .
cp ../expressions_parser.h .
cp ../symtable.h .
cp ../symtable.c .
cp ../scanner.h .
cp ../error_handle.c .
cp ../error_handle.h .
make
rm -rf *.o
rm expressions_parser.c
rm expressions_parser.h
rm symtable.h
rm symtable.c
rm scanner.h
rm error_handle.c
rm error_handle.h
./tests_expressions_parser 2> /dev/null
rm tests_expressions_parser
