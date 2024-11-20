#include <iostream>
#include "Player.h"
#include <ncurses.h>
#include "Board.h"
#include "Interface.h"
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

class Game
{
public:
    Board _board;
    int _numPlayers = 0;
    int _turn = 0;
    Player _players[4];
    WINDOW *_BOARD;
    Game(WINDOW *BOARD);
    void displayBoard();
    void playerSelect(WINDOW *menuWin, WINDOW *statsWin);
    void displayStats(WINDOW *statsWin);

private:
    bool checkSort(Stats arrIn[4]);
    void shotgunSort(Stats arrIn[4]);
};
Game::Game(WINDOW *BOARD)
{
    _board = Board(BOARD);
    _BOARD = BOARD;
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
bool Game::checkSort(Stats arrIn[4])
{
    for (int i = 0; i < _numPlayers - 1; i++)
    {
        if (arrIn[i].points >= arrIn[i + 1].points)
        {
            return false;
        }
    }
    return true;
}
void Game::shotgunSort(Stats arrIn[4])
{
    // shotgun shot
    int sortInd = rand() % _numPlayers;
    Stats temp;
    if (arrIn[0 + sortInd].points >= arrIn[1 + sortInd].points)
    {
        temp = arrIn[1 + sortInd];
        arrIn[1 + sortInd] = arrIn[0 + sortInd];
        arrIn[0 + sortInd] = temp;
    }
}
void Game::displayStats(WINDOW *statsWin)
{
    mvwaddstr(statsWin, 0, 1, "Leaderboard");
    Stats gameStat[4];
    for (int i = 0; i < _numPlayers; i++)
    {
        gameStat[i] = _players[i].printStats();
    }
    do
    {
        shotgunSort(gameStat);
    } while (!checkSort(gameStat));

    for(int i = 0; i < _numPlayers; i++){
        if(i == _turn){wattron(statsWin,A_STANDOUT);}
        const char* cName = gameStat[i].name.c_str();
        mvwaddstr(statsWin, 1 + i*3, 1, cName);
        const char* cStat = gameStat[i].display.c_str();
        mvwaddstr(statsWin, 2 + i*3, 2, cStat);
        if(i == _turn){wattroff(statsWin,A_STANDOUT);}
    }
    wrefresh(statsWin);
}
void Game::playerSelect(WINDOW *menuWin, WINDOW *statsWin)
{

    Menu startMenu(menuWin, "Entomology Trek",
                   {"Welcome to a glorious", "safari adventure where ", "you will, uh, collect bugs.", "Please select # of players", "  Arrows to move,", "  Enter to select"}, {"1", "2", "3", "4"}, {});

    startMenu.displayMenu();
    _numPlayers = startMenu.getChoice(7, 2) + 1;

    Menu characterMenu(menuWin, "Character Selection", {"Select your Character", "strength|stamina|provis|bugs"});
    characterMenu.getFromFile("characters.txt");
    string nameTitle = "Player 1";
    Menu playerMenu(menuWin, nameTitle, {"Enter your name: "});

    for (int i = 0; i < _numPlayers; i++)
    {
        nameTitle[7] = i + '1';
        playerMenu.changeName(nameTitle);
        playerMenu.displayMenu();
        string pName = playerMenu.enterText(3, 2);
        characterMenu.displayMenu();

        _players[i] = characterMenu.processCharacterSelection(characterMenu.getChoice(4, 2), pName, i + '1');
        displayStats(statsWin);
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
    WINDOW *menuWin = newwin(15, 30, LINES / 2 - 10, COLS / 2 - 5);
    WINDOW *boardWin = newwin(7, 200, LINES - 10, 1);
    WINDOW *statsWin = newwin(15, 30, LINES - 26, 1);
    Game gameState(boardWin);
    refresh();
    gameState.playerSelect(menuWin,statsWin);
    gameState.displayBoard();
    gameState.displayStats(statsWin);

    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();
    return 0;
}