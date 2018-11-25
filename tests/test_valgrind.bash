#!/bin/bash

if [ "`pwd | grep -oE '[^/]+/[^/]+$'`" = "ifj18/tests" ]
then
	rm ../*.o 2> /dev/null
	rm ../main 2> /dev/null
	rm ./main 2> /dev/null
else
	echo "Change directory to ifj18/tests"
	exit 1
fi

(cd ../ && make) || { printf '\033[1;4;31m\n\nUnable to compile program!\033[0m\n' ; exit 1; }

cd tests/
cp ../main .

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

for path in correct_program_tests incorrect_program_tests
do
    printf "\n****************************** $path ******************************\n\n"
    for file in $path/*
    do

        ./main < $file > /dev/null 2>&1
        res=$?
        ret="return value: $res"
    
        if test $res -eq 0
        then
            if [ "$path" = 'correct_program_tests' ];
            then
                printf "${GREEN}${file}: SUCCESS${NC}\n"
                echo "    $ret"
		valgrind --log-file=valgrind ./main < $file > /dev/null 2>&1
          	echo "    ---------------------------------------------------------------VALGRIND OUT"
          	cat valgrind | sed 's/^.*[=]/    |/g' | grep bytes
          	echo "    ---------------------------------------------------------------VALGRIND OUT"
            else
                printf "${RED}${file}: SUCCESS${NC}\n"
                echo "    $ret"
            fi
        else
            if [ "$path" = 'correct_program_tests' ];
            then
                printf "${RED}${file}: FAILED${NC}\n"
                echo "    $ret"
            else
                printf "${GREEN}${file}: FAILED${NC}\n"
                echo "    $ret"
		valgrind --log-file=valgrind ./main < $file > /dev/null 2>&1
          	echo "    ---------------------------------------------------------------VALGRIND OUT"
          	cat valgrind | sed 's/^.*[=]/    |/g' | grep bytes
          	echo "    ---------------------------------------------------------------VALGRIND OUT"
            fi
        fi
    done
done 
