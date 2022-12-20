#!/bin/bash

# get absolute path of the ../host directory
HOST_PATH=$(cd ../../host && pwd)

export PATH=:$HOST_PATH:$PATH