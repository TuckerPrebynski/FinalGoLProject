#ifndef GAME_H
#include <iostream>
#include "Player.h"
#include "Board.h"
#if defined(_WIN32)
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <random>
#include <vector>
using namespace std;
#define GAME_H
class Game
{
public:
    Board _board;
    int _numPlayers = 0;
    int _turn = 0;
    Player _players[4];
    WINDOW *_BOARD;
    WINDOW *_MENU;
    WINDOW *_LONGMENU;
    WINDOW * _STATS;
    WINDOW *_ROLL;
    WINDOW *_COMPANION;
    WINDOW *_LEGEND;
    int _turnNum;
    vector <string> _riddles;
    vector <string> _events;
    vector <string> _companions;
    Game(WINDOW *BOARD, WINDOW *MENU, WINDOW *LONGMENU, WINDOW * STATS,WINDOW* ROLL, WINDOW* COMPANION,WINDOW *LEGEND);
    /// @brief disply the board state
    void displayBoard();
    /// @brief run the start/player select routine
    void playerSelect();
    /// @brief Display the leaderboard
    void displayStats();
    /// @brief run through a single player's turn, given which player to run
    /// @param pNum which player to run (0-3)
    void turn(int pNum);
    /// @brief display the die window onscreen
    void displayRoll();
    /// @brief roll the die
    /// @return number 1-6 indicating the player's roll
    int rollDie();
    /// @brief saves gamestate to file
    void saveStateToFile();
    /// @brief runs the initial path selection and first turn sequence for the given player, used only at start of game
    /// @param player player num
    void pickPath(int player);
    
    /// @brief run the first turn for each player
    void firstTurn();
    void eventTile(string name, int player);
    /// @brief Execute a full turn 
    /// @return true if not all players have finished
    bool runTurn();
    /// @brief update companion window
    /// @param pNum the current player
    void displayCompanion(int pNum);
    /// @brief Change player companion
    /// @return the companion string
    vector <string> getCompanion();
    /// @brief Display the final stats in MENU window
    void displayFinal();
    void displayLegend();
private:
    /// @brief populate given array of numbers 1-4 indicating the ordered positions of each player object;
    /// @param players array of Stats objects to rank
    /// @param oldPositionArray the array to output the ranks to
    void generatePositionArray(Stats players[4],int oldPositionArray[4]);
    void playerBattle(int p1, int p2);
    void pBWin(int winner, int loser);
    int polePosition(Stats arrIn[4],int arrOut[4],int intIn);
    void initTiles();
    void executeTile(char tile,int pNum,int roll);
    void basicTileDisplay(string name, vector <string> text);
    bool riddleTile(string name,vector <string> text);
    void findTraveler(string name, vector <string> text, int pNum);
    bool testCond(int condType, int cond, int pNum);
    void initCompanionList(string filename);
};
#endif