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

Menu::Menu(WINDOW *winIn, string name, vector <const char*> text, vector <const char*> choices, vector <const char*> choiceText){
    MENU = winIn;
    _name = name;
    _text = text;
    _choices = choices;
    _choiceText = choiceText;
    idx = -1;
}
Menu::Menu(WINDOW *winIn, string name, vector <const char*> text, vector <const char*> choices, vector <const char*> choiceText, vector <const char*> choiceText2){
    MENU = winIn;
    _name = name;
    _text = text;
    _choices = choices;
    _choiceText = choiceText;
    _choiceText2 = choiceText2;
    idx = -1;
}
Menu::Menu(WINDOW *winIn, string name, vector <const char*> text){
    MENU = winIn;
    _name = name;
    _text = text;
}
void Menu::displayMenu(){
    wclear(MENU);
    box(MENU, 0, 0);
    const char* cName = _name.c_str();
    mvwprintw(MENU,0,1,cName);
    int yoff = 1;
    for(int i = 0; i < _text.size(); i++){
        yoff = (i == 0) ? yoff+2: 1;
        mvwaddstr(MENU, 1 + i, yoff, _text[i]);
    }
}
void Menu::changeName(string name){
    _name = name;
}
void Menu::changeChoices(vector <const char*> choices, vector <const char*> choiceText, vector <const char*> choiceText2){
    _choices = choices;
    _choiceText = choiceText;
    _choiceText2 = choiceText2;
}
void Menu::displayChoiceSelection(int x, int y, vector <const char*> choiceSet, int sep, int highlight){
    for(int i = 0; i < choiceSet.size(); i++){
        if(i == highlight){wattron(MENU,A_STANDOUT);}
        mvwaddstr(MENU, x + i*sep, y, choiceSet[i]);
        if(i == highlight){wattroff(MENU,A_STANDOUT);}
    }
    //DEBUG mvwprintw(MENU, x + 4*sep, y, "index: %d",idx);
}
int Menu::selectChoice(int x, int y){
    keypad(MENU, TRUE);
    if(_choiceText.size() > 0){
        if(_choiceText2.size() > 0){
            displayChoiceSelection(x,y,_choices,3,idx);
            displayChoiceSelection(x+1,y+2,_choiceText,3,idx);
            displayChoiceSelection(x+2,y+2,_choiceText2,3,idx);
        } else{
        displayChoiceSelection(x,y,_choices,2,idx);
        displayChoiceSelection(x+1,y+2,_choiceText,2,idx);
        }
    } else{
        displayChoiceSelection(x,y,_choices,1,idx);
    }
    
    wrefresh(MENU);
    int ch;
    ch = wgetch(MENU);
    if(ch == KEY_DOWN){
        idx = (idx + 1)%4;
    }else if(ch == KEY_UP){
        idx = (idx + 3)%4;
    } else if(ch == '\n'){
        return idx;
    } 
    return -1;
}
void Menu::getFromFile(string fileName){
    vector <const char*> choices; 
    vector <const char*> choiceText; 
    vector <const char*> choiceText2;
    ifstream inFile(fileName);
    
    string temp;
    string temp2;
    getline(inFile,temp);
    while(getline(inFile,temp2)){
        stringstream input(temp2);

        getline(input,temp,'|');
        const char *cTemp = temp.c_str(); //stupid voodoo to get ncurses to like cstrings
        choices.push_back(cTemp);

        getline(input,temp,'|');
        const char *cTemp1 = temp.c_str();
        choiceText.push_back(cTemp1);

        getline(input,temp,'|');
        const char *cTemp2 = temp.c_str();
        choiceText2.push_back(cTemp2);
    }

    changeChoices(choices,choiceText,choiceText2);
}
int Menu::getChoice(int x, int y){
    int choice;
    do{
        choice = selectChoice(x,y);
    }while(choice == -1);
    return choice;
}
string Menu::enterText(int x, int y){
    keypad(MENU, TRUE);
    int input;
    string text = "";
    wattron(MENU,A_STANDOUT);
    mvwaddstr(MENU,x,y,"__________");
    wattroff(MENU,A_STANDOUT);
    wmove(MENU,x,y);
    while(1){
        wrefresh(MENU);
        input = wgetch(MENU);
        if((input == KEY_ENTER)||(input == '\n')){
            break;
        } else if(input == KEY_BACKSPACE){
            y--;
            text.pop_back();
            wmove(MENU,x,y);
        } else{
            waddch(MENU,input);
            text += input;
            y++;
        }
    }
    return text;
}
Player Menu::processCharacterSelection(int choice, string name,char piece){
    string charName = _choices[choice];
    stringstream stats(_choiceText2[choice]);
    string temp;
    getline(stats,temp,'|');
    int age = stoi(temp);
    getline(stats,temp,'|');
    int strength = stoi(temp);
    getline(stats,temp,'|');
    int stamina = stoi(temp);
    getline(stats,temp,'|');
    int provisions = stoi(temp);
    getline(stats,temp,'|');
    int bugs = stoi(temp);
    Player newPlayer(name,age,strength,stamina,provisions,bugs,piece);
    _choices.erase(_choices.begin() + choice);
    _choiceText.erase(_choiceText.begin() + choice);
    _choiceText2.erase(_choiceText2.begin() + choice);
    return newPlayer;
}