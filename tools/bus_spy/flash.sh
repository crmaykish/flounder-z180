#!/bin/sh
sudo mount $1 /mnt/pico
sudo cp build/z180_bus_spy.uf2 /mnt/pico
sync
sudo umount /mnt/pico