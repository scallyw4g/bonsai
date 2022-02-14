#! /bin/bash

RunningInInteractiveTerminal=0

if [ -t 1 ]; then
 RunningInInteractiveTerminal=1
fi
