#!/bin/bash

source utils/colors.sh

exit_code=0

echo -e "${CYAN}${BOLD}\nTesting programs that should crash:${NC}\n"

(cd programs_that_should_crash && bash programs_that_should_crash.sh)
if [ $? -ne 0 ]
then
    exit_code=1
fi
echo -e "${CYAN}${BOLD}\nTesting programs that not should crash:${NC}\n"

(cd programs_that_should_not_crash && bash programs_that_should_not_crash.sh)

exit $exit_code