#include <iostream>
#include "Player.h"
#include "Board.h"
#include "Interface.h"
#include <ncurses/ncurses.h>
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

//Compile command
// g++ mainGame.cpp BoardDriver.cpp MenuDriver.cpp -lncurses -DNCURSES_STATIC 

class Game{
    public: 
        Board _board;
        int _numPlayers = 0;
        Player _players[4];
        WINDOW *_BOARD;
        Game(WINDOW *BOARD);
        void displayBoard();
        void playerSelect(WINDOW *menuWin);
};
Game::Game(WINDOW *BOARD){
    _board = Board(BOARD);
    _BOARD = BOARD;
    _numPlayers = 0;
    _players[4];
}
void Game::displayBoard(){
    _board.initializeBoard();
    _board.displayBoard(_players);
    wrefresh(_BOARD);
}
void Game::playerSelect(WINDOW *menuWin){
    
    
    Menu startMenu(menuWin,"Entomology Trek", 
        {"Welcome to a glorious", "safari adventure where ","you will, uh, collect bugs.","Please select # of players", "  Arrows to move,", "  Enter to select"}
        ,{"1","2","3","4"},{});

    startMenu.displayMenu();
    _numPlayers = startMenu.getChoice(7,2) + 1;
    
    Menu characterMenu(menuWin, "Character Selection",{"Select your Character","strength|stamina|provis|bugs"});
    characterMenu.getFromFile("characters.txt");
    string nameTitle = "Player 1";
    Menu playerMenu(menuWin,nameTitle,{"Enter your name: "});
    
    
    for(int i = 0; i<_numPlayers;i++){
        nameTitle[7] = i+'1';
        playerMenu.changeName(nameTitle);
        playerMenu.displayMenu();
        string pName = playerMenu.enterText(3,2);
        characterMenu.displayMenu();

        _players[i] = characterMenu.processCharacterSelection(characterMenu.getChoice(4,2),pName,i+'1');
    }
    _board.setPlayerCount(_numPlayers);
    
}
int main(int argc, char **argv)
{
    srand(time(0));
    // init screen and sets up screen
    initscr();
    noecho();
    cbreak();
    
    refresh();
    WINDOW *menuWin = newwin(15, 30, LINES/2-10, COLS/2-5);
    WINDOW *boardWin = newwin(7, 200, LINES-10, 1);
    Game gameState(boardWin); 
    gameState.playerSelect(menuWin);
    gameState.displayBoard();

    
    

    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}