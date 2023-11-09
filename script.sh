#!/bin/bash
sudo apt install libgtk-3-dev
sudo apt-get install x11-xserver-utils
gcc `pkg-config --cflags gtk+-3.0` -o lightref lightref.c `pkg-config --libs gtk+-3.0`
./lightref
