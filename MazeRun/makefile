#CC = g++
CXX = g++
CXXFLAGS = -Wall -Werror

objects = main.o MazeCurses.o
game:  $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -lncurses
main.o : main.cpp MazeCurses.h
	$(CXX) $(CXXFLAGS) -c -g main.cpp MazeCurses.cpp

MazeCurses.o : MazeCurses.h MazeCurses.cpp

all : game
clean :
	-rm main $(objects)    
	
	
