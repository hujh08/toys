#!/bin/bash

./solver.py | grep '\(range\)\|\(integers\)'      # 4, 13
echo

./solver.py 700 | grep '\(range\)\|\(integers\)'  # 4, 13
echo

./solver.py 1000 | grep '\(range\)\|\(integers\)'  #4, 13; 4, 61
echo

./solver.py 865 | grep '\(range\)\|\(integers\)'  #4, 13
echo

./solver.py 866 | grep '\(range\)\|\(integers\)'  #4, 13; 4, 61
echo