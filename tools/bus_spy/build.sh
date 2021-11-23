#!/bin/sh
# Open a Docker container and build the Pico project

# Build the "picodev" image
docker build -t picodev .

# Launch a "picodev" container and run the build script
docker run --user pico -v ~/Workspace:/home/pico/Workspace -it picodev bash /home/pico/Workspace/flounder-z180/tools/bus_spy/build_internal.sh
