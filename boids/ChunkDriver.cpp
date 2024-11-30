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

Chunk::Chunk()
{
    _topx = _topy = _sizey = _sizex = 0;
    _boids = {};
    _haveBoids = false;
}
Chunk::Chunk(double topx, double topy, double sizex, double sizey)
{
    _topx  = topx;
    _topy  = topy;
    _sizex = sizex;
    _sizey = sizey;
    _boids = {};
    _haveBoids = false;
}

bool Chunk::addBoid(Boid* newBoid)
    {
    cout << _topx << ", " << _topy << "| "<<newBoid->getx() <<", "<<newBoid->gety()<<"|" << inMyBounds(newBoid->getx(),newBoid->gety()) ;
    cout << endl;
    if(inMyBounds(newBoid->getx(),newBoid->gety())){
        //cout << "added boid";
        _boids.push_back(newBoid);
        newBoid->setChunk(this);
        _haveBoids=true;
        return true;
        }
return false;
        }

void Chunk::refreshBoid(Boid* newBoid){
    if(!addBoid(newBoid)){
        for(int i = 0; i < _neighbors.size();i++){
            if(_neighbors[i]->addBoid(newBoid)){
                break;
            }
        }
    }
}
vector<BoidRef> Chunk::giveNearbyBoids(double x, double y)
{
    cout << "Giving neighbors" << endl;
    vector <BoidRef> nearbyBoids;
    checkBoids(x,y,nearbyBoids);
    cout << "checked self" << endl;
    for(int i = 0; i < _neighbors.size(); i++){
        _neighbors[i]->checkBoids(x,y,nearbyBoids);
    }
    return nearbyBoids;
}
vector<BoidRef> Chunk::checkBoids(double x, double y, vector<BoidRef> nearbyBoids)
{
    cout << "checking self" << endl;

    if(_haveBoids){    
        cout  << _boids.size()<<endl;
        for(int i = 0; i < _boids.size(); i++){
        cout << i << endl;
        Boid boid = *_boids.at(i);
        cout << "assigned boid" << boid.getx() << endl;
        cout << "dist: "<<boid.getDistance(x,y);
        bool distCheck = boid.checkDistance(x,y);
        if(distCheck){
            cout << "passed dist check" << endl;
            nearbyBoids.push_back(boid.getRef(x,y));
        }
    }
    }
    return nearbyBoids;
}