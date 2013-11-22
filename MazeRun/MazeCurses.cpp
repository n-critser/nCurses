/*
Author : Eric (Nick) Critser
Email : ec93@hunter.cuny.edu
Class : CSCI 135.003
Filename: MazeCurse.cpp
To compile :g++ -Wall -o ...-lncurses
debug with: M-x gdb ----need debug symbols set
Completeness : incomplete
*/
#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<ncurses.h>
#include"MazeCurses.h"

/* TODO: http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/keys.html */
/*TODO: Should separate movable from static tiles   */
/*
  'G' = goal
  'P' = hero
  'A' = wall
  '.' = mobileSpace

  Directions : n=neutral, s=static, l= left, r=right, u=up, d=down 
*/
Tile::Tile(int row, int col, char _flag, char _direction)
{
    y = row;
    x = col;
    flag = _flag;
    direction = _direction;
}
Tile::Tile(char _flag)
{
    y = 0;
    x = 0;
    flag = _flag;
    direction = 's';
}
Tile::Tile()
{    
    y = 0;
    x = 0;
    flag = '.';
    direction = 's';
}

Maze::Maze()
{
    std::cout<< "simple constructor called" << std::endl;
    /* ncurses initialization for terminal window */
    initscr();
    del=110000; //delay value
    //nodelay(stdscr,TRUE);
    cbreak();
    keypad(stdscr,TRUE);//initialize keyboard arrows
    noecho(); //doesn't write from buffer to screen
    curs_set(1);// invisible cursor
    getmaxyx(stdscr,maxHeight,maxWidth);//sets the window size
    /* variables for Tile specification   */
    hero = 'P';
    monster= 'M';
    wall = (char)219;// turns the wall character 'A' into a rectangle
    goal = 'G';
    space = '.';
    blank = ' ';
    
    //refresh();
}

Maze::~Maze()
{
    nodelay(stdscr,false);
    getch();
    endwin();
}


 /*
   Function: setMaze()
   Initializes maze players and still tiles  and gives values to all
   of the above.
   input: null
   output:sets up the curses window with maze
   error:maze characters must be valid
 */
void Maze::setMaze()
{
    playerCount = 0;
    goalCount = 0;
    //hero= Player('P');
    for(int y=0;y<MAPY;y++)
    {
        for(int x=0; x<MAPX;x++)
        {
            move(y,x);
            if (mazeChars[y][x] == 'A')
            {          
                addch(wall);
                
            }
            if (mazeChars[y][x]== hero)
            {
                Tile HERO1(y,x,hero,'n');
                HERO = &HERO1;
                // HERO->y=y;
                // HERO->x=x;
                // HERO->flag=hero;   
                addch(hero);
                              
            }
            else if( mazeChars[y][x]== goal)
            {
                addch(goal);
                Tile nextGoal(y,x,goal,'s');
                goals.push_back(nextGoal);
                goalCount++;
            }
             else if( mazeChars[y][x]== monster)
            {
                addch(monster);
                Tile nextMonster(y,x,monster,'n');
                monsters.push_back(nextMonster);
                monsterCount++;
            }
               
        }
    }
    move(HERO->y,HERO->x);
    refresh();

}
/*function: collision()
  detects a collision between HERO tile  and returns appropriate response
  input:null
  output: integer response (1 = monster, 3= goal, 0 = no collision, -1 = wall)

  error: 
 */ 
int  Maze::collision()
{
    for (unsigned i=0; i<monsters.size();i++)
    {  
        if (HERO->y==monsters[i].y && HERO->x==monsters[i].x)
            return 1;
    }
    for (unsigned j=0; j<goals.size();j++)
    {
        if (HERO->y==goals[j].y && HERO->x==goals[j].x)
            return 3;
    }
    for (unsigned k=0; k<walls.size();k++)
    {
        if (HERO->y==walls[k].y && HERO->x==walls[k].x)
            return -1;
    }    
    return 0;     
}
/*function: moveTile( atile)
  move a tile on the board according to keyboard input
  input:
  output:
  error:
*/
void Maze::moveHero()
{
    // getch() collects the keyboard int value to be tested below
    int tmp= getch();
    // int heroX=HERO->x;
    // int heroY=HERO->y;
    //move(HERO->y,HERO->x);

    //TODO: TEST FOR COLLISIONS and update moved to Tile!!!!
    switch(tmp)
    {
    case KEY_LEFT:
        if(HERO->direction !='r')
        {
            //HERO->direction='l';
            // move(HERO->y,HERO->x);
            //addch(blank);
            move(HERO->y,HERO->x-1);
            HERO->x= HERO->x-1;
            addch(hero);
        }
        break;
             
    case KEY_UP:
        if(HERO->direction !='d')
        {
            move(HERO->y-1,HERO->x);
            addch(hero);
            HERO->y = HERO->y -1;
            //HERO->direction='u';
        }
        break;
    case KEY_DOWN:
        if(HERO->direction !='u')
        {
            move(HERO->y+1,HERO->x);
            addch(HERO->flag);
             HERO->y = HERO->y +1;
            // HERO->direction = 'd';
        }
        break;
    case KEY_RIGHT:
        if(HERO->direction!='l')
        {
            move(HERO->y,HERO->x+1);
            addch(hero);
            HERO->x= HERO->x+1;
            //HERO->direction='r';
        }
        break;
    case KEY_BACKSPACE:
        HERO->direction='k';//k is for kill ends game
        break;
    }
    refresh();

}

/*function: start()
  starts the game and continues the event loop
  input:
  output:
  error
*/
void Maze::start()
{
    while(1)
    {
        if(monsterKill())
        {
            move(maxWidth/2-4,maxHeight/2);
            printw("game over");
            break;
        }
  
         moveHero();
        if(HERO->direction=='k')
            break;
        usleep(del);
    }
    

}

bool Maze::monsterKill()
{
    for (unsigned i=0; i<monsters.size();i++)
        {
            if (HERO->x == monsters[i].x && HERO->y == monsters[i].y)
                return true;
        }
    return false;
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
            std::cout << mazeChars[i][j] << ' ';
        std::cout << std::endl;
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
    std::vector<Node> openList;
    std::vector<Node> closedList;
    
    for(unsigned int i=0; i< monsters.size();i++)
    {
/* CANT DO IT THIS WAY NEED TO RESPECT THE WALLS!!!*/
        if((HERO->x < monsters[i].x))
        {      
            moveTileLeft(monsters[i]) ;
        }
        if((HERO->x> monsters[i].x))
        {
            moveTileRight(monsters[i]);
        }
        if((HERO->y< monsters[i].y))
        {
            moveTileUp(monsters[i]);
        }
        if((HERO->y> monsters[i].y))
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
void Maze::moveTileUp(Tile &aPlayer)
{

     int x,y;
    x= aPlayer.x;
    y= aPlayer.y;
    if (mazeChars[y-1][x]!= wall)
    {
        //temp = mazeChars[y][x];
        aPlayer.y-=1;// this doesn't make the tile a Char!!!!
        mazeChars[y-1][x]= aPlayer.flag;
        mazeChars[y][x]=space;
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
void Maze::moveTileLeft(Tile &aPlayer)
{

    int x,y;
    x= aPlayer.x;
    y= aPlayer.y;
    if (mazeChars[y][x-1]!= wall)
    {
        //temp = mazeChars[y][x];
        aPlayer.x-=1;// this doesn't make the tile a Char!!!!
        mazeChars[y][x-1]= aPlayer.flag;
        mazeChars[y][x]=space;
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
void Maze::moveTileRight(Tile &aPlayer)//how does the tile position get known?
{
    int x,y;
    x= aPlayer.x;
    y= aPlayer.y;
    if (mazeChars[y][x+1]!= wall)
    {
        //temp = mazeChars[y][x];
        aPlayer.x+=1;// this doesn't make the tile a Char!!!!
        mazeChars[y][x+1]= aPlayer.flag;
        mazeChars[y][x]=space;
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
void Maze::moveTileDown(Tile &aPlayer)
{
    int x,y;
    x= aPlayer.x;
    y= aPlayer.y;
    if (mazeChars[y+1][x]!= wall)
    {
        //temp = mazeChars[y][x];
        aPlayer.y+=1;// this doesn't make the tile a Char!!!!
        mazeChars[y+1][x]= aPlayer.flag;
        mazeChars[y][x]=space;
        //mazeChars[y][x+1]=temp;
    }    
}
/*
bool Maze::check4Goal(Tile &agoal)
{
       
    if ((hero.getXPos() == agoal.x) &&
        (hero.getYPos() == agoal.getYPos()))
    {
        //cout<<"you have reached the goal"<<endl;
        return true;
    }
    else
        return false;
}
*/



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
    int y= 0;       
    int x= 0;

    while(input >> next)
    {
        if (next == EOF)
            break;
        if (next == '\n')// at newline character y moves to next row x=0
        {
            y++;
            x=0;
            continue;
        }
        maze.mazeChars[y][x]= next;//character added to maze
        //TODO:
        //maze.tileVector.pushBack(y,x,next);  makes a vector of tiles as
        // maze is read from maze-file
        x++;
    }
    return input;
}
