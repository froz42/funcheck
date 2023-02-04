#!/bin/bash
source '../utils/colors.sh'
source '../utils/add_path.sh'

MAINS=`find mains -name "*.c" | sort`

exit_code=0

for main in $MAINS
do
    printf "${YELLOW}${BOLD}Testing ${WHITE}${main}${NC}: "
    
    gcc -Wall -Wextra -Werror -o test $main -lpthread &> /dev/null
    
    if [ $? -ne 0 ]
    then
        printf "${RED}${BOLD}Compilation failed${NC}\n"
        exit_code=1
        continue
    fi

    funcheck ./test &> /dev/null
    if [ $? -ne 0 ]
    then
        exit_code=1
        printf "${RED}${BOLD}KO${NC}\n"
        exit 1
    else
        printf "${GREEN}${BOLD}OK${NC}\n"
    fi
done

# clean
rm test

exit $exit_code