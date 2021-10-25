#include "../includes/rimaze.h"

/* Maze's features */

volatile Maze *maze;
volatile unsigned short int NB_BITS_CELL;
volatile unsigned short int maxX, maxY;
volatile unsigned short int startX, startY;

void startRiMaze(char *fileName)
{

    NB_BITS_CELL = sizeof(volatile unsigned short int) * 8;
    maze = (Maze *)malloc(sizeof(Maze));
    initMaze(maze, fileName);
    displayMaze(maze);
    freeMaze(maze);
}

void displayMaze(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            displayCellAtPosition(maze, i, j);
        }
    }
}