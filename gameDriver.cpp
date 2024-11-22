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
using namespace std;
Game::Game(WINDOW *BOARD, WINDOW *MENU, WINDOW * STATS)
{
    _board = Board(BOARD);
    _BOARD = BOARD;
    _MENU = MENU;
    _STATS = STATS;
    _numPlayers = 0;
    _players[4];
}
void Game::displayBoard()
{
    _board.initializeBoard();
    _board.displayBoard(_players);
    refresh();
    wrefresh(_BOARD);
}

int Game::polePosition(Stats arrIn[4],int arrOut[4],int intIn){
    int pos = 5;
    for(int i = 0; i < _numPlayers; i++){
        if(arrIn[i].points <= intIn){
            pos -= 1;
        }
    }
    for(int j = 0; j < 4; j ++){
        if(pos == arrOut[j]){
            pos ++;
        }
    }
    return pos;
}
void Game::generatePositionArray(Stats players[4],int oldPositionArray[4]){
    int positionArray[4] = {-1,-1,-1,-1};
    for(int i = 0; i < _numPlayers; i++){
        positionArray[i] = polePosition(players,positionArray,players[i].points);
    }
    for(int i = 0; i < 4; i++){
        oldPositionArray[i] = positionArray[i] - 1;
    }

}
void Game::displayStats()
{
    box(_STATS, 0,0);
    mvwaddstr(_STATS, 0, 1, "Leaderboard");
    mvwaddstr(_STATS, 1,2, "age|str|stamina|provis|bugs");
    Stats gameStat[4];
    for (int i = 0; i < _numPlayers; i++)
    {
        gameStat[i] = _players[i].printStats();
    }
    int positions[4];
    generatePositionArray(gameStat,positions);
    int offset = 0;
    for(int i = 0; i < _numPlayers; i++){
        if(gameStat[i].name != ""){        
            // stringstream cNamePoints;
            // cNamePoints << gameStat[i].name << " | " << to_string(gameStat[i].points);
            // string flatName;
            // cNamePoints >> flatName;
            if(i == _turn){wattron(_STATS,A_STANDOUT);}
            // const char* cName = flatName.c_str();
            const char* cName = gameStat[i].name.c_str();
            mvwaddstr(_STATS, 2 + positions[i]*3, 1, cName);
            const char* cStat = gameStat[i].display.c_str();
            mvwaddstr(_STATS, 3 + positions[i]*3, 2, cStat);
            if(i == _turn){wattroff(_STATS,A_STANDOUT);}
            offset ++;
        }
    }
    wrefresh(_STATS);
}

void Game::playerSelect()
{

    Menu startMenu(_MENU, "Entomology Trek",
                   {"Welcome to a glorious", "safari adventure where ", "you will, uh, collect bugs.", "Please select # of players", "  Arrows to move,", "  Enter to select"}, {"1", "2", "3", "4"}, {});

    startMenu.displayMenu();
    _numPlayers = startMenu.getChoice(7, 2) + 1;

    Menu characterMenu(_MENU, "Character Selection", {"Select your Character", "strength|stamina|provis|bugs"});
    characterMenu.getFromFile("characters.txt");
    string nameTitle = "Player 1";
    Menu playerMenu(_MENU, nameTitle, {"Enter your name: "});

    for (int i = 0; i < _numPlayers; i++)
    {
        nameTitle[7] = i + '1';
        playerMenu.changeName(nameTitle);
        playerMenu.displayMenu();
        string pName = playerMenu.enterText(3, 2);
        characterMenu.displayMenu();

        _players[i] = characterMenu.processCharacterSelection(characterMenu.getChoice(4, 2), pName, i + '1');
        displayStats();
    }
    _board.setPlayerCount(_numPlayers);
}
void Game::turn(int pNum){

}