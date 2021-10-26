#ifndef RIMAZE_H
#define RIMAZE_H

#include "utils.h"
#include "loadmaze.h"
#include "solvemaze.h"

void startRiMaze(char *fileName);
void displayMaze(volatile Maze *maze);

#endif