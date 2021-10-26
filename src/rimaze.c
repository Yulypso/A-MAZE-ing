#include "../includes/rimaze.h"

/* Maze's features */

volatile Maze *maze;
volatile Maze *mazeTmp;
volatile unsigned short int NB_BITS_CELL;
volatile unsigned short int maxX, maxY;
volatile unsigned short int startX, startY;

void startRiMaze(char *fileName)
{
    NB_BITS_CELL = sizeof(volatile unsigned short int) * 8;
    maze = (Maze *)malloc(sizeof(Maze));
    mazeTmp = (Maze *)malloc(sizeof(Maze));
    initMaze(fileName);
    displayMaze(maze);

    //displaySolverAtPosition(maze->exit.x, maze->exit.y);
    freeMaze(mazeTmp);
    freeMaze(maze);
}

void displayMaze(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < mazeTmp->nbL; ++i)
        for (unsigned short int j = 0; j < mazeTmp->nbC; ++j)
        {
            displayDecorativeWall(maze, i, j);
            createCellAtPosition(mazeTmp, i, j);
            displaySolverAtPosition(i, j);
        }

    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short int j = 0; j < maze->nbC; ++j)
            displayCellAtPosition(maze, i, j);

    mvaddch(startX - 1, startY - 1, ACS_ULCORNER);
    mvaddch(startX - 1, startY + 2 * maze->nbC - 1, ACS_URCORNER);
    mvaddch(startX + 2 * maze->nbL - 1, startY - 1, ACS_LLCORNER);
    mvaddch(startX + 2 * maze->nbL - 1, startY + 2 * maze->nbC - 1, ACS_LRCORNER);

    attron(COLOR_PAIR(3));
    mvaddch(startX + 2 * maze->entrance.x, startY + 2 * maze->entrance.y, 'E');
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(1));
    mvaddch(startX + 2 * maze->exit.x, startY + 2 * maze->exit.y, 'S');
    attroff(COLOR_PAIR(1));
}