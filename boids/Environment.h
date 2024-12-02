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
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
const int chunkFactor = 3;
const int scale = 1;
const int screenY = (COLS > 0) ? COLS : 30;
const int screenX = (LINES > 0) ? LINES : 30;
const int numBoids = 30;
Chunk map[chunkFactor][chunkFactor];
#endif