#!/bin/bash

for f in $(find ./tests)
do 
    if [[ -d "$f" ]]
    then
        echo -e "==== Running test for files in $f====\n"
        ./build/coolc $(ls "$f") 
        echo -e "\n"
    elif [[ -f "$f" ]]
    then
        if [[ $f == "*.cl" ]]
        then
            echo -e "==== Running test for $f====\n"
            ./build/coolc "$f"
            echo -e "\n"
        fi
    fi
done