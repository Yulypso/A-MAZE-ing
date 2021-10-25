#ifndef GENERATEMAZE_H
#define GENERATEMAZE_H

#include "utils.h"

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC, unsigned short int enX, unsigned short int enY, unsigned short int exX, unsigned short int exY);
void generateMazeToFile(char *fileName, unsigned short int nbL, unsigned short int nbC, Entrance en, Exit ex);

#endif