#!/usr/bin/env lua
local git_root = io.popen('printf $(git rev-parse --show-toplevel 2> /dev/null || pwd)'):read("*a")
print(git_root)
local root_list = io.popen('ls -A "'..git_root..'"'):read("*a")
print(root_list)
