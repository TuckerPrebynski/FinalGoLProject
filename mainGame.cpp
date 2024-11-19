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

int main(int argc, char **argv)
{
    srand(time(0));
    // init screen and sets up screen
    initscr();
    // init major objects
    WINDOW *board = newwin(7, 200, 2, 10);
    Board map(board, 2);
    map.initializeBoard();
    box(board, 0, 0);

    refresh();
    // print to screen

    map.displayBoard();

    wrefresh(board);

    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}