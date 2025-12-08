# My Makefile!

clear
g++ -o game main.cpp src/player.cpp src/enemy.cpp\
    -Wall -std=c++14 -O2 \
    -I. -I~/raylib/src \
    -L. -L~/raylib/src \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic \
    -DPLATFORM_DESKTOP
 ./game
