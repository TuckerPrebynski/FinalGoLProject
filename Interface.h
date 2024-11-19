#ifndef INTERFACE_H
#define INTERFACE_H
#include <iostream>
#include "Player.h"
#include "Board.h"
#include <ncurses/ncurses.h>
#include <random>
#include <vector>
using namespace std;


class Menu{
    private:
        WINDOW *MENU;
        string _name;
        vector <const char*> _text;
        int _sizeX;
        int _sizeY;
        vector <const char*> _choices;
        vector <const char*> _choiceText;
        vector <const char*> _choiceText2;
        int idx;
        int _choiceTextMaxLines = 1;
        /// @brief interate through list of menu choices, display them with given parameters
        /// @param x start coord of the choice list
        /// @param y start coord of the choice list
        /// @param choiceSet the vector of choices to display
        /// @param sep how many lines to move between each choice (default 1)
        /// @param highlight the index of highlighted choice (default none)
        void displayChoiceSelection(int x, int y,vector <const char*> choiceSet, int sep = 1, int highlight = -1);
        /// @brief Display choices with top right corner at y,x, do a sinlge iteration of keyboard input
        /// @param x 
        /// @param y 
        /// @return choice or -1 if invalid
        int selectChoice(int x, int y);
    public:
        /// @brief Standard constructor
        /// @param winIn the WINDOW object to print to
        /// @param name the name of the window displayed on the top left
        /// @param text the text of the menu, displayed below the title
        /// @param choices set of choices to choose from
        /// @param choiceText description text displayed below each choice
        Menu(WINDOW *winIn, string name, vector <const char*> text, vector <const char*> choices, vector <const char*> choiceText);
        Menu(WINDOW *winIn, string name, vector <const char*> text, vector <const char*> choices, vector <const char*> choiceText, vector <const char*> choiceText2);
        /// @brief Text Entry Constructor
        /// @param winIn the WINDOW object to print to
        /// @param name the name of the window displayed on the top left
        /// @param text the text of the menu, displayed below the title
        Menu(WINDOW *winIn, string name, vector <const char*> text);
        
        /// @brief Display choices with top right corner at y,x. Wait until valid choice made
        /// @param x 
        /// @param y 
        /// @return valid choice 0 - number of choices possible
        int getChoice(int x, int y);
        string enterText(int x, int y);
        /// @brief draws the menu box, title, and text to window. Does not refresh
        void displayMenu();
        void changeName(string name);
        void changeChoices(vector <const char*> choices, vector <const char*> choiceText, vector <const char*> choiceText2);
        void getFromFile(string fileName);
        Player processCharacterSelection(int choice, string name, char piece);
      };
#endif