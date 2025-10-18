#! /usr/bin/env bash

# This tells ctags abouit the poof macro, which frequently confuses its parser
ctags -R -I poof+ .
