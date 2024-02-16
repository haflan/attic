#!/bin/bash

# Set up a Vue application in the current directory

if [ -z "$1" ]; then
    echo "Please give name of application"
    exit
fi


docker run -it \
    --name vue-$1 \
    --env VUE_APP=$1 \
    --env DOCKER_USER=$UID \
    -v $PWD:/vue node:alpine /bin/sh \
    -C /vue/generate.sh

docker rm -f vue-$1

# Inspired by: 
#       https://daten-und-bass.io/blog/getting-started-with-vue-cli-on-docker/
