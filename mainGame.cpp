#include <iostream>
#include "Player.h"
#include "Board.h"
#include <ncurses/ncurses.h>
#include <random>
#include <vector>
using namespace std;



// core loop:
//   getInput();
//   drawScreen();
//   playEvent();
//   change player

//Compile command
// g++ mainGame.cpp BoardDriver.cpp -lncurses -DNCURSES_STATIC 


/// @brief iterate through choices on a list, and see if the given mouse pos hits one
/// @param x mouse x
/// @param y mouse y
/// @param winx window x
/// @param winy window y
/// @param posx first choice x
/// @param posy first choice y
/// @param clength length of longest choice
/// @param ccount number of choices
/// @return returns number of choice selected, or -1 if not a hit.
int reportChoice(int x, int y, int winx, int winy, int posx, int posy, int clength, int ccount){
    int topCornerx = winx+posx;
    int topCornery = winy+posy;
    for(int i = 0; i< ccount ;i ++){
        if(y == (topCornery+i)){
            if( (x >= topCornerx) &&(x < topCornerx+clength)){
                return i;
            }
            break;
        }
    }
    return -1;            

}

void startGame(WINDOW *menu){
    int selfx, selfy;
    getbegyx(menu,selfy,selfx);
    box(menu, 0, 0);
    mvwaddstr(menu, 0, 1, "Entomology Trek");
    mvwaddstr(menu, 2, 4, "Welcome to a ");
    mvwaddstr(menu, 3, 2, "glorious safari ");
    mvwaddstr(menu, 4, 2, "adventure where ");
    mvwaddstr(menu, 5, 2, "you will, uh, ");
    mvwaddstr(menu, 6, 2, "collect bugs.");
    mvwaddstr(menu, 7, 4, "Please select ");
    mvwaddstr(menu, 8, 2, "number of players");
    mvwaddstr(menu, 9, 4, "1");
    mvwaddstr(menu, 10, 4, "2");
    mvwaddstr(menu, 11, 4, "3");
    mvwaddstr(menu, 12, 4, "4");

    mousemask(ALL_MOUSE_EVENTS, NULL);
    MEVENT event;

    while(1)
	{	int c = wgetch(menu);
		switch(c)
		{	case KEY_MOUSE:
			if(getmouse(&event) == OK)
			{	/* When the user clicks left mouse button */
				if(event.bstate & BUTTON1_PRESSED)
				{	int pChoice = reportChoice(event.x + 1, event.y + 1,selfx,selfy,12,4,4,4);
					
					mvwprintw(menu,3, 1, "Choice made is : %d", event.x);
					wrefresh(menu); 
				}
			}
			//break;
		}
	}
    wrefresh(menu);
}
int main(int argc, char **argv)
{
    srand(time(0));
    // init screen and sets up screen
    initscr();
    WINDOW *startMenu = newwin(15, 20, LINES/2-7, COLS/2-5);
    WINDOW *board = newwin(7, 200, LINES-10, 1);
    // init major objects
    Board map(board, 2);
    map.initializeBoard();
    


    refresh();
    startGame(startMenu);
    
    // print to screen

    map.displayBoard();

    wrefresh(board);

    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}