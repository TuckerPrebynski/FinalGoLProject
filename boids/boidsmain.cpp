#include <iostream>
#include "Chunk.h"
#include "Boid.h"
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




//compilation comand
//g++ boidsmain.cpp BoidDriver.cpp ChunkDriver.cpp -lncurses -DNCURSES_STATIC
int main(){
    // initscr();
    // noecho();
    // cbreak();
    // refresh();
    srand(time(0));
    const int chunkFactor = 4;
    const int scale = 1;
    const int screenX = 30;//LINES;
    const int screenY = 30;//COLS;
    const int numBoids = 15;
    int maxX = screenX*scale;
    int maxY = screenY*scale;
    double chunkX = maxX/chunkFactor;
    double chunkY = maxY/chunkFactor;
    Chunk map[chunkFactor][chunkFactor];
    
    for(int i = 0; i < chunkFactor; i++){
        for(int j = 0; j < chunkFactor; j++){
            map[i][j] = Chunk(i*chunkX,j*chunkY,chunkX,chunkY);
        }
    }
    cout << "loaded chunks" << endl;
    for(int i = 0; i < chunkFactor; i++){
        for(int j = 0; j < chunkFactor; j++){
            
            if(i == 0){
                if(j < chunkFactor - 1){
                    map[i][j].addNeighbor(&map[i+1][j]);
                    map[i][j].addNeighbor(&map[i+1][j+1]);
                    map[i][j].addNeighbor(&map[i][j+1]);
                    if(j != 0){
                        map[i][j].addNeighbor(&map[i+1][j-1]);
                        map[i][j].addNeighbor(&map[i][j-1]);
                    }
                }else{
                    map[i][j].addNeighbor(&map[i+1][j]);
                    map[i][j].addNeighbor(&map[i+1][j-1]);
                    map[i][j].addNeighbor(&map[i][j-1]);
                    
                }
            }else if(i = chunkFactor - 1){
                if(j < chunkFactor - 1){
                    map[i][j].addNeighbor(&map[i-1][j]);
                    map[i][j].addNeighbor(&map[i-1][j+1]);
                    map[i][j].addNeighbor(&map[i][j+1]);
                    if(j != 0){
                        map[i][j].addNeighbor(&map[i-1][j-1]);
                        map[i][j].addNeighbor(&map[i][j-1]);
                    }
                }else{
                    map[i][j].addNeighbor(&map[i-1][j]);
                    map[i][j].addNeighbor(&map[i-1][j-1]);
                    map[i][j].addNeighbor(&map[i][j-1]);
                    
                }
            }else{
                if(j < chunkFactor - 1){
                    map[i][j].addNeighbor(&map[i-1][j]);
                    map[i][j].addNeighbor(&map[i-1][j+1]);
                    map[i][j].addNeighbor(&map[i+1][j]);
                    map[i][j].addNeighbor(&map[i+1][j+1]);
                    map[i][j].addNeighbor(&map[i][j+1]);
                    if(j != 0){
                        map[i][j].addNeighbor(&map[i-1][j-1]);
                        map[i][j].addNeighbor(&map[i+1][j-1]);
                        map[i][j].addNeighbor(&map[i][j-1]);
                    }
                }else{
                    map[i][j].addNeighbor(&map[i-1][j]);
                    map[i][j].addNeighbor(&map[i-1][j-1]);
                    map[i][j].addNeighbor(&map[i+1][j]);
                    map[i][j].addNeighbor(&map[i+1][j-1]);
                    map[i][j].addNeighbor(&map[i][j-1]);
                    
                }
            }
            

            }
            
        }
    
    cout << "referenced chunks" << endl;
    Boid boids[numBoids];
    cout << "initialized boids" << endl;
    int posx = 10;
    int posy = 10;
    for(int i = 0; i < numBoids; i++){
        //cout << "in loop" << endl;
        posx = rand() % maxX;
        posy = rand() % maxY;
        //cout << "randPos" << endl;
        boids[i].setPos(posx,posy);
        int chX = int(posx/chunkX)%chunkFactor;
        int chY = int(posy/chunkY)%chunkFactor;
        //cout<< chX << ", " << chY << endl;
        map[chX][chY].refreshBoid(&boids[i]);
    }
    //cout << "fully loaded" << endl;
    //main loop
    while(true){
        for(int i = 0; i < numBoids; i++){
            boids[i].calculateNewPos();
        }

        for(int i = 0; i < numBoids; i++){
            boids[i].update();
            boids[i].display();
        }
        // refresh();
        // getch();
    }
    

    // // pause the screen output
    // getch();
    // // deallocates memory and ends ncurses
    // endwin();
}