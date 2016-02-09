
# spel: spel.cpp
# 	g++ -std=c++11 spel.cpp -o spel



CC    = g++
# CXXFLAGS = -std=c++11 -g -Wall -pedantic
CXXFLAGS = -O0 -std=c++11 -g -Wall -pedantic

ifdef USE_READLINE
LDFLAGS += -lreadline
CXXFLAGS += -DUSE_READLINE
endif


# %.o: %.cpp
# 	g++ -std=c++0x -g -Wall $*.cpp

main: main.o ItemOwner.o Room.o Player.o Actor.o GameObject.o Item.o Stats.o Equippable.o Sword.o Shoes.o Shield.o Armor.o Classes.o Serialize.o Potion.o Key.o Shop.o Callback.o CashCard.o
main.cpp: exceptions.hpp

wc:
	wc exceptions.hpp ItemOwner.{cpp,hpp} Room.{cpp,hpp} Player.{cpp,hpp} Actor.{cpp,hpp} GameObject.{cpp,hpp} Item.{cpp,hpp} Stats.{cpp,hpp} Equippable.{cpp,hpp} Sword.{cpp,hpp} Shoes.{cpp,hpp} Shield.{cpp,hpp} Armor.{cpp,hpp} Classes.{cpp,hpp} Serialize.{cpp,hpp} Potion.{cpp,hpp} Key.{cpp,hpp} Shop.{cpp,hpp} Callback.{cpp,hpp}

clean:
	rm -f *.o main
