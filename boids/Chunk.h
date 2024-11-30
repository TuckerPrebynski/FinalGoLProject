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
#ifndef CHUNK_H
#define CHUNK_H
#include "Boid.h"
class Chunk{
    private:
    double _topx;
    double _topy;
    double _sizex;
    double _sizey;
    vector <Chunk*> _neighbors;

    vector <Boid*> _boids;
    bool _haveBoids = false;
    /// @brief check if given point is within chunk
    /// @param x 
    /// @param y 
    /// @return true if point in chunk, false otherwise
    bool inMyBounds(double x,double y){return (((x-_topx) >= 0)&&((x-_topx) <= _sizex))&& (((y-_topy) >= 0)&&((y-_topy) <= _sizey));}
    public:
    Chunk();
    Chunk(double topx, double topy, double sizex,double sizey);
    void addNeighbor(Chunk* newNeighbor){_neighbors.push_back(newNeighbor);}
    /// @brief check if given boid is within chunck, if so add it to list of boids in chunk
    /// @param newBoid 
    /// @return true if boid succesfully added, false if out of bounds
    bool addBoid(Boid* newBoid);
    /// @brief try to place the given boid into either itself, or one of its neighbors
    /// @param newBoid pointer to the boid object
    void refreshBoid(Boid* newBoid);
    void resetBoids(){_boids = {}; _haveBoids=false;}

    /// @brief iterate through self and neighbors, return list of all boids near to given point
    /// @param x 
    /// @param y 
    /// @return vector of BoidRef structs, one for each boid within threshold
    vector <BoidRef> giveNearbyBoids(double x, double y);
    vector <BoidRef> checkBoids(double x, double y, vector<BoidRef> nearbyBoids);
    void dummmyRef(){
        cout << "you got the dud" << endl;
    }
};
#endif