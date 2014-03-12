#!/bin/bash
make clean
make
rmmod open2.ko
rmmod getpid2.ko
rmmod getuid2.ko
insmod open2.ko
insmod getpid2.ko
insmod getuid2.ko
