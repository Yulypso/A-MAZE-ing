#include "../includes/generatemaze.h"

/* Generate maze and save it into file */

volatile Maze *genMaze;

void breakWallUpper(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    *(*(maze->board + x) + y) &= 7;
    *(*(maze->board + x - 1) + y) &= 13;
}
void breakWallRight(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    *(*(maze->board + x) + y) &= 11;
    *(*(maze->board + x) + y + 1) &= 14;
}
void breakWallBottom(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    *(*(maze->board + x) + y) &= 13;
    *(*(maze->board + x + 1) + y) &= 7;
}
void breakWallLeft(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    *(*(maze->board + x) + y) &= 14;
    *(*(maze->board + x) + y - 1) &= 11;
}

void breakWallDirection(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dir)
{
    switch (dir)
    {
    case 3:
        if (x != 0)
            breakWallUpper(maze, x, y);
        break;
    case 2:
        if (y != maze->nbC - 1)
            breakWallRight(maze, x, y);
        break;
    case 1:
        if (x != maze->nbL - 1)
            breakWallBottom(maze, x, y);
        break;
    case 0:
        if (y != 0)
            breakWallLeft(maze, x, y);
        break;
    default:
        break;
    }
}

unsigned short int probabilityBreakWallDirection(unsigned short int len, unsigned short int *brokeWall)
{
    unsigned short int dir;
    unsigned short int index = 0;

    for (unsigned short int i = 0; i < len; ++i) // initialise values
        *(brokeWall + i) = 4;

    while (true)
    {
        if (index == len)
            break;

        dir = rand() % 4; // Get a direction
        for (unsigned short int i = 0; i < len; ++i)
        {
            if (dir == brokeWall[i]) // Check if the direction is already taken
                break;
            if (i == len - 1)
            {
                *(brokeWall + index) = dir;
                ++index;
            }
        }
    }
    return dir;
}

void breakWall(volatile Maze *maze, unsigned short int nbWall, unsigned short int x, unsigned short int y)
{
    unsigned short int *brokeWall = (unsigned short int *)malloc(nbWall * sizeof(unsigned short int));
    probabilityBreakWallDirection(nbWall, brokeWall);

    for (unsigned short int i = 0; i < nbWall; ++i)
        breakWallDirection(maze, x, y, *(brokeWall + i));
}

void buildMaze(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            switch (probabilityAction())
            {
            case 0 ... 9: // Break 0 wall: 10%
                break;
            case 10 ... 54: // Break 1 wall: 45%
                breakWall(maze, 1, i, j);
                break;
            case 55 ... 74: // Break 2 walls: 20%
                breakWall(maze, 2, i, j);
                break;
            case 75 ... 84: // Break 3 walls: 10%
                breakWall(maze, 3, i, j);
                break;
            case 85 ... 99: // Break 4 walls: 15%
                breakWall(maze, 4, i, j);
                break;
            default:
                break;
            }
        }
}

unsigned short int probabilityAction()
{
    return rand() % 100; // % value between (0-99)
}

void initEntranceCoords(volatile Maze *maze)
{
    maze->entrance.x = 0;
    maze->entrance.y = rand() % maze->nbC;
    maze->exit.x = maze->nbL - 1;
    maze->exit.y = rand() % maze->nbC;
}

void generateMazeToFile(char *fileName, volatile Maze *maze)
{
    FILE *file = fopen(fileName, "w");

    if (file == NULL)
    {
        fprintf(stderr, "[Error]: Can't open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%hd %hd %hd %hd %hd %hd\n", maze->nbL, maze->nbC, maze->entrance.x, maze->entrance.y, maze->exit.x, maze->exit.y);

    buildMaze(maze);

    for (unsigned short int i = 0; i < maze->nbL; ++i)
    {
        for (unsigned short int j = 0; j < maze->nbC; ++j)
            fprintf(file, "%hd ", *(*(maze->board + i) + j));
        fprintf(file, "\n");
    }

    fclose(file);
}

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC)
{
    initRand();

    genMaze = (Maze *)malloc(sizeof(Maze));
    genMaze->nbL = nbL;
    genMaze->nbC = nbC;

    initEntranceCoords(genMaze);
    initBoard(genMaze);
    generateMazeToFile(fileName, genMaze);

    freeMaze(genMaze);
}