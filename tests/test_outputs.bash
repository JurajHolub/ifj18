#!/bin/bash

# @author Juraj Holub
# @brief Final test for full testing
# @date November 2018

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

#cd tests/
#cp ../main .


BLUE='\033[1;34m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

if [ ! -d "output" ]; then
    mkdir "output"
fi
if [ ! -d "output/correct_program_tests" ]; then
    mkdir "output/correct_program_tests"
fi
if [ ! -d "output/incorrect_program_tests" ]; then
    mkdir "output/incorrect_program_tests"
fi

for path in correct_program_tests incorrect_program_tests
do
for test_file in $path/*
do
    file=`basename "$test_file"`

    if [ "$path" = 'correct_program_tests' ]; ########### CORRECT TESTS
    then
        printf "${YELLOW}**************************${path}/${file}**************************${NC}\n"
    else                                      ########### INCORRECT TESTS
        printf "${BLUE}**************************${path}/${file}**************************${NC}\n"
    fi

    ../main < $test_file > "output/${path}/compiler_output_$file" 2> /dev/null
    compiler_res=$?
  
    ../ic18int "output/${path}/compiler_output_$file" > "output/${path}/interpret_output_$file" 2> /dev/null
    interpret_res=$?
  
    ruby -r ./ifj18.rb $test_file > "output/${path}/ruby_output_$file" 2> /dev/null
    ruby_res=$?

    if [ "$path" = 'correct_program_tests' ]; ########### CORRECT TESTS
    then

            if test $compiler_res -eq 0
            then
                diff "output/${path}/ruby_output_$file" "output/${path}/interpret_output_$file"
                diff_res=$?
                if test $diff_res -eq 0
                then
                    printf "    ${GREEN}${file}: SUCCESS${NC}\n"
                else
                    printf "    ${RED}${file}: INTERPRET FAILED ${interpret_res} ${NC}\n"
                fi
            else
                printf "    ${RED}${file}: COMPILER FAILED ${compiler_res} ${NC}\n"
            fi

    else                                      ########### INCORRECT TESTS
    
            if test $compiler_res -eq 0
            then
                if test $interpret_res -eq 0
                then
                    #diff "output/${path}/ruby_output_$file" "output/${path}/interpret_output_$file"
                    printf "    ${RED}${file}: SUCCESS${NC}\n"
                else
                    printf "    ${GREEN}${file}: INTERPRET FAILED ${interpret_res} ${NC}\n"
                fi
            else
                printf "    ${GREEN}${file}: COMPILER FAILED ${compiler_res} ${NC}\n"
            fi
    
    fi


done
done
