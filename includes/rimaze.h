#ifndef RIMAZE_H
#define RIMAZE_H

#include "utils.h"
#include "generatemaze.h"
#include "loadmaze.h"
#include "solvemaze.h"

void startRiMaze(char *fileName, unsigned short int speed, unsigned short int dev);
void displayMaze(volatile Maze *maze);

#endif