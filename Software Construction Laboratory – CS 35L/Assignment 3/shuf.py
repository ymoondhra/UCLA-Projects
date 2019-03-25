#!/usr/bin/python

"""
This program outputs all the input lines in a different order. 
Each permutation of the input has an equal chance of being outputted.

For example, for the input "a \n b \n c", the output may be
"b \n a \n c", "a \n b \n c", etc . . .

This script is the python version of GNU Coreutils command "shuf".
It has the same behavior as shuf:
-i: --input-range
-n: --head-count
-r: --repeat
--help

Created by: Yash Moondhra
"""

import random, sys, string
from argparse import ArgumentParser

def convertfiletolist(filename):
    f = open(filename, 'r')
    lines = f.readlines()
    f.close()
    return lines

def convertstdintolist(f):
    lines = f.readlines()
    f.close()
    return lines

def chooseline(lines):
    return random.choice(lines)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE                                       
                                                                                
Output randomly selected lines from FILE."""

    parser = ArgumentParser()
    parser.add_argument("-n", "--head-count",
                        action="store", dest="headcount", default=0,
                        help="output HEADCOUNT lines (default is the number of \
lines of the input)")
    parser.add_argument("-r", "--repeat",
                        action="store_true", dest="repeatlines",
                        help="allows lines from the input to be repeated. Usage\
 without --head-count leads to infinite lines of output")
    parser.add_argument("-i", "--input-range",
                        action="store", dest="inputrange", default="0",
                        help="selects which lines from the input will be used")

    options, args = parser.parse_known_args(sys.argv[1:])

    input_lines = []
    #-i option
    if(options.inputrange == "0"):
        try:
            if(args[0] == "-"):
                input_lines = convertstdintolist(sys.stdin)
            else:
                input_lines = convertfiletolist(args[0])
        except:
            input_lines = convertstdintolist(sys.stdin)
                
    else:
        if(len(args) != 0):
            parser.error(" extra operand " + args[0].
                         format(args[0])) 
        input_range = options.inputrange.split('-')
        first_number = int(input_range[0])
        second_number = int(input_range[1])
        if(first_number > second_number):
            parser.error("invalid input range ".
                         format(input_range))
        numlines = second_number - first_number + 1
        input_lines = list()
        for num in range(0, numlines):
            numbar = num + first_number
            input_lines.append(str(numbar) + "\n")
    #setting the headcount
    headcount = int(options.headcount)
    if(headcount == 0):
        headcount = len(input_lines)
    #repeat
    repeat = bool(options.repeatlines)
    if(headcount > len(input_lines)  and repeat != True):
        headcount = len(input_lines)

    
    counter = headcount - 1
    while counter in range(0, headcount):
        currLine = chooseline(input_lines)
        sys.stdout.write(str(currLine))
        if(repeat == False):
            input_lines.remove(currLine)
            counter = counter - 1
        #elif(headcount != 0):
         #   headcount = headcount - 10
          #  counter = counter - 1
        
        
if __name__ == "__main__":
    main()
