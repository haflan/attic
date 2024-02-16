#!/bin/bash

# Build Docker image if it doesn't exist (or if 'clean' argument is given)
if [ "$1" = "clean" ]; then
    docker container prune
    docker rmi pynblinc-daw
fi

if [ -z "$(docker images | grep pynblinc-daw)" ]; then
    docker build -t pynblinc .
fi

# Run a container from the image
docker run -it -v pynblinc-daw 
