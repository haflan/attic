#!/usr/bin/env lua
if #arg < 1 then
    print('Use: '..arg[0]..' <dir>')
    return
end

function Set (list)
    local set = {}
    for _, l in ipairs(list) do set[l] = true end
    return set
end

local pf = io.popen('ls -a "'..arg[1]..'"')
local matches = {}
print(pf:read("*a"))
for filename in pf:lines() do
    if filename == '.' or filename == '..' then
        goto continue
    end
    table.insert(matches, filename)
    ::continue::
end

matches = Set(matches)
print(matches)
