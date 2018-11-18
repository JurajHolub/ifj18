#!/bin/bash

cd ../ && make -f Makefile
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
            else
                printf "${RED}${file}: SUCCESS${NC}\n"
            fi
        else
            if [ "$path" = 'correct_program_tests' ];
            then
                printf "${RED}${file}: FAILED${NC}\n"
            else
                printf "${GREEN}${file}: FAILED${NC}\n"
            fi
        fi
        echo "    $ret"

    done
done 
