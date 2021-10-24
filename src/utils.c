#include "../includes/utils.h"

extern volatile Maze maze;

/* Utils to load, read and writes files */

void loadMazeFromFile(volatile Maze *maze, char *fileName)
{
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        fprintf(stderr, "[Error]: Can't open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%hd %hd %hd %hd %hd %hd", &maze->nbL, &maze->nbC, &maze->entrance.x, &maze->entrance.y, &maze->exit.x, &maze->exit.y);

    maze->solver.x = maze->entrance.x;
    maze->solver.y = maze->entrance.y;

    maze->board = (unsigned short int **)malloc(maze->nbL * sizeof(unsigned short int *));
    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        *(maze->board + i) = (unsigned short int *)malloc(maze->nbC * sizeof(unsigned short int));
    }

    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            fscanf(file, "%hd", *(maze->board + i) + j);
        }
    }
}

void initMaze(volatile Maze *maze, char *fileName)
{
    loadMazeFromFile(maze, fileName);
}

void freeMaze(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        free(*(maze->board + i));
    }
    free(maze->board);
    maze->board = NULL;

    free((void *)maze);
    maze = NULL;
}

/* For console purpose only */
void displayMazeFileConsole(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            fprintf(stdout, "%hd ", *(*(maze->board + i) + j));
        }
        fprintf(stdout, "\n");
    }
}

void displayMazeFile(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            printw("%hd ", *(*(maze->board + i) + j));
        }
        printw("\n");
    }
}