#!/bin/bash

# If 'serve' arg is given, a port is actually exposed, otherwise the host
# is responsible for the server
if [ "$1" == "serve" ]; then
    port=$2
    if [ -z "$port" ]; then
        port=8080
    fi
    port_arg="-p $port:8080"
    docker run -it -p $port:8080 -u $UID -v $PWD:/vue -w /vue node:alpine npm run serve
fi

docker run -it -p $PORT:8080 -u $UID -v $PWD:/vue -w /vue node:alpine \
       npm run build-dev
