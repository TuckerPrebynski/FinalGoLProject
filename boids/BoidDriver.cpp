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


char Boid::getDisplay()
{
    //\ ^ / 
    //<   >
    /// v 
    if(abs(_heading[0]) > abs(_heading[1])){
        if(_heading[0] > 0){
            return '>';
        } else{
            return '<';
        }
    }else{
        if(_heading[1] > 0){
            return 'v';
        } else{
            return '^';
        }
    }
    return '#';
}

vector<double> Boid::headingToVector(double h[2], double w)
{
    double hs = sqrt(h[0]*h[0] + h[1]*h[1]);
    return {(h[0]/hs)*w,(h[1]/hs)*w};
}
void Boid::setMax(int maxX, int maxY)
{
    MAXX = maxX;
    MAXY = maxY;
}
BoidRef Boid::getRef(double x, double y)
{
    BoidRef newRef;
    newRef.heading[0] = _heading[0];
    newRef.heading[1] = _heading[1];
    newRef.position[0] = _x;
    newRef.position[1] = _y;
    newRef.relationVector[0] = _x - x;
    newRef.relationVector[1] = _y - y;
    newRef.dist = getDistance(x,y);
    return newRef;
}
void Boid::findNeighbors(){
    Chunk currChunk = *_chunk;
    //cout << "before"<<_neighbors.size() << endl;
    _neighbors = currChunk.giveNearbyBoids(_x,_y);
   // cout <<"after" <<_neighbors.size() << endl;
}
void Boid::avgNeighbors()
{
    if(_neighbors.size() > 0){
        double heading[2] = {0,0};
        double avgVx = 0;
        double avgVy = 0;
        double avgx = 0;
        double avgy = 0;
        double avgDist = 0;
        int count = _neighbors.size();
        for(int i = 0; i < count;i++){
            BoidRef currRef = _neighbors[i];
            //cout << _heading[0]*currRef.relationVector[0] + _heading[1]*currRef.relationVector[1] << endl;
            if((_heading[0]*currRef.relationVector[0] + _heading[1]*currRef.relationVector[1]) > -.5){
                heading[0] += currRef.heading[0];
                heading[1] += currRef.heading[1];
                avgVx += currRef.relationVector[0];
                avgVy += currRef.relationVector[1];
                avgx  += currRef.position[0];
                avgy  += currRef.position[1];
                avgDist += currRef.dist;
            }else{
                _neighbors.erase(_neighbors.begin() + i);
            }
        }
        _avgNeighbor.heading[0] = heading[0]/count;
        _avgNeighbor.heading[1] = heading[1]/count;
        _avgNeighbor.position[0] = avgx/count;
        _avgNeighbor.position[1] = avgy/count;
        _avgNeighbor.relationVector[0] = avgVx/count;
        _avgNeighbor.relationVector[1] = avgVy/count;
        _avgNeighbor.dist = avgDist/count;
    }else{
        _avgNeighbor.heading[0] = _heading[0];
        _avgNeighbor.heading[1] = _heading[1];
        _avgNeighbor.position[0] = _x;
        _avgNeighbor.position[1] = _y;
        _avgNeighbor.relationVector[0] = _heading[0];
        _avgNeighbor.relationVector[1] = _heading[1];
        _avgNeighbor.dist = 10;
    }
}

WeightedHeading Boid::separation()
{
    WeightedHeading heading;
    
    heading.heading[0] = _avgNeighbor.relationVector[0] * -1;
    heading.heading[1] = _avgNeighbor.relationVector[1] * -1;
    heading.weight = distThresh/_avgNeighbor.dist;
    //cout << "Separation heading:" << heading.heading << " Weight: " << heading.weight << "|" << _neighbors.size()<<endl;
    return heading;
}
WeightedHeading Boid::alignment()
{
    WeightedHeading heading;
    heading.heading[0] = _avgNeighbor.heading[0];
    heading.heading[1] = _avgNeighbor.heading[1];
    heading.weight = 5*(_heading[0]*heading.heading[1] - _heading[1]*heading.heading[0]);
    //cout << "Alignment heading:" << heading.heading << " Weight: " << heading.weight << "|" << _neighbors.size()<<endl;
    return heading;
}
WeightedHeading Boid::cohesion()
{
    WeightedHeading heading;
    heading.heading[0] = _avgNeighbor.position[0]-_x;
    heading.heading[1] = _avgNeighbor.position[1]-_y;
    heading.weight = 1.0/50.0;
    //cout << "Cohesion heading:" << heading.heading << " Weight: " << heading.weight << "|" << _neighbors.size()<<endl;
    return heading;
}
void Boid::calcVectors()
{   findNeighbors();
    if(_neighbors.size() > 1){
        double avgHead[2] = {0,0};
        double avgPos[2] = {0,0};
        double sepVec[2] = {0,0};
        double aliVec[2] = {0,0};
        double cohVec[2] = {0,0};
        int count = _neighbors.size();
        for(int i = 0; i < count;i++){
            BoidRef currRef = _neighbors[i];
            //if((_heading[0]*currRef.relationVector[0] + _heading[1]*currRef.relationVector[1]) > -.5){
                
                avgPos[0] += _neighbors[i].position[0];
                avgPos[1] += _neighbors[i].position[1];
            
                if(sqrt(((_neighbors[i].position[0]-_x)*(_neighbors[i].position[0]-_x) + (_neighbors[i].position[1]-_y)*(_neighbors[i].position[1]-_y))) < 2){
                    sepVec[0] -= _neighbors[i].position[0] - _x;
                    sepVec[1] -= _neighbors[i].position[1] - _y;
                }

                avgHead[0] += _neighbors[i].heading[0];
                avgHead[1] += _neighbors[i].heading[1];

            //}
        }
        //avgPos[0] =- _x;
        //avgPos[1] =- _y;
        avgPos[0] /= count;
        avgPos[1] /= count;
        avgHead[0] /= count;
        avgHead[1] /= count;
        //cout << "avghead: " << avgHead[0] << ", " << avgHead[1] << '|';
        cout << "avgpos: " << avgPos[0] << ", " << avgPos[1] << ',' << count <<'|';
        cohVec[0] = (avgPos[0] - _x)/50;
        cohVec[1] = (avgPos[1] - _y)/50;
        aliVec[0] = (avgHead[0] - _heading[0])/8;
        aliVec[1] = (avgHead[1] - _heading[1])/8;
        cout << "Coh:" << cohVec[0] << ',' << cohVec[1];
        cout << "|Ali:" << aliVec[0] << ',' << aliVec[1];
        //cout << "|Sep:" << sepVec[0] << ',' << sepVec[1];
        cout << endl;
        _newHeading[0] = cohVec[0] + aliVec[0] + sepVec[0];
        _newHeading[1] = cohVec[1] + aliVec[1] + sepVec[1];
        vector <double> deltaPos = headingToVector(_newHeading,_speed);
        _newx = deltaPos[0];
        _newy = deltaPos[1];
    }
}
WeightedHeading Boid::screenX()
{
    WeightedHeading heading;
    
    heading.heading[0] = ((_x > (MAXX/2)) ? -1 : 1)*edgeAvoidance;
    heading.heading[1] = 0;
    heading.weight = (1.0/(_x) + 1.0/(MAXX-_x));
    return heading;
}
WeightedHeading Boid::screenY()
{
    WeightedHeading heading;
    
    heading.heading[0] = ((_y > (MAXY/2)) ? -1 : 1)*edgeAvoidance;
    heading.heading[1] = 0;
    heading.weight = (1.0/(_y) + 1.0/(MAXY-_y));
    return heading;
}

void Boid::calculateNewPos()
{
    findNeighbors();
    // cout << "found Neighbors" << endl;
    avgNeighbors();
    // cout << "Avg neighbors" <<endl;
    WeightedHeading sep, ali, coh, scx, scy;
    sep = separation();
    // cout << "sep ";
    ali = alignment();    
    //cout << "ali ";
    coh = cohesion();
    //cout << "coh ";
    scx = screenX();
    scy = screenY();
    //cout << "scr";

    vector <double> vSep;

    vector <double> vAli;
    vector <double> vCoh;
    vector <double> vScx;
    vector <double> vScy;
    vector <double> deltaPos;
    double weightSum = sep.weight + ali.weight + coh.weight + scx.weight + scy.weight;
    vSep = headingToVector(sep.heading,sep.weight/weightSum);
    vAli = headingToVector(ali.heading,ali.weight/weightSum);
    vCoh = headingToVector(coh.heading,coh.weight/weightSum);
    vScx = headingToVector(scx.heading,scx.weight/weightSum);
    vScy = headingToVector(scy.heading,scy.weight/weightSum);
    double xSum = vSep[0] + vAli[0] + vCoh[0] + vScx[0] + vScy[0];
    double ySum = vSep[1] + vAli[1] + vCoh[1] + vScx[1] + vScy[1];
    //xSum /= weightSum;
    //ySum /= weightSum;
    if(_neighbors.size() > 1){
        _newHeading[0] = xSum;
        _newHeading[1] = ySum;
        }else{
        _newHeading[0] = _heading[0];
        _newHeading[1] = _heading[1];
        }
    deltaPos = headingToVector(_newHeading,_speed);
    _newx = deltaPos[0];
    _newy = deltaPos[1];
    //cout << "done" << endl;
}
void Boid::update(){
    _x += _newx;
    _y += _newy;
    _heading[0] += _newHeading[0];
    _heading[1] += _newHeading[1];

    vector <double> vecHead = headingToVector(_heading,1);
    _heading[0] = vecHead[0];
    _heading[1] = vecHead[1];

    _chunk->refreshBoid(this);
}
void Boid::displayDebug(){
    //cout << _x << ", " << _y << "| " << _heading[0]<<", " <<_heading[1] <<"| " << _newHeading[0]<<", " << _newHeading[1]<<"|"<< _neighbors.size()<<endl;
    
}
void Boid::display(){
    //cout << _x << ", " << _y << "| " << _heading[0]<<", " <<_heading[1] <<"| " << _newHeading[0]<<", " << _newHeading[1]<<"|"<< _neighbors.size()<<endl;
    string output = " "; output[0] = getDisplay();
    mvaddstr(int(_y),int(_x),output.c_str());
}