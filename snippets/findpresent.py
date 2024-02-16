#!/usr/bin/python3

import os
import sys
from hashlib import sha256

def get_file_hash(fi):
    with open(fi, 'rb') as f:
        return sha256(f.read()).hexdigest()

### Generate hash for all files
def get_hash_dict():
    hash_to_filename = {}
    for rootdir, _, files in os.walk('.'):
        try:
            #use grep on the result instead?
            #files = [f for f in files if filefilter.lower() in f.lower()]
            for fi in files:
                fpath = os.path.join(rootdir, fi)
                fhash = get_file_hash(fpath)
                if fhash not in hash_to_filename:
                    hash_to_filename[fhash] = fi
        except Exception as e :
            print(e)
    return hash_to_filename

def check_hashes(in_file):
    hashes_to_find = {}
    lines = open(in_file).read().split('\n')
    for line in lines:
        if ' ' not in line:
            continue
        fhash, fname = line.split(' ')
        hashes_to_find[fhash] = fname
    hashes_found = get_hash_dict()
    num_found = 0
    for fhash in hashes_to_find:
        if fhash not in hashes_found:
            print(hashes_to_find[fhash], f'not found ({fhash})')
        else:
            num_found += 1
    print(num_found, 'files found')

if len(sys.argv) < 2:
    print('Use: findpresent.py list | check')
    exit(1)
if sys.argv[1] == 'list':
    hash_to_filename = get_hash_dict()
    for fhash in hash_to_filename:
        print(fhash, hash_to_filename[fhash])
if sys.argv[1] == 'check':
    if len(sys.argv) < 3:
        print('Use: findpresent.py check <checksum file>')
        exit(1)
    check_hashes(sys.argv[2])
