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

int main(){
    int myArr[4] = { 1231251,5321,1234,0 };
    do{
        sorting(myArr);
        for(int i = 0; i<4;i++){
            cout <<myArr[i] <<' ';
        }
        cout <<endl;
    }while(!checkSort(myArr,4));
    cout << "Sorted!" << endl;
    for(int i = 0; i<4;i++){
            cout <<myArr[i] <<' ';
        }
    cout <<endl;

}