#CC = g++
CXX = g++
CXXFLAGS = -Wall #-Werror

objects = main.o 
main:  $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -lncurses
main.o : main.cpp
	$(CXX) $(CXXFLAGS) -c -g main.cpp 

all : main
clean :
	-rm a.out main $(objects)    
	
	
