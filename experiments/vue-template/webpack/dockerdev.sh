#!/bin/bash

# Start qumpwebdev container (assuming that qumpweb is a submodule)

DOCKERCMD="docker run -it -v $PWD:/vue -w /vue node:alpine /bin/sh -c "

if [ ! -d "node_modules" ]; then
     $DOCKERCMD "npm install; npm run build-dev"
else
     $DOCKERCMD "npm run build-dev"
fi

