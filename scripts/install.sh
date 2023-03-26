#!/usr/bin/bash

# Funcheck - A tool for checking functions calls return protections
# Copyright (C) 2023  Theo Matis
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

ASCCI_ART="
______                _               _    
|  ___|              | |             | |   
| |_ _   _ _ __   ___| |__   ___  ___| | __
|  _| | | | '_ \\ / __| '_ \\ / _ \\/ __| |/ /
| | | |_| | | | | (__| | | |  __/ (__|   < 
\\_|  \\__,_|_| |_|\\___|_| |_|\\___|\\___|_|\\_\\

funcheck  Copyright (C) 2023  Theo Matis
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
"

echo "$ASCCI_ART"

if [ ! -t 0 ]; then
    echo "This script must be run in a terminal"
    exit 1
fi

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

HOST_URL="https://github.com/tmatis/funcheck/releases/latest/download/funcheck"
LIBRARY_URL="https://github.com/tmatis/funcheck/releases/latest/download/libfuncheck.so"

SHELL=$(basename $SHELL)

function check_return_code {
    if [ $? -ne 0 ]; then
        echo -e "${RED}failed${NC}"
        exit 1
    else
        echo -e "${GREEN}done${NC}"
    fi
}

function confirm {
    read -p "$1 [y/N] " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
}

echo "Welcome to the funcheck installer!"

echo "Please choose an option:"
echo "I - Install/update funcheck in $HOME/.local/funcheck (latest version)"
echo "T - Install funcheck in /tmp/funcheck and open a shell with funcheck in PATH, and delete the directory on exit"
echo "U - Uninstall funcheck"
echo "Q - Quit"

read -p "Your choice: " -n 1 -r
echo

SAVE_REPLY=$REPLY

if [[ ! $SAVE_REPLY =~ ^[IiTtUuQq]$ ]]; then
    echo "Invalid choice"
    exit 1
fi

if [[ $SAVE_REPLY =~ ^[Qq]$ ]]; then
    exit 0
fi

if [[ $SAVE_REPLY =~ ^[Uu]$ ]]; then
    confirm "Are you sure you want to uninstall funcheck?"
    echo -n "Uninstalling funcheck ... "
    rm -rf $HOME/.local/funcheck
    check_return_code
    exit 0
fi

confirm "Are you sure you want to install funcheck?"

echo -n "Creating temporary host directory ... "
mkdir -p /tmp/funcheck/host
check_return_code

echo -n "Creating temporary library directory ... "
mkdir -p /tmp/funcheck/library
check_return_code

echo -n "Downloading host ... "
wget -O /tmp/funcheck/host/funcheck $HOST_URL &> /dev/null
check_return_code

echo -n "Downloading library ... "
wget -O /tmp/funcheck/library/libfuncheck.so $LIBRARY_URL &> /dev/null
check_return_code

echo -n "Setting permissions ... "
chmod +x /tmp/funcheck/host/funcheck &> /dev/null
check_return_code

if [[ $SAVE_REPLY =~ ^[Tt]$ ]]; then
    export PATH=/tmp/funcheck/host:$PATH
    echo "funcheck is temporarily installed in /tmp/funcheck"
    echo "You can now use funcheck in $SHELL for the current session"
    echo "funcheck will be deleted when you exit $SHELL"
    echo "To use funcheck permanently, run the installer again and choose option I"
    $SHELL
    echo -n "Deleting temporary directory ... "
    rm -rf /tmp/funcheck
    check_return_code
    exit 0
fi

# if no params, install funcheck in $HOME/.local/bin/funcheck

echo -n "Creating host directory ... "
mkdir -p $HOME/.local/funcheck/host
check_return_code

echo -n "Creating library directory ... "
mkdir -p $HOME/.local/funcheck/library
check_return_code

mv /tmp/funcheck/host/funcheck $HOME/.local/funcheck/host/funcheck
mv /tmp/funcheck/library/libfuncheck.so $HOME/.local/funcheck/library/libfuncheck.so

# add the host and library to PATH depending on the shell

if [ -n "$(echo $PATH | grep $HOME/.local/funcheck/host)" ]; then
    echo "funcheck path is already installed in $HOME/.local/funcheck/host"
    echo "You can now use funcheck in $SHELL"
    exit 0
fi

if [ "$SHELL" = "bash" ]; then
    echo "export PATH=$HOME/.local/funcheck/host:\$PATH" >> $HOME/.bashrc
elif [ "$SHELL" = "zsh" ]; then
    echo "export PATH=$HOME/.local/funcheck/host:\$PATH" >> $HOME/.zshrc
elif [ "$SHELL" = "fish" ]; then
    echo "set -gx PATH $HOME/.local/funcheck/host \$PATH" >> $HOME/.config/fish/config.fish
elif [ "$SHELL" = "sh" ]; then
    echo "export PATH=$HOME/.local/funcheck/host:\$PATH" >> $HOME/.profile
else
    echo "Your shell is not supported. Please add $HOME/.local/funcheck/host to your PATH"
    exit 1
fi

echo "funcheck is installed in $HOME/.local/funcheck/host"
echo "You can now use funcheck in $SHELL"

$SHELL
exit 0