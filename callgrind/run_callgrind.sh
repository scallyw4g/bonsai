#! /bin/bash

rm ./callgrind*

valgrind --tool=callgrind --collect-atstart=no --instr-atstart=no ./bonsai

./process_callgrind.sh | vim -
