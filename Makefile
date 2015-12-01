
# spel: spel.cpp
# 	g++ -std=c++11 spel.cpp -o spel



CC    = g++
CXXFLAGS = -std=c++11 -g -Wall -pedantic


# %.o: %.cpp
# 	g++ -std=c++0x -g -Wall $*.cpp 

main: main.o ItemOwner.o Room.o Player.o Actor.o GameObject.o Item.o 
