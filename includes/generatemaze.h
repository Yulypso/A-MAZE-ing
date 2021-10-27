#ifndef GENERATEMAZE_H
#define GENERATEMAZE_H

#include "utils.h"
#include "init.h"
#include "loadmaze.h"

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC);
void generateMazeToFile(char *fileName, volatile Maze *maze);
unsigned short int probabilityAction();
void initEntranceCoords(volatile Maze *maze);
void buildMaze(volatile Maze *maze);
void breakWall(volatile Maze *maze, unsigned short int nbWall, unsigned short int x, unsigned short int y);
void breakWallDirection(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dir);
void breakWallUpper(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallRight(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallBottom(volatile Maze *maze, unsigned short int x, unsigned short int y);
void breakWallLeft(volatile Maze *maze, unsigned short int x, unsigned short int y);
void displayEntrance(volatile Maze *maze);
void displayExit(volatile Maze *maze);

#endif