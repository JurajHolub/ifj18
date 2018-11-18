#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

if [ ! -d "output" ]; then
    mkdir "output"
fi

for path in correct_program_tests/*
do
    file=`basename "$path"`

    ./main < $path > "output/compiler_output_$file" 2> /dev/null
    compiler_res=$?
  
    ./ic18int "output/compiler_output_$file" > "output/interpret_output_$file"
    interpret_res=$?
  
    ruby -r ./ifj18.rb $path > "output/ruby_output_$file"
    ruby_res=$?

    if test $compiler_res -eq 0
    then
        diff "output/ruby_output_$file" "output/interpret_output_$file"
        diff_res=$?
        if test $diff_res -eq 0
        then
            printf "${GREEN}${file}: SUCCESS${NC}\n"
        else
            printf "${RED}${file}: FAILED${NC}\n"
            diff "output/ruby_output_$file" "output/interpret_output_$file" #reprint fail
        fi
    fi

done
