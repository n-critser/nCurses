/*
Author : Eric (Nick) Critser
Email : ec93@hunter.cuny.edu
Class : CSCI 135.003
Filename: Maze.cpp
Assignment : Prog4: Maze.cpp
To compile :g++ -Wall -o Game GameCurses.cpp MazeCurses.cpp  -lncurses
Completeness : incomplete
*/

#include"MazeCurses.h"
#include<fstream>
#include<iostream>
#include<string>
#include<cstdlib>
//#include<unistdlib>

using namespace std;

int main(int argc, char* argv[])
//int main()
{
    string mazeName;
    if (argc == 1)
    {
        cout << "No command line args given " << endl
             << "map = maze-hard.txt" << endl;
        mazeName = "maze-hard.txt";
    }
    else
        mazeName = argv[1];
    
    ifstream mazeFile1(mazeName.c_str());//ifstream only takes c-string arguments !!!
    ifstream mazeFile2("maze-easy.txt");

    Maze amaze;
    mazeFile1>>amaze;
    amaze.printMazeChars();
    amaze.setMaze();
    amaze.start();
 
    return 0;
}



