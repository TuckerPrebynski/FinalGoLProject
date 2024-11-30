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
    double heading;
    double relationVector[2];
    double position[2];
    double dist;
};
struct WeightedHeading{
    double heading;
    double weight;
};
class Chunk;
class Boid{
    private:
    double _x;
    double _y;
    double _heading;
    double _speed = 2;

    const int distThresh = 10;
    Chunk *_chunk; 
    double _newx;
    double _newy;
    double _newHeading;
    vector <BoidRef> _neighbors;
    BoidRef _avgNeighbor;
    double compareHeadings(double h1, double h2);
    vector <double> headingToVector(double h, double w);
    
    public:
    double getx(){return _x;}
    double gety(){return _y;}
    BoidRef getRef(double x, double y);
    void setChunk(Chunk *chunk){_chunk = chunk;}
    void findNeighbors();
    double getDistance(double x, double y){return sqrt((_x-x)*(_x-x) + (_y-y)*(_y-y));}
    bool checkDistance(double x, double y){
        
        double dist = getDistance(x,y);
        return (  dist < distThresh);}
    void avgNeighbors();
    WeightedHeading separation();
    WeightedHeading alignment();
    WeightedHeading cohesion();
    void calculateNewPos();
    void update();
    void setPos(int x, int y){_x = x; _y = y;}
    void display();
};
#endif