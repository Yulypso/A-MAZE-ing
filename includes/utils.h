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

void loadMazeFromFile(volatile Maze *maze, char *fileName);
void initMaze(volatile Maze *maze, char *fileName);
void freeMaze(volatile Maze *maze);
void displayMazeFileConsole(volatile Maze *maze);
void displayMazeFile(volatile Maze *maze);

#endif