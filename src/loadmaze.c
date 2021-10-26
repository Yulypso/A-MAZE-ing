#include "../includes/loadmaze.h"

/* Load maze from file */

extern volatile Maze *maze;
extern volatile Maze *mazeTmp;
extern volatile unsigned short int NB_BITS_CELL;
extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

void loadMazeFromFile(char *fileName)
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
        *(maze->board + i) = (unsigned short int *)malloc(maze->nbC * sizeof(unsigned short int));

    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short int j = 0; j < maze->nbC; ++j)
            fscanf(file, "%hd", *(maze->board + i) + j);

    mazeTmp->nbL = maze->nbL;
    mazeTmp->nbC = maze->nbC;
    initBoard(mazeTmp);

    fclose(file);
}

void initBoard(volatile Maze *maze)
{
    maze->board = (unsigned short int **)malloc(maze->nbL * sizeof(unsigned short int *));
    for (unsigned short int i = 0; i < maze->nbL; ++i)
        *(maze->board + i) = (unsigned short int *)malloc(maze->nbC * sizeof(unsigned short int));

    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short int j = 0; j < maze->nbC; ++j)
            *(*(maze->board + i) + j) = 15;
}

void initMaze(char *fileName)
{
    loadMazeFromFile(fileName);
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

/* create wall bit positions b3 b2 b1 b0 at solver position (x,y)*/
void createCellAtPosition(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 13; i >= 0; --i)
    {
        if ((*(*(maze->board + x) + y) >> i) & 1)
        {
            switch (i)
            {
            case 0:
                mvaddch(startX + 2 * x, startY + 2 * y - 1, ACS_VLINE);
                break;

            case 1:
                mvaddch(startX + 2 * x + 1, startY + 2 * y, ACS_HLINE);
                break;

            case 2:
                mvaddch(startX + 2 * x, startY + 2 * y + 1, ACS_VLINE);
                break;

            case 3:
                mvaddch(startX + 2 * x - 1, startY + 2 * y, ACS_HLINE);
                break;

            default:
                break;
            }
        }
    }
}

unsigned short int checkBreakLeft(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    if (checkWall(maze, x, y - 1, 2)) // Error if right wall exists
        return 0;
    else
    {
        if (x != 0 && y != 0) // not in borders
        {
            /* Left Upper part */
            if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x, y - 1, 3)) // l
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x, y - 1, 0)) // u
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x - 1, y, 1) && checkWall(maze, x, y, 3)) // r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0) && checkWall(maze, x - 1, y, 1)) // u r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LLCORNER);
            if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y, 1) && checkWall(maze, x, y, 3) && checkWall(maze, x, y - 1, 3)) // l r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_HLINE);
            if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0)) // l u
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LRCORNER);
            if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0) && checkWall(maze, x - 1, y, 1)) // l u r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_BTEE);
        }
    }
    return 1;
}

unsigned short int checkBreakRight(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    if (checkWall(maze, x, y + 1, 0)) // Error if left wall exists
        return 0;
    else
    {
        if (x != maze->nbL - 1 && y != maze->nbC - 1)
        {
            /* Right Bottom part */
            if (checkWall(maze, x, y, 1) && checkWall(maze, x + 1, y, 3)) // l
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x + 1, y, 2) && checkWall(maze, x + 1, y + 1, 0)) // b
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x, y + 1, 1) && checkWall(maze, x + 1, y + 1, 3)) // r
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x + 1, y, 3) && checkWall(maze, x + 1, y, 2) && checkWall(maze, x + 1, y + 1, 0)) // l b
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_URCORNER);
            if (checkWall(maze, x + 1, y, 2) && checkWall(maze, x + 1, y + 1, 0) && checkWall(maze, x + 1, y + 1, 3)) // b r
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_ULCORNER);
            if (checkWall(maze, x, y, 1) && checkWall(maze, x + 1, y, 3) && checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x, y + 1, 1)) // l r
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_HLINE);
            if (checkWall(maze, x + 1, y, 3) && checkWall(maze, x + 1, y, 2) && checkWall(maze, x + 1, y + 1, 0) && checkWall(maze, x + 1, y + 1, 3)) // l b r
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_TTEE);
        }
    }
    return 1;
}

unsigned short int checkBreakUpper(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    if (checkWall(maze, x - 1, y, 1)) // Error if Bottom wall exists
        return 0;
    else
    {
        if (x != 0 && y != maze->nbC - 1)
        {
            /* Upper Right part */
            if (checkWall(maze, x - 1, y, 2) && checkWall(maze, x - 1, y + 1, 0)) // u
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x, y + 1, 3)) // r
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x, y, 2) && checkWall(maze, x, y + 1, 0)) // b
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ' ');
            if (checkWall(maze, x - 1, y, 2) && checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x, y + 1, 3)) // u r
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_LLCORNER);
            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x, y + 1, 3) && checkWall(maze, x, y + 1, 0)) // r b
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_ULCORNER);
            if (checkWall(maze, x - 1, y, 2) && checkWall(maze, x - 1, y + 1, 0) && checkWall(maze, x, y, 2) && checkWall(maze, x, y + 1, 0)) // u b
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_VLINE);
            if (checkWall(maze, x - 1, y, 2) && checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x, y + 1, 3) && checkWall(maze, x, y + 1, 0)) // u r b
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_LTEE);
        }
    }
    return 1;
}

unsigned short int checkBreakBottom(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    if (checkWall(maze, x + 1, y, 3)) // Error if Upper wall exists
        return 0;
    else
    {
        if (x != maze->nbL - 1 && y != 0)
        {
            /* Bottom Left part */
            if (checkWall(maze, x + 1, y, 0) && checkWall(maze, x + 1, y - 1, 2)) // b
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x, y - 1, 1) && checkWall(maze, x + 1, y - 1, 3)) // l
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x, y, 0) && checkWall(maze, x, y - 1, 2)) // u
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ' ');
            if (checkWall(maze, x + 1, y, 0) && checkWall(maze, x + 1, y - 1, 2) && checkWall(maze, x, y - 1, 1) && checkWall(maze, x + 1, y - 1, 3)) // b l
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_URCORNER);
            if (checkWall(maze, x, y - 1, 1) && checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x, y, 0) && checkWall(maze, x, y - 1, 2)) // l u
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LRCORNER);
            if (checkWall(maze, x + 1, y, 0) && checkWall(maze, x + 1, y - 1, 2) && checkWall(maze, x, y, 0) && checkWall(maze, x, y - 1, 2)) // b u
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_VLINE);
            if (checkWall(maze, x + 1, y, 0) && checkWall(maze, x + 1, y - 1, 2) && checkWall(maze, x, y - 1, 1) && checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x, y, 0) && checkWall(maze, x, y - 1, 2)) // b l u
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_RTEE);
        }
    }
    return 1;
}

/* Displays only wall bit positions b3 b2 b1 b0 at solver position (x,y)*/
void displayCellAtPosition(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 13; i >= 0; --i)
    {
        if (!((*(*(maze->board + x) + y) >> i) & 1))
        {
            switch (i)
            {
            case 0: // left
                if (checkBreakLeft(maze, x, y))
                    mvaddch(startX + 2 * x, startY + 2 * y - 1, ' ');
                else
                {
                    fprintf(stderr, "[Error]: Inconsistent maze : checkBreakLeft(x, y) (%hd, %hd) - ", x, y);
                    exit(EXIT_FAILURE);
                }
                break;

            case 1: // bottom
                if (checkBreakBottom(maze, x, y))
                    mvaddch(startX + 2 * x + 1, startY + 2 * y, ' ');
                else
                {
                    fprintf(stderr, "[Error]: Inconsistent maze : checkBreakBottom(x, y) (%hd, %hd) - ", x, y);
                    exit(EXIT_FAILURE);
                }
                break;

            case 2: // right
                if (checkBreakRight(maze, x, y))
                    mvaddch(startX + 2 * x, startY + 2 * y + 1, ' ');
                else
                {
                    fprintf(stderr, "[Error]: Inconsistent maze : checkBreakRight(x, y) (%hd, %hd) - ", x, y);
                    exit(EXIT_FAILURE);
                }
                break;

            case 3: // upper
                if (checkBreakUpper(maze, x, y))
                    mvaddch(startX + 2 * x - 1, startY + 2 * y, ' ');
                else
                {
                    fprintf(stderr, "[Error]: Inconsistent maze : checkBreakUpper(x, y) (%hd, %hd) - ", x, y);
                    exit(EXIT_FAILURE);
                }
                break;

            default:
                break;
            }
        }
    }
}

void displaySolverAtPosition(unsigned short int x, unsigned short int y)
{
    mvaddch(startX + 2 * x, startY + 2 * y, ACS_BULLET);
}

/* 3: Upper, 2: Right, 1: Bottom, 0: Left*/
unsigned short int checkWall(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int pos)
{
    switch (pos)
    {
    case 3:
        return (*(*(maze->board + x) + y) >> 3) & 1;
        break;
    case 2:
        return (*(*(maze->board + x) + y) >> 2) & 1;
        break;
    case 1:
        return (*(*(maze->board + x) + y) >> 1) & 1;
        break;
    case 0:
        return (*(*(maze->board + x) + y) >> 0) & 1;
        break;
    }
    return 0;
}

void displayDecorativeWall(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ' ');
    if (x == 0)
    {
        mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_HLINE);
        if (checkWall(maze, x, y, 2) && checkWall(maze, x, y, 0))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
        }
        else if (checkWall(maze, x, y, 2))
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
        else if (checkWall(maze, x, y, 0))
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
    }
    if (x == maze->nbL - 1)
    {
        mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_HLINE);
        if (checkWall(maze, x, y, 2) && checkWall(maze, x, y, 0))
        {
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
        }
        else if (checkWall(maze, x, y, 2))
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
        else if (checkWall(maze, x, y, 0))
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
    }
    if (y == 0)
    {
        mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_VLINE);

        if (checkWall(maze, x, y, 3))
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
        if (checkWall(maze, x, y, 1))
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);
        if (checkWall(maze, x, y, 3) && checkWall(maze, x, y, 1))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);
        }
    }
    if (y == maze->nbC - 1)
    {
        mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_VLINE);

        if (checkWall(maze, x, y, 3))
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
        if (checkWall(maze, x, y, 1))
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
        if (checkWall(maze, x, y, 3) && checkWall(maze, x, y, 1))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
        }
    }
}