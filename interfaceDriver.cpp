#include <iostream>
#include "Player.h"
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

Menu::Menu(WINDOW *winIn, string name, vector <string> text, vector <string> choices, vector <string> choiceText){
    MENU = winIn;
    _name = name;
    _text = text;
    fixText();
    _choices = choices;
    _choiceText = choiceText;
    idx = 0;
}
Menu::Menu(WINDOW *winIn, string name, vector <string> text, vector <string> choices, vector <string> choiceText, vector <string> choiceText2){
    MENU = winIn;
    _name = name;
    _text = text;
    fixText();
    _choices = choices;
    _choiceText = choiceText;
    _choiceText2 = choiceText2;
    idx = 0;
}
Menu::Menu(WINDOW *winIn, string name, vector <string> text){
    MENU = winIn;
    _name = name;
    _text = text;
    fixText();
}
vector <string> Menu::splitString(vector <string> toSplit,int maxLen){
    string modString = toSplit.back();
    int strlen = modString.length();
    string temp;
    
    if(strlen > maxLen ){
        toSplit.pop_back();
        stringstream strin(modString);
        stringstream strout("");
        string temp2;
        string temp3;
        while(true){
            if (strout.str().size() > maxLen){
                
                getline(strin,temp3);
                temp += " ";
                temp += temp3;
                break;
            }
            if(!getline(strin,temp,' ')){
                if(!getline(strin,temp)){
                    break;
                }
            }
            temp2 = strout.str();
            strout << temp << ' ' ;
            
        }
        if(temp2 == ""){
            temp2 = temp.substr(0,maxLen);
            temp = temp.substr(maxLen);
        }
        toSplit.push_back(temp2);
        toSplit.push_back(temp);

        return splitString(toSplit,maxLen);
    }
    return toSplit;
}
void Menu::fixText(){
    vector <string> newText;
    vector <string> newTextTemp;
    string temp;
    int maxlen = getmaxx(MENU)-4;
    for(int i = 0; i < _text.size();i++){
        newTextTemp = splitString({_text.at(i)},maxlen);
        for(int ii = 0; ii < newTextTemp.size();ii++){
            newText.push_back(newTextTemp.at(ii));
        }
    }
    _text = newText;
}
void Menu::displayMenu(){
    wclear(MENU);
    box(MENU, 0, 0);
    const char* cName = _name.c_str();
    mvwprintw(MENU,0,1,cName);
    int yoff = 1;
    for(int i = 0; i < _text.size(); i++){
        yoff = (i == 0) ? yoff+2: 1;
        const char* cText = _text[i].c_str();
        mvwaddstr(MENU, 1 + i, yoff, cText);
    }
    wrefresh(MENU);
}
void Menu::changeName(string name){
    _name = name;
}
void Menu::changeChoices(vector <string> choices, vector <string> choiceText, vector <string> choiceText2){
    _choices = choices;
    _choiceText = choiceText;
    _choiceText2 = choiceText2;
}
void Menu::displayChoiceSelection(int x, int y, vector <string> choiceSet, int sep, int highlight){
    for(int i = 0; i < choiceSet.size(); i++){
        if(i == highlight){wattron(MENU,A_STANDOUT);}
        const char* cChoice = choiceSet[i].c_str();
        mvwaddstr(MENU, x + i*sep, y, cChoice);
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
    refresh();
    wrefresh(MENU);
    int ch;
    ch = wgetch(MENU);
    if(ch == KEY_DOWN){
        idx = (idx + 1)%_choices.size();
    }else if(ch == KEY_UP){
        idx = (idx + (_choices.size() - 1))%_choices.size();
    } else if(ch == '\n'){
        return idx;
    } 
    return -1;
}
void Menu::getFromFile(string fileName){
    vector <string> choices; 
    vector <string> choiceText; 
    vector <string> choiceText2;
    ifstream inFile(fileName);
    
    string temp;
    string temp1;
    string temp2;
    string temp3;

    getline(inFile,temp);
    while(getline(inFile,temp)){
        stringstream input(temp);

        getline(input,temp1,'|');
        //stupid voodoo to get ncurses to like cstrings
        choices.push_back(temp1.c_str());

        getline(input,temp2,'|');
        
        choiceText.push_back(temp2.c_str());

        getline(input,temp3);
        
        choiceText2.push_back(temp3.c_str());
    }
    
    _choices = choices;
    _choiceText = choiceText;
    _choiceText2 = choiceText2;
}
int Menu::getChoice(int x, int y){
    idx = 0;
    int choice = -1;
    do{
        choice = selectChoice(x,y);
    }while(choice == -1);
    wclear(MENU);
    wrefresh(MENU);
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
        refresh();
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
vector <string> Menu::returnChoice(int choice){
    return {_choices[choice],_choiceText[choice],_choiceText2[choice]};
}