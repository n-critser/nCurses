#ifndef MAZE_H
#define MAZE_H
/*
  Author : Eric (Nick) Critser
  Email : critser101@gmail.com
  Filename: Maze.h (ncurses version)
  To compile :g++ -Wall -o ...-lncurses
  Completeness : incomplete
*/

/*
  Header file for Maze Class
  Implementation in Maze.cpp
  Maze class current attributes are:
  1: Wall,  2: Goal, 3: Player Start Position, 4:Mobile Space
  Maze is deliniated by arrays of Type Char [20][26].
*/

#include<iostream>
#include<ncurses.h>
#include<vector>
#include<cstdlib>
#include<unistd.h>
//#include"Player.h"
#include"Goal.h"
//#include"Monster.h"
//using namespace std;
const int MAPY= 20;
const int MAPX= 26;
const int RECTDIM=20;

struct Tile
{
  int y,x;
  char flag;
  Tile(int row,int col, char _flag);
  Tile(char _flag);
  Tile();
};

class Maze
{

public:
  /*
    Constructor Declarations
  */
  Maze();
  ~Maze();
  void start(); //starts the game basically!
 
  /*
    Member function: drawMaze()
    draws the maze onto the canvas
  */
  char getMazeChar(int xCord, int yCord);
  void printMazeChars();
  //void setHeroStartPos(); //sets the initial hero start position X & Y
  void setMaze(); // sets all positions and fills player,goal,enemy vectors
  int getHeroX();
  int getHeroY();
  void moveTileUp(Player &aPlayer);
  void moveTileDown(Player &aPlayer);
  void moveTileLeft(Player &aPlayer);
  void moveTileRight(Player &aPlayer);
  bool check4Goal(Goal &agoal);
  //void drawHero(Canvas &canvas);
  void drawMaze(Canvas &canvas);
  //vector<Player> players;
  bool hasMonsters(); // boolean check for monsters
  void monsterChase(); //monsters find path to player
  vector<Goal> goals;
  vector<Player> monsters;
  Player hero;
    
private:
  /*
    Maze is just private for now with no member functions
    We don't want the maze to be manipulated while the game is
    running....Yet!
  */
  int goalX;
  int goalY;
  int playerCount;
  int goalCount;
  int monsterCount;

  bool collision();
  
 
  
  static int score;
  char mazeChars[MAPY][MAPX];

  friend std::istream &operator>>(std::istream &input, Maze &maze);
};


#endif
  
  
