#!/usr/bin/env bash

ecode=0

for input in *.swexp.input; do
    output=${input%.input}.output
    if [ -e $output ] ; then
        real_output=$(./sweet_test $input)
        expected_output=$(cat $output)
        real_output=`echo $real_output | sed -e 's/\s//g'`
        expected_output=`echo $expected_output | sed -e 's/\s//g'`
        cmp -s <(echo $real_output) <(echo $expected_output) 
        if [ $? -ne 0 ]; then
            echo "error comparing output of './sweet_test $input; to '$output'"
            ecode=1
        fi
    fi
done

exit $ecode

