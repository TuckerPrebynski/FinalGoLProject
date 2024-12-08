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



int main()
{
    
    srand(time(0));
    // init screen and sets up screen
    initscr();
    noecho();
    cbreak();
    WINDOW *menuWin = newwin(15, 30, 1, COLS / 2 - 5);
    WINDOW *menuLongWin = newwin(25, 30, 1, COLS / 2 - 5);

    WINDOW *boardWin = newwin(7, 200, 17, 1);
    WINDOW *statsWin = newwin(15, 30, 1, 1);
    WINDOW *companionWin = newwin(5, 30, 11, 32);
    WINDOW *rollWin = newwin(10, 15, 1, 31);
    Game gameState(boardWin,menuWin,menuLongWin,statsWin,rollWin, companionWin);
    refresh();
    refresh();
    gameState.playerSelect();
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
    gameState.saveStateToFile();



    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}