#include <iostream>
//#include <ncurses/ncurses.h>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

vector <string> splitString(vector <string> toSplit,int maxLen){
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
int main(){
    vector <string> text = {"THIS IS A LONG STRING","THIS IS ANOTHEREERERERERER LONG STRING","YOULL NEVER GUESS WHAT THIS IS"};
    vector <string> textOut;
    vector <string> textTemp;
    for(int i = 0; i < text.size();i++){
        textTemp = splitString({text.at(i)},12);
        for(int ii = 0; ii < textTemp.size();ii++){
            textOut.push_back(textTemp.at(ii));
        }
    }
    for(int i = 0; i < textOut.size();i++){
        cout << textOut.at(i) << endl;
    }
}