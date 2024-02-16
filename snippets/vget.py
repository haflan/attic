#!/usr/bin/python3

# This script is meant as a client for 'python3 -m http.server'
# This script either gets a given resource or returns autocomplete suggestions
# (with flag -l) based on the input argument the contents detected on the server
#
# Setup: To install and set up bash completion, run:
#   ln -s $PWD/vget.py /usr/bin/vget
# and add the following to /usr/share/bash-completion/completions/vget:
#   _vget()
#   {
#       COMPREPLY=($(vget -l ${COMP_WORDS[1]}))
#   }
#   complete -o nospace -F _vget vget

import re
import requests
import argparse

# Host aliases go here
DEFAULT_HOST='me'
HOST_URLS={
    'me': 'http://localhost:8000',
    'vetle': 'http://localhost:8000'
}


parser = argparse.ArgumentParser()
parser.add_argument('url', type=str, nargs='?')
parser.add_argument("-l", dest='list_only', action='store_true',
                    help="Actually download the resource, don't get list")
parser.set_defaults(list_only=False)
args = parser.parse_args()
url = args.url
download = not args.list_only

# TODO: Allow no arguments, and return the entire list of aliases if so

# Split the URL in host and path
# If the URL doesn't contain a path, it should be a host alias only
if not url:
    print(' '.join(HOST_URLS.keys()))
    exit()
if 'http' in url[:4]:
    pass # in this case the url does not contain an alias
if '/' in url:
    alias = url.split('/')[0]
    host = HOST_URLS[alias]
    path = url.replace(alias, '')
    url = host + path
elif url in HOST_URLS:
    alias = url
    host = HOST_URLS[url]
    path = '/'
    url = host + path
else:
    print(' '.join(alias for alias in HOST_URLS if url in alias))
    exit()

if download:
    r = requests.get(url)
    filename = url.split('/')[-1]
    open(filename, 'wb').write(r.content)
    print('--> ', filename)
    exit()

# Otherwise download the list for autocompletion
incomplete = path[path.rindex('/')+1:]
path = path[:path.rindex('/')] + '/'
url = host + path
html = requests.get(url).text
resources = re.findall('<a.+>(.+)</a>', html)
resources = [alias+path+r for r in resources if incomplete in r]
print(' '.join(resources))
