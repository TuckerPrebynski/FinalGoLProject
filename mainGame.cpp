#include <iostream>
#include "Player.h"
#include "Board.h"
#include "Interface.h"
#include "Game.h"
#if defined(_WIN32)
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// core loop:
//   getInput();
//   drawScreen();
//   playEvent();
//   change player

// Compile command
//  g++ mainGame.cpp BoardDriver.cpp MenuDriver.cpp -lncurses -DNCURSES_STATIC



int main(int argc, char **argv)
{
    srand(time(0));
    // init screen and sets up screen
    initscr();
    noecho();
    cbreak();

    refresh();
    WINDOW *menuWin = newwin(15, 30, LINES / 2 - 10, COLS / 2 - 5);
    WINDOW *boardWin = newwin(7, 200, LINES - 10, 1);
    WINDOW *statsWin = newwin(15, 30, LINES - 26, 1);
    Game gameState(boardWin,menuWin,statsWin);
    refresh();
    gameState.playerSelect();
    gameState.displayBoard();
    gameState.displayStats();

    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}