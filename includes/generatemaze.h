#ifndef GENERATEMAZE_H
#define GENERATEMAZE_H

#include "utils.h"
#include "init.h"
#include "loadmaze.h"

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC, unsigned short int enX, unsigned short int enY, unsigned short int exX, unsigned short int exY);
void generateMazeToFile(char *fileName, volatile Maze *maze, Entrance en, Exit ex);
unsigned short int probabilityAction();
void buildMaze(volatile Maze *maze, Entrance en, Exit ex);
void breakWall(volatile Maze *maze, unsigned short int nbWall, unsigned short int x, unsigned short int y);
void breakWallDirection(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dir);
void breakWallUpper(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallRight(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallBottom(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallLeft(volatile Maze *maze, unsigned short int x, unsigned short int y);

#endif