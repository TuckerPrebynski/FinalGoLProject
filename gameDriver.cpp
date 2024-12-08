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
#include <fstream>
using namespace std;
Game::Game(WINDOW *BOARD, WINDOW *MENU,WINDOW *LONGMENU, WINDOW * STATS, WINDOW *ROLL, WINDOW *COMPANION, WINDOW *LEGEND)
{
    _board = Board(BOARD);
    _BOARD = BOARD;
    _MENU = MENU;
    _LONGMENU = LONGMENU;
    _STATS = STATS;
    _ROLL = ROLL;
    _LEGEND = LEGEND;
    _COMPANION = COMPANION;
    _numPlayers = 0;
    _players[4];
    _turnNum = 0;
    initTiles();
    
}
void Game::displayBoard()
{   
    displayLegend();
    _board.displayBoard(_players);
    refresh();
    wrefresh(_BOARD);
}

int Game::polePosition(Stats arrIn[4],int arrOut[4],int intIn){
    int pos = 5;
    for(int i = 0; i < 4; i++){
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
    Stats nullStat;
    nullStat.points = -1;
    for (int i = 0; i < 4; i++)
    {
        if(i < _numPlayers){
            gameStat[i] = _players[i].printStats();
        }else{
            gameStat[i] = nullStat;
        }
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

    Menu characterMenu(_LONGMENU, "Character Selection", {"Select your Character", "strength|stamina|provis|bugs"});
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
        int pChoice = characterMenu.getChoice(4, 2);
        
        _players[i] = characterMenu.processCharacterSelection(pChoice, pName, i + '1');
        displayStats();
    }
    _board.setPlayerCount(_numPlayers);
}
void Game::displayRoll(){
    int idx = 1;
    wmove(_ROLL,1,1);
    mvwaddstr(_ROLL,idx+ 0, 1," ________\n");
    mvwaddstr(_ROLL,idx+ 1, 1,"|\\       \\\n");
    mvwaddstr(_ROLL,idx+ 2, 1,"| \\       \\\n");
    mvwaddstr(_ROLL,idx+ 3, 1,"|  \\_______\\\n");
    mvwaddstr(_ROLL,idx+ 4, 1,"\\  |       |\n");
    mvwaddstr(_ROLL,idx+ 5, 1," \\ |       |\n");
    mvwaddstr(_ROLL,idx+ 6, 1,"  \\|_______|\n");
    box(_ROLL,0,0);
    wrefresh(_ROLL);
}
int Game::rollDie(){
    int roll = rand() % 6 + 1;
    int idx = 1;
    switch(roll){
        case 6:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\  o  o \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\  o  o \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\__o__o_\\\n");
        break;
        case 5:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\  o  o \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\   o   \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\__o__o_\\\n");
        break;
        case 4:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\  o  o \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\  o  o \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\_______\\\n");
        break;
        case 3:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\  o    \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\   o   \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\_____o_\\\n");
        break;
        case 2:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\  o    \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\     o \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\_______\\\n");
        break;
        case 1:
            mvwaddstr(_ROLL,idx+ 1, 1,"|\\       \\\n");
            mvwaddstr(_ROLL,idx+ 2, 1,"| \\   o   \\\n");
            mvwaddstr(_ROLL,idx+ 3, 1,"|  \\_______\\\n");
        break;
    }
    box(_ROLL,0,0);
    wrefresh(_ROLL);
    return roll;
}
vector <string> Game::getCompanion(){
    Menu companionMenu(_LONGMENU, "Companion Selection", {"Select your Companion"});
    companionMenu.getFromFile("companions.txt");
    companionMenu.displayMenu();
    int companionChoice = companionMenu.getChoice(3,2);
    return companionMenu.returnChoice(companionChoice);

}
void Game::displayFinal()
{
    box(_MENU, 0,0);
    mvwaddstr(_MENU, 0, 1, "Final Leaderboard");
    mvwaddstr(_MENU, 1,2, "age|str|stamina|provis");
    Stats gameStat[4];
    Stats nullStat;
    nullStat.points = -1;
    for (int i = 0; i < 4; i++)
    {
        if(i < _numPlayers){
            gameStat[i] = _players[i].printFinalStats();
        }else{
            gameStat[i] = nullStat;
        }
    }
    int positions[4];
    generatePositionArray(gameStat,positions);

    int offset = 0;
    for(int i = 0; i < _numPlayers; i++){
        if(gameStat[i].name != ""){        
            if(i == _turn){wattron(_MENU,A_STANDOUT);}
            // const char* cName = flatName.c_str();
            const char* cName = gameStat[i].name.c_str();
            mvwaddstr(_MENU, 2 + positions[i]*3, 1, cName);
            const char* cStat = gameStat[i].display.c_str();
            mvwaddstr(_MENU, 3 + positions[i]*3, 2, cStat);
            if(i == _turn){wattroff(_MENU,A_STANDOUT);}
            offset ++;
        }
    }
    wrefresh(_MENU);
}
#define START_COLOR 1
#define END_COLOR 2
#define PLAIN_COLOR 3
#define OASIS_COLOR 4
#define BRIDGE_COLOR 5
#define BOULDER_COLOR 6
#define TRAVELER_COLOR 6
#define CHALLENGE_COLOR 7
void Game::displayLegend()
{   box(_LEGEND,0,0);
    mvwaddstr(_LEGEND, 0, 1, "Legend");   
    start_color();
    init_pair(START_COLOR, COLOR_BLACK, COLOR_WHITE);
    init_pair(END_COLOR, COLOR_RED, COLOR_YELLOW);

    init_pair(PLAIN_COLOR, COLOR_YELLOW, COLOR_GREEN);

    init_pair(OASIS_COLOR, COLOR_CYAN, COLOR_CYAN);
    init_pair(BRIDGE_COLOR, COLOR_BLACK, COLOR_BLUE);
    init_pair(BOULDER_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(CHALLENGE_COLOR, COLOR_WHITE, COLOR_MAGENTA);
    string tilesName[]={"Start","Finish","Plains","Oasis","Bridge","Boulder","Companion","Challenge"};
    int tilesColor[] = {START_COLOR,END_COLOR,PLAIN_COLOR,OASIS_COLOR,BRIDGE_COLOR,BOULDER_COLOR,BOULDER_COLOR,CHALLENGE_COLOR};
    string tilesDisp[]={"| |","| |","| |","|~|","|=|","|o|","|&|","|?|"};
    int startRow = 2;
    int startCol = 1;
    for (int i = 0; i < 8; i++)
    {
        string disp = tilesDisp[i];
        wmove(_LEGEND,startCol+i,startRow);
        wattron(_LEGEND, COLOR_PAIR(tilesColor[i]));
        waddstr(_LEGEND, disp.c_str());
        wattroff(_LEGEND, COLOR_PAIR(tilesColor[i]));
        wmove(_LEGEND,startCol+i,startRow+4);
        waddstr(_LEGEND,tilesName[i].c_str());
    }
    wrefresh(_LEGEND);
}

void Game::pickPath(int player){
    
    Menu pathSel(_MENU, _players[player].getName().c_str(),{"Did you remember to pack?"},{"Yes","No"},{"Better stats, companion,", "Charge boldly forward"},{"and you have a map", "you get the early worms"});
    pathSel.displayMenu();
    int choice = pathSel.getChoice(3,2);
    if (choice == 0){
        _players[player].rememberToPack(500,500,1000);
        _players[player].setCompanion(getCompanion());
        displayCompanion(player);
        _board.displayBoard(_players);
    } else{
        _players[player].chargeForth();
    }
    displayStats();
}
void Game::saveStateToFile(){
    ofstream out_file("gameState.txt");
    out_file << _turnNum << endl;


}
void Game::firstTurn(){
    for(int i = 0; i < _numPlayers; i++){
        pickPath(i);
        _board.displayBoard(_players);
        turn(i);
    }
}
void Game::basicTileDisplay(string name, vector <string> text){
    Menu tileDisplay(_MENU,name,text);
    tileDisplay.displayMenu();
    tileDisplay.enterText(9,2);
}
bool Game::riddleTile(string name,vector <string> text){
    int riddleNum = rand() % _riddles.size();
    string riddle = _riddles.at(riddleNum);
    //_riddles.erase(_riddles.begin() + riddleNum);
    stringstream riddleStream(riddle);
    string temp;
    getline(riddleStream,temp,'|');
    int type = stoi(temp);
    string riddleText;
    getline(riddleStream,riddleText,'|');
    text.push_back(riddleText);
    if (type == 1){
        //free response
        string answer;
        getline(riddleStream,answer);
        Menu tileDisplay(_MENU,name,text);
        tileDisplay.displayMenu();
        if(tileDisplay.enterText(9,2) == answer){
            return true;
        } else{
            return false;
            }
    } else if(type != 0){
        //multiple choice
        riddleStream << '|';
        vector <string> riddleResponses(type);
        int answer = 0;
        for(int i = 0; i < type; i ++){
            int idx = rand() % type;
            if(riddleResponses[idx] == ""){
                if(i == 0){
                    answer = idx;
                }
                getline(riddleStream,temp,'|');
                riddleResponses[idx] = temp;
            }else{
                i--;
            }
        }
        Menu tileDisplay(_MENU,name,text,riddleResponses,{});
        tileDisplay.displayMenu();
        if(tileDisplay.getChoice(7,2) == answer){
            return true;
        } else{
            return false;
        }
    }
    return false;
}   
void Game::eventTile(string name,int player){
    
    //debug << "In event" << endl;
    string temp;
    int pPos[2] = {0,0};
    int pathType = -1;
    _players[player].getPos(pPos);
    //debug << "loaded pos" << endl;
    stringstream eventString;
    string eventText;
    string eventPass;
    string eventFail;
    // debug << _events.size() <<endl;
    int eventNum = rand() % (_events.size());
    do{
        eventNum = rand() % (_events.size());
        // debug << "eventNum: " <<eventNum << endl;
        string event = _events.at(eventNum);
        // debug << "event: " << event <<endl;
        eventString.str(event);
        getline(eventString,eventText,'|');
        getline(eventString,eventPass,'|');
        getline(eventString,eventFail,'|');
        getline(eventString,temp,'|'); 
        pathType = stoi(temp);
    }while((pathType != pPos[0])&&(pathType != 2));
    getline(eventString,temp,'|');
    int eventCondType = stoi(temp);
    getline(eventString,temp,'|');
    int eventCond = stoi(temp);
    getline(eventString,temp);
    int eventPoints = stoi(temp);
    // debug << "Loaded event" << endl;
    //_events.erase(_events.begin() + eventNum+1);
    
    
    bool condPass = testCond(eventCondType,eventCond,player);
    if(eventPoints < 0){
        if(!condPass){
            _players[player].addBugsPoints(eventPoints);
        }
    }else{
        if(condPass){
            _players[player].addBugsPoints(eventPoints);
        }
    }
    temp = (condPass) ? eventPass : eventFail;
    Menu tileDisplay(_MENU,name,{eventText,temp});
    
    tileDisplay.displayMenu();
    tileDisplay.enterText(9,2);


}
void Game::initCompanionList(string filename){
    ifstream in_file(filename);
    string temp;
    string temp2;
    getline(in_file,temp);
    stringstream line;
    while(getline(in_file,temp)){
        line.str(temp);
        getline(line,temp2,'|');
        _companions.push_back(temp2);
    }

}
bool Game::testCond(int condType, int cond, int pNum){
    Player player = _players[pNum];
    switch(condType){
        //(0 = Companion; 1 = Age; 2 = Strength; 3 = Stamina; 4 = Provisions)
        case 0:
            //companion
            if (player.getCompanion().at(0) == _companions.at(cond-1)){
                return true;
            }
            break;
        case 1:
            //age
            if(player.getAge() >= cond){
                return true;
            }
            break;
        case 2:    
            //strength
            if(player.getStrength() >= cond){
                return true;
            }
            break;
        case 3:
            //stamina
            if(player.getStamina() >= cond){
                return true;
            }            
            break;
        case 4:
            //provisions
            if(player.getProvisions() >= cond){
                return true;
            }
            break;
    }
    //did not pass condition
    return false;
}
void Game::initTiles(){
    _board.initializeBoard();
    ifstream in_file("riddles.txt");
    if(!in_file.is_open()){
        throw;
        }
    string temp;
    getline(in_file,temp);
    while(getline(in_file,temp)){
        _riddles.push_back(temp);
    }

    ifstream in_file2("events.txt");
    if(!in_file2.is_open()){
        throw;
        }
    getline(in_file2,temp);
    while(getline(in_file2,temp)){
        _events.push_back(temp);
    }
    initCompanionList("companions.txt");
}
void Game::findTraveler(string name, vector <string> text, int pNum){
    Menu tileDisplay(_MENU,name,text,{"Yes","No"},{});
            tileDisplay.displayMenu();
            if(tileDisplay.getChoice(9,2) == 0){
            _players[pNum].addProvisions(300);
            _players[pNum].addStamina(300);
            _players[pNum].addStrength(300);
            _players[pNum].setCompanion(getCompanion());
            displayCompanion(pNum);
            }
}
void Game::executeTile(char tile,int pNum, int roll){
    string name = "";
    vector <string> text;
    switch(tile){
        case 'B':
            name = "River Crossing";
            text = {"The, in retrospect, really", "poorly-constructed bridge","collapses beneath you.", "You have to backtrack to", "find the trail again.","Move back 10 and lose", "stats"};
            _players[pNum].addProvisions(-100);
            _players[pNum].addStamina(-100);
            _players[pNum].addStrength(-100);
            _players[pNum].changePos(-10);
            basicTileDisplay(name,text);
        break;
        case 'R':
            name = "Boulder Trap";
            text = {"A giant rock starts", "rolling towards you.", "You have to jump out of its","way","Move back and lose", "stamina"};
            _players[pNum].addStamina(-300);
            _players[pNum].changePos(-1 * roll);
            basicTileDisplay(name,text);
        break;
        case 'T':
            name = "Traveler";
            text = {"You find a traveler", "Pick them up?"};
            findTraveler(name,text,pNum);
        break;
        case 'O':
            name = "Oasis";
            text = {"You find an oasis", "Take an extra turn"};
            _players[pNum].addProvisions(200);
            _players[pNum].addStamina(200);
            _players[pNum].addStrength(200);
            basicTileDisplay(name,text);
            turn(pNum);
        break;
        case 'C':
            name = "Riddle";
            text = {"A strange man asks you:"};
            //basicTileDisplay(name,text);

            if(riddleTile(name,text)){
                //got riddle right
                _players[pNum].addProvisions(500);
                basicTileDisplay(name,{"Correct!","He gives you some supplies for the help","Press enter to move on"});
            } else{
                //got riddle wrong
                basicTileDisplay(name,{"Incorrect!","Better luck next time","Press enter to move on"});
            }
        break;
        case 'G':
            name = "Grasslands";
            //TODO change 3 back to 0
            if((rand() % 2) == 0){
                text = {"Nothing Happens"};
                basicTileDisplay(name,text);
            }else{
                // debug << "calling event new" << endl;
                eventTile(name,pNum);
            }
        break;
    }

        
}
void Game::turn(int pNum){
    werase(_ROLL);
    wrefresh(_ROLL);
    Menu charMenu(_MENU, _players[pNum].getName(), {"Press Enter to Roll"});
    charMenu.displayMenu();
    charMenu.enterText(3, 2);
    displayRoll();
    int roll = rollDie();
    
    _players[pNum].changePos(roll);
    _board.displayBoard(_players);
    
    refresh();
    wrefresh(_BOARD);
    int pPos[2];
    _players[pNum].getPos(pPos);
    char tile = _board.getTileType(pPos[0],pPos[1]);
    executeTile(tile,pNum,roll);
}
bool Game::runTurn(){
    int playersWon = 0;
    for(int i = 0; i < _numPlayers; i++){
        int ppos[2];
        _players[i].getPos(ppos);
        if(ppos[1] < 51){
            displayCompanion(i);
            _board.displayBoard(_players);
            turn(i);
        } else{
            playersWon += 1;
        }
    }
    if(playersWon == _numPlayers){
        return false;
    } else {return true;}
}
void Game::displayCompanion(int pNum){
    wclear(_COMPANION);
    box(_COMPANION, 0,0);
    mvwaddstr(_COMPANION, 0, 1, "Companion");
    vector <string> companion = _players[pNum].getCompanion();
    for(int i = 0; i < 3; i++){
        mvwaddstr(_COMPANION, 1+i,2, companion[i].c_str());
    }
    wrefresh(_COMPANION);
}
