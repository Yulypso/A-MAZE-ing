#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

typedef struct Entrance
{
    unsigned short int x;
    unsigned short int y;
} Entrance;

typedef struct Exit
{
    unsigned short int x;
    unsigned short int y;
} Exit;

typedef struct Solver
{
    unsigned short int x;
    unsigned short int y;
} Solver;

typedef struct Maze
{
    unsigned short int **board;
    unsigned short int nbL;
    unsigned short int nbC;
    Entrance entrance;
    Exit exit;
    Solver solver;
} Maze;

void loadMazeFromFile(char *fileName);
void initMaze(char *fileName);
void freeMaze(volatile Maze *maze);
void displayMazeFileConsole(volatile Maze *maze);
void displayMazeFile(volatile Maze *maze);
void displayCellBits(volatile Maze *maze, unsigned short int x, unsigned short int y);
void createCellAtPosition(volatile Maze *maze, unsigned short int x, unsigned short int y);
void displayCellAtPosition(volatile Maze *maze, unsigned short int x, unsigned short int y);
void displaySolverAtPosition(unsigned short int x, unsigned short int y);
void displayDecorativeWall(volatile Maze *maze, unsigned short int x, unsigned short int y);
unsigned short int checkWall(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int pos);
unsigned short int checkBreakLeft(volatile Maze *maze, unsigned short int x, unsigned short int y);
unsigned short int checkBreakUpper(volatile Maze *maze, unsigned short int x, unsigned short int y);
unsigned short int checkBreakBottom(volatile Maze *maze, unsigned short int x, unsigned short int y);

#endif