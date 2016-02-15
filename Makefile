# testandring
# spel: spel.cpp
# 	g++ -std=c++11 spel.cpp -o spel


CXX = g++
# CXX = clang++
LD = $(CXX)
CXXFLAGS = -std=c++11 -g -Wall -pedantic
# CXXFLAGS = -O0 -std=c++11 -g -Wall -pedantic

USE_READLINE = 1
USE_BOOST_REGEX = 1

ifneq "" "$(findstring 'g++',$(CXX)))"
# using g++
GCCVERSION_GTE_4_9 := $(shell expr `$(CXX) -dumpversion | cut -f1,2 -d.` '>=' 4.9)
ifeq "$(GCCVERSION_GTE_4_9)" "1"
USE_BOOST_REGEX = 0
endif
endif

ifneq ($(USE_READLINE), 0)
$(info Compiling with readline support.)
LDFLAGS += -lreadline
CXXFLAGS += -DUSE_READLINE
endif

ifneq ($(USE_BOOST_REGEX), 0)
$(info Using boost for regexes.)
LDFLAGS += -lboost_regex
CXXFLAGS += -DUSE_BOOST_REGEX
else
$(info Using C++11 regexes.)
endif

# %.o: %.cpp
# 	g++ -std=c++0x -g -Wall $*.cpp

main: main.o ItemOwner.o Room.o Player.o Actor.o GameObject.o Item.o Stats.o Equippable.o Sword.o Shoes.o Shield.o Armor.o Classes.o Serialize.o Potion.o Key.o Shop.o Callback.o CashCard.o
	$(LD) $^ $(LDFLAGS) -o $@

main.cpp: exceptions.hpp

Serialize.o: Serialize.cpp Key.hpp Actor.hpp exceptions.hpp GameObject.hpp Room.hpp Item.hpp Equippable.hpp Sword.hpp Shoes.hpp Shield.hpp Armor.hpp Classes.hpp Potion.hpp Key.hpp Shop.hpp Callback.hpp CashCard.hpp

Shop.o: Shop.hpp exceptions.hpp Serialize.hpp

wc:
	wc exceptions.hpp ItemOwner.{cpp,hpp} Room.{cpp,hpp} Player.{cpp,hpp} Actor.{cpp,hpp} GameObject.{cpp,hpp} Item.{cpp,hpp} Stats.{cpp,hpp} Equippable.{cpp,hpp} Sword.{cpp,hpp} Shoes.{cpp,hpp} Shield.{cpp,hpp} Armor.{cpp,hpp} Classes.{cpp,hpp} Serialize.{cpp,hpp} Potion.{cpp,hpp} Key.{cpp,hpp} Shop.{cpp,hpp} Callback.{cpp,hpp} CashCard.{cpp,hpp}

clean:
	rm -f *.o main

# print any makefile variable (magic)
print-%  : ; @echo $* = $($*)
