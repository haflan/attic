#!/usr/bin/python3

# TODO: Allow giving multiple directories as arguments and check them all
# TODO: Discover directories with no unique files
# TODO: Make interactive deletion functionality?

import os
from hashlib import sha256

filefilter = ".jpg" # TODO: Make this an argument, use regex ++

hash_to_file_list = {}

def get_file_hash(fi):
    with open(fi, "rb") as f:
        return sha256(f.read()).hexdigest()

### Generate hash for all files
for rootdir, _, files in os.walk("."):
    try:
        files = [f for f in files if filefilter in f]
        for fi in files:
            fpath = os.path.join(rootdir, fi)
            fhash = get_file_hash(fpath)
            if fhash in hash_to_file_list:
                hash_to_file_list[fhash] += [os.path.abspath(fpath)]
            else:
                hash_to_file_list[fhash] = [os.path.abspath(fpath)]
    except Exception as e :
        print(e)

### Find all lists with two or more elements (i.e. duplicate files)
dls = [copies for copies in hash_to_file_list.values() if len(copies) >= 2]
if len(dls) > 0:
    print("The following files are copies of each other:")
    for duplist in dls:
        print(" = ".join(duplist))
else:
    print("No duplicates were found :)")
