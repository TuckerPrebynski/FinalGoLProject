#include <iostream>
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
#ifndef BOID_H
#define BOID_H
#include "Chunk.h"
struct BoidRef{
    double heading[2];
    double relationVector[2];
    double position[2];
    double dist;
};
struct WeightedHeading{
    double heading[2];
    double weight;
};
class Chunk;
class Boid{
    private:
    double _x;
    double _y;
    double _heading[2];
    double _speed = .5;
    int MAXX;
    int MAXY;
    const int distThresh = 20;
    const int edgeAvoidance = 5;
    Chunk *_chunk; 
    double _newx;
    double _newy;
    double _newHeading[2];
    vector <BoidRef> _neighbors;
    BoidRef _avgNeighbor;
    char getDisplay();
    vector <double> headingToVector(double h[2], double w);
    WeightedHeading screenY();
    WeightedHeading screenX();
    WeightedHeading separation();
    WeightedHeading alignment();
    WeightedHeading cohesion();
    public:
    void displayDebug();
    void calcVectors();
    void setMax(int maxX,int maxY);
    double getx(){return _x;}
    double gety(){return _y;}
    BoidRef getRef(double x, double y);
    void randomHeading(){_heading[0] = rand()%10 - 5; _heading[1] = rand()%10 - 5;}
    void setChunk(Chunk *chunk){_chunk = chunk;} //cout << "chunkSet" << chunk << endl;}
    void findNeighbors();
    double getDistance(double x, double y){return sqrt((_x-x)*(_x-x) + (_y-y)*(_y-y));}
    bool checkDistance(double x, double y){
        
        double dist = getDistance(x,y);
        return (  dist < distThresh);}
    void avgNeighbors();
    void calculateNewPos();
    void update();
    void setPos(int x, int y){_x = x; _y = y;}
    void display();
};
#endif