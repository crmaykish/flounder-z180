#!/bin/sh

# Note: don't call this directly, Docker will call this when it opens a container
# Start a build with the build.sh script

cd /home/pico/Workspace/flounder-z180/tools/bus_spy
rm -rf build
mkdir build
cd build
cmake ../ -DPICO_SDK_PATH=/mnt/pico/pico/pico-sdk
make -j4
cd ..
