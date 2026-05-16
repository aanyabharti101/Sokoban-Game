# Copyright 2026 Aanya Bharti

CXX = g++
CXXFLAGS = --std=c++20 -Wall -Werror -pedantic -g

SFML = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
BOOST = -lboost_unit_test_framework

all: Sokoban Sokoban.a test

Sokoban: main.o Sokoban.o
	$(CXX) $(CXXFLAGS) -o Sokoban main.o Sokoban.o $(SFML)

Sokoban.a: Sokoban.o
	ar rcs Sokoban.a Sokoban.o

test: test.o Sokoban.a
	$(CXX) $(CXXFLAGS) -o test test.o Sokoban.a $(SFML) $(BOOST)

main.o: main.cpp Sokoban.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Sokoban.o: Sokoban.cpp Sokoban.hpp
	$(CXX) $(CXXFLAGS) -c Sokoban.cpp

test.o: test.cpp Sokoban.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

lint:
	cpplint *.cpp *.hpp

clean:
	rm -f *.o Sokoban test Sokoban.a