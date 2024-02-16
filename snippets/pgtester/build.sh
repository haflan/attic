#!/bin/bash
GOOS=linux GOARCH=amd64 go build \
    -a -tags=prod,osusergo,netgo,sqlite_omit_load_extension \
    -ldflags '-s -w -extldflags "-static"' .
