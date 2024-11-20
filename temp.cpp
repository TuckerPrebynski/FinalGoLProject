#include <iostream>
//#include <ncurses/ncurses.h>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
bool checkSort(int arrIn[4],int len){
    for(int i = 0; i < len-1;i++){
        if(arrIn[i] >= arrIn[i+1]){
            return false;
        }
    }
    return true;
}
void sorting(int arrIn[4], int len){
    //shotgun shot
    if(len > 1){
    int sortInd = rand()%len;
    int temp;
    if(arrIn[0+sortInd] >= arrIn[1+sortInd]){
        temp = arrIn[1+sortInd];
        arrIn[1+sortInd] = arrIn[0+sortInd];
        arrIn[0+sortInd] = temp;
    }
    cout << sortInd << ":" ;
    }
}
int polePosition(int arrIn[4],int arrOut[4],int intIn){
    int pos = 5;
    for(int i = 0; i < 4; i++){
        if(arrIn[i] <= intIn){
            pos -= 1;
        }
    }
    for(int j = 0; j < 4; j ++){
        cout << arrOut[j] << '|' << pos <<endl;
        if(pos == arrOut[j]){
            pos ++;
        }
    }
    return pos;
}
void sortArr(int arrIn[4], int arrOut[4]){
    
}
int main(){
    int myArr[4] = { 1,5,2,1 };
    int newArr[4];
    for(int i = 0; i < 4; i++){
        newArr[i] = polePosition(myArr,newArr,myArr[i]);
    }
    cout << "Sorted!" << endl;
    for(int i = 0; i<4;i++){
            cout <<myArr[i] <<' ';
        }
    cout <<endl;
    
    for(int i = 0; i<4;i++){
            cout <<newArr[i] <<' ';
        }
    cout <<endl;

}