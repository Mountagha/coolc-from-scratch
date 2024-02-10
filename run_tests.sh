#!/bin/bash

for f in $(ls ./examples)
do 
    if [[ -d `pwd`"/examples/$f" ]]
    then
        echo -e "==== Running test for files in $f ====\n"
        args=""
        for f1 in $(ls `pwd`"/examples/$f")
        do 
            args=`pwd`"/examples/$f/$f1 "$args
        done
        echo "args are $args"
        ./build/coolc $args 
        if [ $? -ne 0 ]
        then
            echo "******* Failed to run test for $f ********"
            exit 1
        fi
        echo -e "\n"
    elif [[ -f `pwd`"/examples/$f" ]]
    then
        if [[ $f == *.cl ]]
        then
            echo "hein"
            echo -e "==== Running test for $f====\n"
            ./build/coolc "./examples/$f"
            if [ $? -ne 0 ]
            then
                echo "******** testing $f failed. ********" 
                exit 1
            fi
            echo -e "\n"
        fi
    fi
done