/*file: ncurseMaze.cpp
  compile: g++ -Wall ncurseMaze.cpp -lncurses
*/

#include<ncurses.h>

const int y= 25;
const int x= 80;

class node
{
private:
    int yPos,xPos;

    int gValue;

    int fValue;

public:
    node(int _yPos, int _xPos, int _gVal, int _fVal)
        {yPos=_yPos; xPos=_xPos; gValue=_gVal; fValue=_fVal;}

    int getyPos() const {return yPos;}
    int getxPos() const {return xPos;}
    int getgValue() const {return gValue;}
    int getfValue() const {return fValue;}

    /*function: updatefValue(const int xDest,const int yDest)
      changes fValue by (g-value)+ (h-value)*10
      input: int x and y coordinates
    */
    void updatefValue( const int & yDest, const int & xDest)
    {             
            fValue=gValue+hValue(xDest, yDest)*10; //A*
            //cout << "priority = " << priority << endl;
    }

    /*function: setgValue( const int & _dir)
      (g-value) in A* changes from one node to the next.  This can
      update values differently for diagonals vs straight levels
      input: int value of previous node?

    */
    void setgValue(const int & _dir) // _dir: direction
        {
            //diags cost more in 
            level+=(dir==8?(_dir%2==0?10:14):10);
            //level+=10 if dir==4;
        }
    /*function: estimate(const int & xDest, const int & yDest)
      estimate(h-value) is the estimated distance to the goal which has
      three possible calculations Euclidean, Manhattan, Chebyshev
      usage: hValue(yDest, xDest);
     
    */
    const int & hValue(const int & yDest,const int & xDest, ) const
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

