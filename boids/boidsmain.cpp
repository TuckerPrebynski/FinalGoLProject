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
    bool debug = true;
    if(!debug){
        initscr();
        noecho();
        cbreak();
        halfdelay(2);
        refresh();
    }
    srand(time(0));
    const int chunkFactor = 3;
    const int scale = 1;
    const int screenY = (LINES > 0) ? LINES : 30;
    const int screenX = (COLS > 0) ? COLS : 30;
    const int numBoids = 60;
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
    
    //cout << "map loaded" << endl;
    Boid boids[numBoids];
    
    int posx = 10;
    int posy = 10;
    
    for(int i = 0; i < numBoids; i++){
        posx = rand()%(maxX-6)+3;
        posy = rand()%(maxY-6)+3;
        boids[i].setMax(maxX,maxY);
        boids[i].setPos(posx,posy);
        boids[i].randomHeading();
        int chX = int(posx/chunkX)%chunkFactor;
        int chY = int(posy/chunkY)%chunkFactor;
        //cout << posx << ", " << posy << "| " << chX << ", " << chY << endl;
        //cout<< chX << ", " << chY << endl;
        map[chX][chY].refreshBoid(&boids[i]);
    }
    // for(int i = 0; i < chunkFactor; i++){
    //     for(int j = 0; j < chunkFactor; j++){
    //         //cout << "Chunk" << i <<"," <<j << " has: ";
    //         for(int k = 0; k < map[i][j]._boids.size();k++){
    //             cout << map[i][j]._boids[k];
    //         }
    //         cout << endl;
    //     }
    // }
    //cout << "fully loaded" << endl;
    //main loop
    int count = 0;
    while(count < 5){
        if(debug){
            count ++;
        }
    //for(int jj = 0; jj < 5; jj++){
        for(int i = 0; i < numBoids; i++){
            //boids[i].calculateNewPos();
            boids[i].calcVectors();
        }
        //cout << "calculated pos" << endl;
        for(int i = 0; i < chunkFactor; i++){
            for(int j = 0; j < chunkFactor; j++){
                map[i][j].resetBoids();
            }}
        if(!debug){
        clear();}
        //cout << "reset map" << endl;
        for(int i = 0; i < numBoids; i++){
            
            if(!debug){boids[i].display();
            }else{ boids[i].displayDebug();}
            
            boids[i].update();
        }
        if(!debug){
        refresh();}
        if(getch()=='e'){
            break;
        }
    }
    
    if(!debug){
    // pause the screen output
    getch();
    // deallocates memory and ends ncurses
    endwin();}
}