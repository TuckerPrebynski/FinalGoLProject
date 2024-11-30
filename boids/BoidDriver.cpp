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
double Boid::compareHeadings(double h1, double h2)
{
    int dist1;
    int dist2;
    if(h1>h2){
        dist1 = (360-h1) + (h2-0);
    }else{
        dist1 = (360-h2) + (h1-0);
    }
        dist2 = abs(h1-h2);
    return (dist1 < dist2) ? dist1 : dist2;
}
vector<double> Boid::headingToVector(double h, double w)
{
    double radH = h * M_PI / 180;
    return {cos(radH)*w,sin(radH)*w};
}
BoidRef Boid::getRef(double x, double y)
{
    BoidRef newRef;
    newRef.heading = _heading;
    newRef.position[0] = _x;
    newRef.position[1] = _y;
    newRef.relationVector[0] = _x - x;
    newRef.relationVector[1] = _y - y;
    newRef.dist = getDistance(x,y);
    return newRef;
}
void Boid::findNeighbors(){
    cout << "finding neighbors" <<endl;
    _chunk->dummmyRef();
    _neighbors = _chunk->giveNearbyBoids(_x,_y);
}
void Boid::avgNeighbors()
{
    double heading = 0;
    double avgVx = 0;
    double avgVy = 0;
    double avgx = 0;
    double avgy = 0;
    double avgDist = 0;
    int count = _neighbors.size();
    for(int i = 0; i < count;i++){
        BoidRef currRef = _neighbors[i];
        heading += currRef.heading;
        avgVx += currRef.relationVector[0];
        avgVy += currRef.relationVector[1];
        avgx  += currRef.position[0];
        avgy  += currRef.position[1];
        avgDist += currRef.dist;
    }
    _avgNeighbor.heading = heading/count;
    _avgNeighbor.position[0] = avgx/count;
    _avgNeighbor.position[1] = avgy/count;
    _avgNeighbor.relationVector[0] = avgVx/count;
    _avgNeighbor.relationVector[1] = avgVy/count;
    _avgNeighbor.dist = avgDist/count;
}
WeightedHeading Boid::separation()
{
    WeightedHeading heading;
    heading.heading = (atan2(_avgNeighbor.relationVector[1]*-1,_avgNeighbor.relationVector[0]*-1) * 180 / M_PI);
    heading.weight = 1 - _avgNeighbor.dist/distThresh;
    return heading;
}
WeightedHeading Boid::alignment()
{
    WeightedHeading heading;
    heading.heading = _avgNeighbor.heading;
    heading.weight = compareHeadings(_heading,_avgNeighbor.heading)/180;
    return heading;
}
WeightedHeading Boid::cohesion()
{
    WeightedHeading heading;
    heading.heading = atan2(_avgNeighbor.position[1]-_y,_avgNeighbor.position[0]-_x) * 180 / M_PI;
    heading.weight = _avgNeighbor.dist/distThresh;
    return heading;
}

void Boid::calculateNewPos()
{
    findNeighbors();
    cout << "Found Neighbors" <<endl;
    avgNeighbors();
    cout << "avg Neighbors" << endl;
    WeightedHeading sep, ali, coh;
    sep = separation();
    cout <<"Sep done" <<endl;
    ali = alignment();    
    cout <<"Ali done" <<endl;
    coh = cohesion();
    cout << "Coh done"<<endl;
    vector <double> vSep;
    vector <double> vAli;
    vector <double> vCoh;
    vector <double> deltaPos;
    vSep = headingToVector(sep.heading,sep.weight);
    vAli = headingToVector(ali.heading,ali.weight);
    vCoh = headingToVector(coh.heading,coh.weight);
    double weightSum = sep.weight + ali.weight + coh.weight;
    double xSum = vSep[0] + vAli[0] + vCoh[0];
    double ySum = vSep[1] + vAli[1] + vCoh[1];
    xSum /= weightSum;
    ySum /= weightSum;

    _newHeading = atan2(ySum,xSum)*180/M_PI;
    deltaPos = headingToVector(_newHeading,_speed);
    _newx = deltaPos[0];
    _newy = deltaPos[1];
}
void Boid::update(){
    _x += _newx;
    _y += _newy;
    _heading =_newHeading;
    _chunk->refreshBoid(this);
}
void Boid::display(){
    cout << _x << ", " << _y << ", " << _heading <<endl;
}