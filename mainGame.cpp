#include <iostream>
#include "Player.h"
#include <ncurses/ncurses.h>
#include <random>
#include <vector>
using namespace std;

#define START_COLOR 1
#define END_COLOR 2
#define PLAIN_COLOR 3
#define OASIS_COLOR 4
#define BRIDGE_COLOR 5
#define BOULDER_COLOR 6
#define TRAVELER_COLOR 6
#define CHALLENGE_COLOR 7

// core loop:
//   getInput();
//   drawScreen();
//   playEvent();
//   change player
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
    void checkCollisions(int top[], int offset[], int pos){

    }
    /// @brief Draw players over drawn board
    /// @param players array of player objects
    /// @param player_count number of players
    void drawPlayers(Player players[], int player_count);

public:
    Board(WINDOW *win_in, int player_count);

    /// @brief Initalize board object
    void initializeBoard();
    /// @brief Display Board to board window
    void displayBoard();
};
Board::Board(WINDOW *win_in, int player_count)
{
    board = win_in;
    _player_count = player_count;
}
Tile Board::fillTile(Tile tile, char type)
{
    switch (type)
    {
    case 'G':
        tile.color = PLAIN_COLOR;
        tile.type = 'G';
        break;
    case 'O':
        tile.color = OASIS_COLOR;
        tile.type = 'O';
        tile.disp[1] = '~';
        break;
    case 'B':
        tile.color = BRIDGE_COLOR;
        tile.type = 'B';
        tile.disp[1] = '=';
        break;
    case 'R':
        tile.color = BOULDER_COLOR;
        tile.type = 'R';
        tile.disp[1] = 'o';
        break;
    case 'T':
        tile.color = TRAVELER_COLOR;
        tile.type = 'T';
        tile.disp[1] = '&';
        break;
    case 'C':
        tile.color = CHALLENGE_COLOR;
        tile.type = 'C';
        tile.disp[1] = '$';
        break;
    }
    return tile;
}
char Board::generateTile(int track, int idx)
{
    int roll = rand() % _BOARD_SIZE;
    // Grassland, 20/52 if t0, 29/52 if t1
    if (roll < (_BOARD_SIZE / 2 + track * 9))
    {
        return 'G'; // Grassland
    }
    else
    {
        roll = rand() % 20;
        // change weights based on if in first half/back half of board
        if (idx < _BOARD_SIZE / 2)
        {
            // first half
            // did not pack
            if (track == 0)
            {
                if (roll < 5)
                {
                    return 'B'; // Bridge
                }
                else if (roll < 10)
                {
                    return 'R'; // Boulder
                }
                else if (roll < 14)
                {
                    return 'T'; // Traveler
                }
                else if (roll < 15)
                {
                    return 'O'; // Oasis
                }
                else
                {
                    return 'C'; // Challenge
                }
                // did pack
            }
            else
            {
                if (roll < 4)
                {
                    return 'B'; // Bridge
                }
                else if (roll < 8)
                {
                    return 'R'; // Boulder
                }
                else if (roll < 11)
                {
                    return 'T'; // Traveler
                }
                else if (roll < 16)
                {
                    return 'O'; // Oasis
                }
                else
                {
                    return 'C'; // Challenge
                }
            }
        }
        else
        {
            // second half
            // did not pack
            if (track == 0)
            {
                if (roll < 3)
                {
                    return 'B'; // Bridge
                }
                else if (roll < 6)
                {
                    return 'R'; // Boulder
                }
                else if (roll < 10)
                {
                    return 'T'; // Traveler
                }
                else if (roll < 15)
                {
                    return 'O'; // Oasis
                }
                else
                {
                    return 'C'; // Challenge
                }
                // did pack
            }
            else
            {
                if (roll < 4)
                {
                    return 'B'; // Bridge
                }
                else if (roll < 8)
                {
                    return 'R'; // Boulder
                }
                else if (roll < 11)
                {
                    return 'T'; // Traveler
                }
                else if (roll < 14)
                {
                    return 'O'; // Oasis
                }
                else
                {
                    return 'C'; // Challenge
                }
            }
        }
    }
}
void Board::initializeTiles(int track)
{
    for (int i = 0; i < _BOARD_SIZE; i++)
    {
        Tile temp;
        temp.idx = i;
        switch (i)
        {
        case 0:
            temp.color = START_COLOR;
            temp.type = 'S';
            break;
        case _BOARD_SIZE - 1:
            temp.color = END_COLOR;
            temp.type = 'E';
            break;
        default:
            temp = fillTile(temp, generateTile(track, i));
        }
        _tiles[track][i] = temp;
    }
}
void Board::initializeBoard()
{
    for (int i = 0; i < 2; i++)
    {
        initializeTiles(i);
    }
}
void Board::drawTiles(int track)
{
    start_color();
    init_pair(START_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair(END_COLOR, COLOR_RED, COLOR_YELLOW);

    init_pair(PLAIN_COLOR, COLOR_YELLOW, COLOR_GREEN);

    init_pair(OASIS_COLOR, COLOR_CYAN, COLOR_CYAN);
    init_pair(BRIDGE_COLOR, COLOR_BLACK, COLOR_BLUE);
    init_pair(BOULDER_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(CHALLENGE_COLOR, COLOR_WHITE, COLOR_MAGENTA);
    for (int i = 0; i < _BOARD_SIZE; i++)
    {
        string disp = _tiles[track][i].disp;
        wattron(board, COLOR_PAIR(_tiles[track][i].color));
        waddstr(board, disp.c_str());
        wattroff(board, COLOR_PAIR(_tiles[track][i].color));
    }
}
void Board::drawPlayers(Player players[], int player_count)
{
    vector <int> topRoute;
    vector <int> btmRoute;
    int playerPos[2];
    
    for(int i = 0; i < player_count; i++){
        players[i].getPos(playerPos);
        int count = 0;
        int offset[2] = {0,0};
        if(playerPos[0] == 0){
            for(int i = 0; i < topRoute.size(); i++){
                if(playerPos[1] == topRoute[i]){
                    offset[0] = -1;
                    offset[1] *= -1;
                    offset[1] += count;
                    count += 1;
                }
            }
            topRoute.push_back(playerPos[1]);
        } else {
            for(int i = 0; i < btmRoute.size(); i++){
                if(playerPos[1] == btmRoute[i]){
                    offset[0] = 1;
                    offset[1] *= -1;
                    offset[1] += count;
                    count += 1;
                }
            }
            btmRoute.push_back(playerPos[1]);
        }
        mvwaddch(board,((2 + playerPos[0] * 2)+offset[0]),(3+playerPos[1]+offset[1]), players[i].getMeeple());
    }
}
void Board::displayBoard()
{
    mvwaddstr(board, 0, 1, "Game Board");
    mvwaddstr(board, 1, 2, "Didn't Pack");
    mvwaddstr(board, 3, 2, "Did Pack");
    for (int i = 0; i < 2; i++)
    {
        wmove(board, 2 + i * 2, 3);
        drawTiles(i);
    }
}

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