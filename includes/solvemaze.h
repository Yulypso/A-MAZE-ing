#ifndef SOLVEMAZE_H
#define SOLVEMAZE_H

#include "utils.h"

void solveMaze(volatile Maze *maze);
void solverGoUpper(volatile Maze *maze);
void solverGoRight(volatile Maze *maze);
void solverGoBottom(volatile Maze *maze);
void solverGoLeft(volatile Maze *maze);
void markCell(volatile Maze *maze, unsigned short int nbMarks);
unsigned short int getNbCellMarks(volatile Maze *maze, unsigned short int x, unsigned short int y);
unsigned short int checkUpperWall(volatile Maze *maze);
unsigned short int checkRightWall(volatile Maze *maze);
unsigned short int checkBottomWall(volatile Maze *maze);
unsigned short int checkLeftWall(volatile Maze *maze);
unsigned short int getMinTabIndex(unsigned short int *tab);
unsigned short int determineBestPath(volatile Maze *maze);
unsigned short int isDeadEnd(volatile Maze *maze);

#endif