#include <iostream>
#include "Player.h"
#include <ncurses/ncurses.h>
#include <random>
#include <vector>
using namespace std;
#ifndef BOARD_H
#define BOARD_H
struct Tile
{
    int color = 1;
    int idx = 1;
    char type = ' ';
    string disp = "| |";
};
class Board
{
private:
    
    static const int _BOARD_SIZE = 52;
    Tile _tiles[2][_BOARD_SIZE];
    WINDOW *board;
    int _player_count;
    /// @brief populate and return tile object based on given type
    /// @param tile tile object to be modified
    /// @param type what kind of tile we want
    /// @return the modifies tile object
    Tile fillTile(Tile tile, char type);
    /// @brief Generate tile color/type for given track
    /// @param track which track the tile is for
    /// @param idx index of tile in board
    /// @return the type of tile generated
    char generateTile(int track, int idx);
    /// @brief Initialize all tile structs with type and color
    /// @param board_index Which track to initialize
    void initializeTiles(int board_index);
    /// @brief Draw tiles to board window object
    /// @param track which track to draw
    void drawTiles(int track);
    void checkCollisions(int top[], int offset[], int pos)
    {
    }
    /// @brief Draw players over drawn board
    /// @param players array of player objects
    void drawPlayers(Player players[]);

public:
    Board();
    Board(WINDOW *win_in);
    /// @brief Initalize board object
    void initializeBoard();
    /// @brief Display Board to board window
    void displayBoard(Player players[]);
    void setPlayerCount(int player_count);
};
#endif