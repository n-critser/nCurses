/*
Author : Eric (Nick) Critser
Email : ec93@hunter.cuny.edu
Class : CSCI 135.003
Filename: Maze.cpp
Assignment : Prog4: Maze.cpp
To compile :g++ -Wall -o ?? $(~jepst/include/cli)
Completeness : incomplete
*/
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include"Maze.h"
#include"Player.h"
#include"Goal.h"
#include"Monster.h"
using namespace std;
//const int MAPX=20;
//const int MAPY=26;
/*
  'G' = goal
  'P' = heroStart
  'A' = wall
  '.' = mobileSpace
*/
Maze::Maze(){ cout<< "simple constructor called" << endl;};

/*
  Currently this way of setting values doesn't work.  Values aren't available
  until after the maze is put in
*/
/*
Maze::Maze(char _mazeChars[MAPY][MAPX])
{
     mazeChars= _mazeChars;
}
*/

 /*
   Function: setMaze()
   Initializes maze players and still tiles  and gives values to all
   of the above.
   input: null
   output:null
   error:map characters must be valid
 */
void Maze::setMaze()
{
    playerCount = 0;
    goalCount = 0;
    hero= Player('P');
    for(int y=0;y<MAPY;y++)
    {
        for(int x=0; x<MAPX;x++)
        {
            if (mazeChars[y][x]=='P')
            {
                
                hero.setXPos(x);
                hero.setYPos(y);
                //heroX=x;
                //heroY=y;
                              
            }
            else if( mazeChars[y][x]=='G')
            {
                Goal nextGoal(x, y);
                goals.push_back(nextGoal);
                goalCount++;
            }
             else if( mazeChars[y][x]=='M')
            {
                Player nextMonster('M',x, y);
                monsters.push_back(nextMonster);
                monsterCount++;
            }
               
        }
    }

}

/*function: printMazeChars()
  Prints out a 2d character representation of the map
  input: null
  ouput: prints int stdout the map characters
  error: valid MAPY and MAPX values so no run off in array
*/
void Maze::printMazeChars()
{
    for(int i=0;i<MAPY;i++)
    {
        for(int j=0; j<MAPX;j++)
            cout << mazeChars[i][j] << ' ';
        cout << endl;
    }
    
}

/*
  Function: getMazeChar()
  input: int xcoordinate, int ycoordinate
  ouput: character on map associated to x and y
  error: invalid x and y
  JUST FOR DEBUGGING MOVEMENT
*/
char Maze::getMazeChar(int xCord, int yCord)
{
    int x,y;
    x= xCord;
    y= yCord;
    char target= mazeChars[y][x];
    return target;
}

/*Function: drawMaze(&canvas)
  input: a valid canvas reference
  output: null, the maze is drawn on the canvas
  error: valid canvas, valid maze and correct tile dimensions
*/
void Maze::drawMaze(Canvas &canvas)
{
    char next;
    
    
    for(int y= 0; y<MAPY; y++)
    {
        int drawY=0, drawX=0;
        for (int x=0; x<MAPX; x++)
        {
            next = mazeChars[y][x];
            //cout << "i and j = " << i << ' ' << j
            //     << "char = " << next << " - " ;
            drawY=y*RECTDIM;
            drawX=x*RECTDIM;
            if (next == 'A')
                canvas.DrawFillRect(drawX,drawY,RECTDIM,RECTDIM,0,0,1);
            else if (next =='G')// REMOVE THIS DRAW ...REDUNDANT
            {
                canvas.DrawFillRect(drawX,drawY,RECTDIM,RECTDIM,0,0,1);
                //canvas.DrawCircle(draw+6,drawI+6,6);
                goalX=drawX;
                goalY=drawY;
            }
            //   else if (next =='P')
            //  canvas.DrawFillRect(drawJ,drawI,20,20,1,1,1);
            else
                canvas.DrawFillRect(drawX,drawY,RECTDIM,RECTDIM,0,0,0);

        }
    }

}
/*Function:hasMonsters()
  gives a boolean answer to the question:
  Does this maze have monsters?
  input: null
  ouput: boolean
  error: none
*/
bool Maze::hasMonsters()
{
    if (monsterCount>0)
        return true;
    else
        return false;
}

struct Node
{
    int xPos;//x position on the map
    int yPos;//y position on the map
    int gVal;
    int hVal;
    //int fVal = gVal + hVal;
    Node (): xPos(0), yPos(0), gVal(0), hVal(0){}
     ~Node(){}//don't know Frack about destructors but appently ~does that
};

// Maze::findPath(object , object) finds the shortest path on the map between the two
// objects...
/*
  Function: monsterChase()
  I hope this is where the pathfinding goes!!!
  input: null
  output: null
  error: if monsterCount=0 this shouldn't be called
*/

//TODO:  NEED TO CHANGE ALL THE DIRECTIONAL FUNCTIONS TO TAKE
// AND X AND Y AND MOVE THAT ELEMENT ACCORDINGLY
// THEN USE THOSE FUNCTIONS IN THE MONSTERCHASE HERE
// OR IN GAME.CPP
void Maze::monsterChase()
{
    /*  */
    vector<Node> openList;
    vector<Node> closedList;
    
    for(unsigned int i=0; i< monsters.size();i++)
    {
/* CANT DO IT THIS WAY NEED TO RESPECT THE WALLS!!!*/
        if((hero.getXPos() < monsters[i].getXPos()))
        {      
            moveTileLeft(monsters[i]) ;
        }
        if((hero.getXPos()> monsters[i].getXPos()))
        {
            moveTileRight(monsters[i]);
        }
        if((hero.getYPos()< monsters[i].getYPos()))
        {
            moveTileUp(monsters[i]);
        }
        if((hero.getYPos()> monsters[i].getYPos()))
        {
            moveTileDown(monsters[i]);
        }
        

    }

        
}
/*
  Function: moveHeroUp()
  moves the hero 'P' character one space up
  input:Null
  output:Null
  error:must return after change of position
*/
void Maze::moveTileUp(Player &aPlayer)
{

     int x,y;
    x= aPlayer.getXPos();
    y= aPlayer.getYPos();
    if (mazeChars[y-1][x]!= 'A')
    {
        //temp = mazeChars[y][x];
        aPlayer.setYPos(y-1);// this doesn't make the tile a Char!!!!
        mazeChars[y-1][x]= aPlayer.getCallSign();
        mazeChars[y][x]='.';
        //mazeChars[y][x+1]=temp;
    }
    /*
    /char next;
    for(int y= 0; y<MAPY; y++)
    {
        for (int x=0; x<MAPX; x++)
        {
            
            if (mazeChars[y][x] =='P'&& (mazeChars[y-1][x]!= 'A'))
            {
                cout<< "entering move up" << endl;
                mazeChars[y-1][x]='P';
                mazeChars[y][x]='.';
                heroY=y-1;
                heroX=x;
                return;
            }
        }
    }
    */
}

/*
  Function: moveHeroLeft()
  moves the hero 'P' character one space left
  input:Null
  output:Null
  error:must return after change of position
*/
void Maze::moveTileLeft(Player &aPlayer)
{

    int x,y;
    x= aPlayer.getXPos();
    y= aPlayer.getYPos();
    if (mazeChars[y][x-1]!= 'A')
    {
        //temp = mazeChars[y][x];
        aPlayer.setXPos(x-1);// this doesn't make the tile a Char!!!!
        mazeChars[y][x-1]= aPlayer.getCallSign();
        mazeChars[y][x]='.';
        //mazeChars[y][x+1]=temp;
    }
    /*
    for(int y= 0; y<MAPY; y++)
    {
        for (int x=0; x<MAPX; x++)
        {
             if (mazeChars[y][x] =='P'&& (mazeChars[y][x-1]!= 'A'))
            {
                cout<< "entering move down" << endl;
                mazeChars[y][x-1]='P';
                mazeChars[y][x]='.';
                heroY=y;
                heroX=x-1;
                return;
            }
        }
    }
    */
}
/*
  Function: moveHeroRight()
  moves the hero 'P' character one space left
  input:Null
  output:Null
  error:must return after change of position
*/
void Maze::moveTileRight(Player &aPlayer)//how does the tile position get known?
{
    int x,y;
    x= aPlayer.getXPos();
    y= aPlayer.getYPos();
    if (mazeChars[y][x+1]!= 'A')
    {
        //temp = mazeChars[y][x];
        aPlayer.setXPos(x+1);// this doesn't make the tile a Char!!!!
        mazeChars[y][x+1]= aPlayer.getCallSign();
        mazeChars[y][x]='.';
        //mazeChars[y][x+1]=temp;
    }

}

/*
  Function: moveHeroDown()
  moves the hero 'P' character one space left
  input:Null
  output:Null
  error:must return after change of position
*/
void Maze::moveTileDown(Player &aPlayer)
{
    int x,y;
    x= aPlayer.getXPos();
    y= aPlayer.getYPos();
    if (mazeChars[y+1][x]!= 'A')
    {
        //temp = mazeChars[y][x];
        aPlayer.setYPos(y+1);// this doesn't make the tile a Char!!!!
        mazeChars[y+1][x]= aPlayer.getCallSign();
        mazeChars[y][x]='.';
        //mazeChars[y][x+1]=temp;
    }    
}

bool Maze::check4Goal(Goal &agoal)
{
       
    if ((hero.getXPos() == agoal.getXPos()) &&
        (hero.getYPos() == agoal.getYPos()))
    {
        //cout<<"you have reached the goal"<<endl;
        return true;
    }
    else
        return false;
}
        

/*
Hero position values returned by the following accessor function
*/
/*int Maze::getHeroX()
{
    return heroX;
}
int Maze::getHeroY()
{
    return heroY;
}
*/


/*Extraction operator for the Maze read from file use 
 *students2.cpp as a basis for this 
 *input: istream reference input and Maze instance reference
 *output: information to fill a preconstructed Maze reference
 *error: invalid input stream or Maze instance reference
 */
std::istream &operator>>(std::istream &input, Maze &maze)
{
    char next;
    int x= 0;
    int y= 0;
    while(input >> next)
    {
        if (next == EOF)
            break;
        if (next == '\n')// at newline character y moves to next row x=0
        {
            x=0;
            y++;
            continue;
        }
        maze.mazeChars[y][x]= next;//character added to maze
        x++;
    }
    return input;
}
