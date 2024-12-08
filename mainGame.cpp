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
//  g++ mainGame.cpp BoardDriver.cpp interfaceDriver.cpp gameDriver.cpp -lncurses -DNCURSES_STATIC
//RECS
//Menu wi/ 5 options & min 2 secondary choices


int main()
{
    
    srand(time(0));
    // init screen and sets up screen
    initscr();
    noecho();
    cbreak();
    //debug << "ncurses init \n";
    WINDOW *menuWin = newwin(15, 30, LINES - 26, 63);
    WINDOW *legendWin = newwin(10, 15, LINES - 26, 31);
    WINDOW *menuLongWin = newwin(25, 30, LINES -26, 63);

    WINDOW *boardWin = newwin(7, 200, LINES - 10, 1);
    WINDOW *statsWin = newwin(15, 30, LINES - 26, 1);
    WINDOW *companionWin = newwin(5, 30, LINES - 16, 32);
    WINDOW *rollWin = newwin(10, 15, LINES - 26, 47);
    Game gameState(boardWin,menuWin,menuLongWin,statsWin,rollWin, companionWin,legendWin);
    //addstr("debug");  
    //getch();
    refresh();
    //debug << "windows init \n";    
    refresh();
    //if no saved game state
    gameState.playerSelect();
    //else read from file
    gameState.displayBoard();
    gameState.displayStats();
    gameState.firstTurn();
    //while game.checkwincon != true
    //run turn for every player
        //get roll
        //update positions
        //check tile
        //run tile event
        //update position/stats
        //repeat
    while(gameState.runTurn()){
        gameState.displayBoard();
        gameState.displayStats();
    }
    gameState.displayFinal();
    



    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}