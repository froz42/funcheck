#!/bin/bash

file=$1

if [ -z "$file" ]; then
    echo "Usage: $0 <file>"
    exit 1
fi

REGEX="DEFINE_HOOK_FUNCTION\(\w+, (\w+)," # regex to match the function name

while read line; do
    # if line  does not start with DEFINE_HOOK_FUNCTION skip it
    if [[ $line != "DEFINE_HOOK_FUNCTION("* ]]; then
        continue
    fi

    # extract the function name
    if [[ $line =~ $REGEX ]]; then
        func=${BASH_REMATCH[1]}
        echo "- $func"
    fi
done < $file