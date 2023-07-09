#!/bin/bash
gcc src/main.c -DFE_STANDALONE -O3 -o main -Wall -Wextra -std=c99 -lraylib -lm -pedantic -g
