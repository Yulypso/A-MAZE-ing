#include "../includes/rimaze.h"

/* Maze's features */

volatile Maze *maze;

void startRiMaze(char *fileName)
{
    maze = (Maze *)malloc(sizeof(Maze));
    initMaze(maze, fileName);
    //displayMaze(maze);
    freeMaze(maze);
}

void displayMaze(volatile Maze *maze)
{
}