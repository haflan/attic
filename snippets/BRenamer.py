#!/bin/python

# Bulk renamer, currently in a pretty specific format.
# Continue building on this to make a general bulk renamer

import sys
import os
import re
from subprocess import call

cwd = os.getcwd()
filelist = os.listdir(cwd)
filelist.sort()
if len(sys.argv) > 2:
    regout = sys.argv[2]
    regin = sys.argv[1]
else:
    regin = input("Enter regex for input files: ")
    regout = input("Enter common part for output files: ")
    # () is replaced with the unique part

rin = re.compile(regin)

matches     = []
new_names   = []
for filename in filelist:
    match = rin.search(filename)
    if match:
# Group 1 contains unique part of original file_name.
# Place this correctly in the new filename:
        print(match.group(1))
        new_nm = regout.replace("{}", match.group(1))
        matches.append(filename)
        new_names.append(new_nm)

for i in range(len(new_names)):
    print("\t{}   ->   {}".format(matches[i], new_names[i]))

reprompt = input("\nRename the above files? [y/n]: ")
if reprompt == 'y':
    print("renamin'")
    for i in range(len(new_names)):
        call(["mv","{}/{}".format(cwd, matches[i]),"{}/{}".format(cwd, new_names[i])])


