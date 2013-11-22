/*
 * Code taken from the Book
 * Programmer's Guide to nCurses
 * http://www.c-for-dummies.com/ncurses/
 *
 *  Created on: Nov 22, 2013
 *      Author: chaos
 */
#include <ncurses.h>

int main(void){
	WINDOW *top, *bottom;

	int maxx, maxy, halfx, halfy, rc;
	char text1[] = "Lorem ipsum dolor sit amet, consectetuer\
				adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet\
				dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis\
				nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex\
				ea commodo consequat."	;
	char text2[] = "Four score and seven years ago our fathers\
                brought forth on this continent, a new nation, conceived in Liberty, and\
                dedicated to the proposition that all men are created equal.";
	initscr();

	/* get window sizes*/
	getmaxyx(stdscr, maxy, maxx);
	halfy = maxy >>1;
	halfx = maxx >>1;

	/*build windows*/
	top = newwin(halfy, maxx, 0,0);
	bottom = newwin(halfy, halfx, halfy, halfx);

	waddstr(top,text1); /* add text1 to the top window*/
	wrefresh(top);
	waddstr(bottom,text2); /* add text2 to the bottom window*/
	wrefresh(bottom);

	/* wait for key press */
	getch();

	/* copy text from top to bottom */
	rc = copywin(top,bottom, 0,0,0,0,4,12,FALSE);
	wrefresh(bottom);
	endwin();
	return 0;
}



