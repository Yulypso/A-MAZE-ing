#include "../includes/generatemaze.h"

/* Generate maze and save it into file */

volatile Maze *genMaze;

void breakWallUpper(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
}
void breakWallRight(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
}
void breakWallBottom(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
}
void breakWallLeft(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
}

void breakWallDirection(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dir)
{
    switch (dir)
    {
    case 3:
        breakWallUpper(maze, x, y);
        break;
    case 2:
        breakWallRight(maze, x, y);
        break;
    case 1:
        breakWallBottom(maze, x, y);
        break;
    case 0:
        breakWallLeft(maze, x, y);
        break;
    }
}

unsigned short int probabilityBreakWallDirection(unsigned short int len, unsigned short int *brokeWall)
{
    unsigned short int dir;
    unsigned short int index = 0;

    for (unsigned short int i = 0; i < len; ++i) // initialise values
    {
        *(brokeWall + i) = 4;
    }

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
    if (nbWall == 4)
    {
        breakWallUpper(maze, x, y);
        breakWallRight(maze, x, y);
        breakWallBottom(maze, x, y);
        breakWallLeft(maze, x, y);
    }
    else
    {
        unsigned short int *brokeWall = (unsigned short int *)malloc(nbWall * sizeof(unsigned short int));
        probabilityBreakWallDirection(nbWall, brokeWall);

        for (unsigned short int i = 0; i < nbWall; ++i)
        {
            mvprintw(3, i + 5, "%hd ", *(brokeWall + i));
            //breakWallDirection(maze, x, y, *(brokeWall +i));
        }
    }
}

void buildMaze(volatile Maze *maze, Entrance en, Exit ex)
{
    breakWall(maze, 3, 5, 5);
    /*for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short int j = 0; j < maze->nbC; ++j)
        {
            switch (probabilityAction)
            {
            case 0 ... 9: // Break 0 wall: 10%
                break;
            case 10 ... 24: // Break 1 wall: 15%
                breakWall(maze, 1, i, j);
                break;
            case 25 ... 64: // Break 2 walls: 40%
                breakWall(maze, 2, i, j);
                break;
            case 65 ... 84: // Break 3 walls: 20%
                breakWall(maze, 3, i, j);
                break;
            case 85 ... 99: // Break 4 walls: 15%
                breakWall(maze, 4, i, j);
                break;
            default:
                break;
            }
        }*/
}

unsigned short int probabilityAction()
{
    return rand() % 100; // % value between (0-99)
}

void generateMazeToFile(char *fileName, volatile Maze *maze, unsigned short int nbL, unsigned short int nbC, Entrance en, Exit ex)
{
    FILE *file = fopen(fileName, "w");

    if (file == NULL)
    {
        fprintf(stderr, "[Error]: Can't open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%hd %hd %hd %hd %hd %hd\n", nbL, nbC, en.x, en.y, ex.x, ex.y);

    buildMaze(maze, en, ex);
    //fprintf(file, "%hd", probabilityAction());

    fclose(file);
}

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC, unsigned short int enX, unsigned short int enY, unsigned short int exX, unsigned short int exY)
{
    initRand();
    Entrance en = {.x = enX, .y = enY};
    Exit ex = {.x = exX, .y = exY};
    genMaze = (Maze *)malloc(sizeof(Maze));
    initBoard(genMaze);

    generateMazeToFile(fileName, genMaze, 10, 10, en, ex);
    freeMaze(genMaze);
}