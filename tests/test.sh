#!/bin/bash

count=1
test_cnt=8

while [ $count -lt $test_cnt ]
do

    i="$(printf %02d $count)"
    output_test=$(./main "test$i")
    res=$?
    echo $?

    if test $res -eq 0
    then
        echo "TEST$i: SUCCES";
    else
        echo "TEST$i: FAILED";
        echo $output_test
    fi

    true $(( count++ ))
done

