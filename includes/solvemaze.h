#ifndef SOLVEMAZE_H
#define SOLVEMAZE_H

#include "utils.h"
#include "generatemaze.h"

void solveMaze(volatile Maze *maze, unsigned short int speed, unsigned short int dev);
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
unsigned short int isSuperDeadEnd(volatile Maze *maze);
unsigned short int isHalfDeadEnd(volatile Maze *maze);
void hideSolver(volatile Maze *maze);
void displaySolver(volatile Maze *maze);
void displayControlPanel(volatile Maze *maze);
void displaySolverTrace(volatile Maze *maze);
unsigned short int checkSpeed(unsigned short int speed);
void markMainPathCell(volatile Maze *maze);
void unmarkMainPathCell(volatile Maze *maze);
unsigned short int isMainPathCellMarked(volatile Maze *maze, unsigned short int x, unsigned short int y);
unsigned short int isMainPathCellAlreadyMarked(volatile Maze *maze, unsigned short int x, unsigned short int y);
void displayMainPath(volatile Maze *maze);
unsigned short int isMountain(volatile Maze *maze, unsigned short int x, unsigned short int y);
void displayMarkers(volatile Maze *maze);

#endif