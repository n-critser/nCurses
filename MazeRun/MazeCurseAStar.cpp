//Original code taken from 
//http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
//Created by FB36 on Mon, 8 Nov 2010 (MIT)
//--with updates from comments on activestate
/*******************************************************/
// http://en.wikipedia.org/wiki/A*
/*Hacked by Nick Critser  June25th,2013
M-x compile
Compiler: g++ -Wall -g  MazeCurseAStar.cpp -lncurses
use gdb with emacs:
M-x gdb 
Run gdb (like this): gdb <MazeAStar>
*/


#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <ncurses.h>
using namespace std;

const int n=80; // horizontal size of the map XMAX
const int m=25; // vertical size size of the map YMAX
int maxHeight= m,maxWidth = n;
static int map[n][m];
static int closed_nodes_map[n][m]; // map of closed (tried-out) nodes
static int open_nodes_map[n][m]; // map of open (not-yet-tried) nodes
static int dir_map[n][m]; // map of directions
const int dir=4; // number of possible directions to go at any position
// if dir==4
static int dx[dir]={1, 0, -1, 0};
static int dy[dir]={0, 1, 0, -1};
// if dir==8
//static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
//static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

class node
{
    // current position
    int xPos;
    int yPos;
    // total distance already travelled to reach the node
    int level;//gValue in A* terms
    // priority=level+remaining distance estimate
    // fValue = gValue+ hValue(remaining distance to target)
    int priority;  // smaller: higher priority

    public:
        node(int xp, int yp, int d, int p) 
            {xPos=xp; yPos=yp; level=d; priority=p;}
    
        int getxPos() const {return xPos;}
        int getyPos() const {return yPos;}
        int getLevel() const {return level;}
        int getPriority() const {return priority;}

        /*function: updatePriority(const int xDest,const int yDest)
          changes priority by level(g-value)+ estimate(h-value)*10
          input: int x and y coordinates
          output: null-- changes priority member variable
        */
        void updatePriority(const int & xDest, const int & yDest)
        {             
             priority=level+estimate(xDest, yDest)*10; //A*
             //cout << "priority = " << priority << endl;
        }

    
        /*function: nextLevel( const int & i)
          level(g-value) in A* changes from one node to the next.  This can
          update values differently for diagonals vs straight levels
          input: int value of previous node?
          output: null
         */
        void nextLevel(const int & i) // i: direction
        {
            //diags cost more in 
            level+=(dir==8?(i%2==0?10:14):10);
            //level+=10 if dir==4;
        }
        
        // Estimation function for the remaining distance to the goal.
        /*function: estimate(const int & xDest, const int & yDest)
          estimate(h-value) is the estimated distance to the goal which has
          three possible calculations Euclidean, Manhattan, Chebyshev
          input: int x and y values for destination
          output: const int value for the distance
         */
        const int & estimate(const int & xDest, const int & yDest) const
        {
            static int xd, yd, d;
            xd=xDest-xPos;//x distance between destination and position
            yd=yDest-yPos;//y distance between destination and position         

            // Euclidian Distance
            d=static_cast<int>(sqrt(xd*xd+yd*yd));

            // Manhattan distance
            //d=abs(xd)+abs(yd);
            
            // Chebyshev distance
            //d=max(abs(xd), abs(yd));

            return(d);
        }
};
//END OF CLASS NODE*********************************
/* Determine priority (in the priority queue)
   Priority queue 
*/
bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();
}

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind( const int & xStart, const int & yStart, 
                 const int & xFinish, const int & yFinish )
{
    static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0; //pointer to a node
    static node* m0; //another pointer to a node
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    // reset the node maps
    for(y=0;y<m;y++)
    {
        for(x=0;x<n;x++)
        {
            closed_nodes_map[x][y]=0;//2d array of closed nodes
            open_nodes_map[x][y]=0;  //2d array of open nodes
        }
    }

    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getPriority(); // mark it on the open nodes map
// '->'= element selection through pointer
    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        // check out the "new" keyword for memory allocation
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), 
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish) 
        {
            // generate the path from finish to start
            // by following the directions
            string path="";
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x][y];
                c='0'+(j+dir/2)%dir;//what is this ????????WHAT IS THIS*********
                //cout << "the funny c ="<<c<< endl;
                path=c+path;
                x+=dx[j];
                y+=dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();           
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            /*
             */
            xdx=x+dx[i]; ydy=y+dy[i];

            if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx][ydy]==1 
                || closed_nodes_map[xdx][ydy]==1))
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(), 
                             n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]==0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx && 
                           pq[pqi].top().getyPos()==ydy))
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pq[pqi].pop(); // remove the wanted node
                    
                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return ""; // no route found
}
// END OF global function PATHFIND****************************
int main()
{
    srand(time(NULL));

    bool start=false;
    cout<<"do you want to start : true or false"<<endl;
    
    cout<<" "<<endl;
    cin>>start;
    while(start)
    {
  
 
    
    // Can i use the ncurses values to create map height & width? 
    // create empty map
    for(int y=0;y<m;y++) // m = vertical push
    {
        for(int x=0;x<n;x++) map[x][y]=0; //n = horizontal push
    }

    // fillout the map matrix with a '+' pattern
    for(int x=n/8;x<n*7/8;x++)
    {
        map[x][m/2]=1;
    }
    for(int y=m/8;y<m*7/8;y++)
    {
        map[n/2][y]=1;
    }
    
    // randomly select start and finish locations
    int xA, yA, xB, yB;
    switch(rand()%8)
    {
        case 0: xA=0;yA=0;xB=n-1;yB=m-1; break;
        case 1: xA=0;yA=m-1;xB=n-1;yB=0; break;
        case 2: xA=n/2-1;yA=m/2-1;xB=n/2+1;yB=m/2+1; break;
        case 3: xA=n/2-1;yA=m/2+1;xB=n/2+1;yB=m/2-1; break;
        case 4: xA=n/2-1;yA=0;xB=n/2+1;yB=m-1; break;
        case 5: xA=n/2+1;yA=m-1;xB=n/2-1;yB=0; break;
        case 6: xA=0;yA=m/2-1;xB=n-1;yB=m/2+1; break;
        case 7: xA=n-1;yA=m/2+1;xB=0;yB=m/2-1; break;
    }

    // cout<<"Map Size (X,Y): "<<n<<","<<m<<endl;
    //cout<<"Start: "<<xA<<","<<yA<<endl;
    //cout<<"Finish: "<<xB<<","<<yB<<endl;
    // get the route
    clock_t start = clock();
    string route=pathFind(xA, yA, xB, yB);
    if(route=="") cout<<"An empty route generated!"<<endl;
    clock_t end = clock();
    double time_elapsed = double(end - start);
    //cout<<"Time to calculate the route (ms): "<<time_elapsed<<endl;
    //cout<<"Route:"<<endl;
    //cout<<route<<endl<<endl;

    // follow the route on the map and display it 
    if(route.length()>0)
    {
        int j; char c;
        int x=xA;
        int y=yA;
        map[x][y]=2;
        for(int i=0;i<route.length();i++)
        {
            c =route.at(i);//gets the char at position i of route
            j=atoi(&c); //atoi(&c) takes the int value from address of c
            x=x+dx[j];
            y=y+dy[j];
            map[x][y]=3;
        }
        map[x][y]=4;
           
        initscr();
        cbreak();
        keypad(stdscr,TRUE);
        noecho();
        curs_set(1);
        getmaxyx(stdscr,maxHeight,maxWidth);
        
        nodelay(stdscr,false);
        getch();
        //endwin();
    
        // display the map with the route
        for(int y=0;y<m;y++)
        {
            for(int x=0;x<n;x++)
            {
                char space = ' ';
                char route= 'R';
                char start= 'S';
                char finish='F';
                char wall=char(219);
                
                move(y,x);
                if(map[x][y]==0)
                    addch(space);
                else if(map[x][y]==1)
                    addch(wall);
                    //cout<'O'; //obstacle
                else if(map[x][y]==2)
                    addch(start);
                    //cout<<"S"; //start
                else if(map[x][y]==3)
                    addch(route);
                    //cout<<"R"; //route
                else if(map[x][y]==4)
                    addch(finish);
                    //cout<<"F"; //finish
                napms(10); /*ncurses delay function! */
            }
            //cout<<endl;
        }
    }
    endwin();
    if(getchar()=='y')
    {
        cout << "good bye" << endl; // wait for a (Enter) keypress
        exit(1);
        
    }
    
    }
   

    return(0);
}
