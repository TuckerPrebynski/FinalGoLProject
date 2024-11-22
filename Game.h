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
    WINDOW * _STATS;
    Game(WINDOW *BOARD, WINDOW *MENU, WINDOW * STATS);
    /// @brief disply the board state
    void displayBoard();
    /// @brief run the start/player select routine
    void playerSelect();
    /// @brief Display the leaderboard
    void displayStats();
    /// @brief run through a single player's turn, given which player to run
    /// @param pNum which player to run (0-3)
    void turn(int pNum);
private:
    /// @brief populate given array of numbers 1-4 indicating the ordered positions of each player object;
    /// @param players array of Stats objects to rank
    /// @param oldPositionArray the array to output the ranks to
    void generatePositionArray(Stats players[4],int oldPositionArray[4]);
    int polePosition(Stats arrIn[4],int arrOut[4],int intIn);
    
};
#endif