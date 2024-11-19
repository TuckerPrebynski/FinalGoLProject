#include <iostream>
#include <ncurses/ncurses.h>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
void getFromFile(string fileName){
    vector <const char*> choices; 
    vector <string> choiceText; 
    vector <string> choiceText2;
    ifstream inFile(fileName);
    
    string temp;
    string temp1;
    string temp2;
    string temp3;

    getline(inFile,temp);
    while(true){
        if (!getline(inFile,temp)){
            break;
        }
        cout << temp << endl;
        stringstream input(temp);
        getline(input,temp1,'|');
        const char *cTemp = temp.c_str(); //stupid voodoo to get ncurses to like cstrings
        choices.push_back(temp1.c_str());

        getline(input,temp,'|');
        const char *cTemp1 = temp2.c_str();
        choiceText.push_back(temp2.c_str());

        getline(input,temp3,'|');
        const char* cTemp2 = temp3.c_str();
        choiceText2.push_back("test");
    }
    
    for(int i = 0; i < choices.size();i++){
        cout << choices.size() << choices[i] << choiceText[i] << choiceText2[i] << endl;
    }
}
int main(){
    getFromFile("characters.txt");
}