#include "../includes/utils.h"

extern volatile Maze *maze;
extern volatile Maze *mazeTmp;
extern volatile unsigned short int NB_BITS_CELL;
extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

/* Utils to load, read and writes files */

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
    mazeTmp->board = (unsigned short int **)malloc(mazeTmp->nbL * sizeof(unsigned short int *));
    for (unsigned short int i = 0; i < mazeTmp->nbL; ++i)
        *(mazeTmp->board + i) = (unsigned short int *)malloc(mazeTmp->nbC * sizeof(unsigned short int));

    for (unsigned short int i = 0; i < mazeTmp->nbL; ++i)
        for (unsigned short int j = 0; j < mazeTmp->nbC; ++j)
            *(*(mazeTmp->board + i) + j) = 15;
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

/* Displays all cell's 16 bits b15 -> b0 */
void displayCellBits(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 1; i >= 0; --i)
        printw("%hd ", (*(*(maze->board + x) + y) >> i) & 1);
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
    if (checkWall(maze, x, y - 1, 2)) // Error if right wall
    {
        fprintf(stderr, "%s", "[Error]: Inconsistent maze -> checkBreakLeft() \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (x != 0 && y != 0 && x != maze->nbL - 1 && y != maze->nbC - 1) // not in borders
        {
            /* Left Upper part */
            if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0) && checkWall(maze, x - 1, y, 1)) // l u r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_BTEE);
            else if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0) && checkWall(maze, x - 1, y, 1)) // u r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LLCORNER);
            else if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y, 1)) // l r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_HLINE);
            else if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y, 0)) // l u
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LRCORNER);
            else if (checkWall(maze, x - 1, y - 1, 1) && checkWall(maze, x, y - 1, 3)) // l
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');
            else if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x, y - 1, 0)) // u
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');
            else if (checkWall(maze, x - 1, y, 1) && checkWall(maze, x, y, 3)) // r
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ' ');

            /* Left Bottom part */
            return 1;
        }
        else // in borders
        {
            return 0; // TODO EDIT
        }
    }
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
                break;

            case 1: // bottom
                mvaddch(startX + 2 * x + 1, startY + 2 * y, ' ');
                break;

            case 2: // right
                mvaddch(startX + 2 * x, startY + 2 * y + 1, ' ');
                break;

            case 3: // upper
                mvaddch(startX + 2 * x - 1, startY + 2 * y, ' ');
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
    mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_PLUS);
    if (x != 0 && y != 0 && x != maze->nbL - 1 && y != maze->nbC - 1)
    {
        // if sees corners
        /*if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x - 1, y + 1, 0))
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_LLCORNER);

        if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y - 1, 1))
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LRCORNER);

        if (checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x + 1, y - 1, 2))
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_URCORNER);

        if (checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x + 1, y + 1, 0))
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_ULCORNER);*/

        /*switch (*(*(maze->board + x) + y))
        {
        case 0:
        case 1:
        case 2:
        case 4:
        case 8:
        case 5:
        case 10:
            break;

        case 3: // Bottom-left wall
            if (checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y - 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);
            break;

        case 6: // Bottom-right wall
            if (checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y + 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
            break;

        case 9: // Upper-left wall
            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y - 1, 2))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            else if (checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
            break;

        case 12: // Upper-right wall
            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y + 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            else if (checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
            break;

        case 7:
            if (checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y - 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);

            if (checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y + 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
            break;

        case 11:
            if (checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y - 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);

            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y - 1, 2))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            else if (checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
            break;

        case 13:
            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y + 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            else if (checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);

            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y - 1, 2))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            else if (checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
            break;

        case 14:
            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y + 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            else if (checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);

            if (checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y + 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
            break;

        case 15:
            if (checkWall(maze, x + 1, y - 1, 3) && checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y - 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y - 1, 2))
                mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);

            if (checkWall(maze, x + 1, y + 1, 3) && checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x + 1, y + 1, 3))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            else if (checkWall(maze, x + 1, y + 1, 0))
                mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);

            if (checkWall(maze, x - 1, y - 1, 2) && checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y - 1, 2))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            else if (checkWall(maze, x - 1, y - 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);

            if (checkWall(maze, x - 1, y + 1, 1) && checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_PLUS);
            else if (checkWall(maze, x - 1, y + 1, 1))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            else if (checkWall(maze, x - 1, y + 1, 0))
                mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
            break;

        default:
            break;
        }*/
    }
    else if (x == 0)
    {
        if (checkWall(maze, x, y, 2) && checkWall(maze, x, y, 0))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
        }
        else
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_HLINE);
        if (checkWall(maze, x, y, 2))
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_TTEE);
        else if (checkWall(maze, x, y, 0))
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_TTEE);
    }
    else if (x == maze->nbL - 1)
    {
        if (checkWall(maze, x, y, 2) && checkWall(maze, x, y, 0))
        {
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
        }
        else
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_HLINE);
        if (checkWall(maze, x, y, 2))
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_BTEE);
        else if (checkWall(maze, x, y, 0))
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_BTEE);
    }
    else if (y == 0)
    {
        if (checkWall(maze, x, y, 3) && checkWall(maze, x, y, 1))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);
        }
        else
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_VLINE);
        if (checkWall(maze, x, y, 3))
            mvaddch(startX + 2 * x - 1, startY + 2 * y - 1, ACS_LTEE);
        else if (checkWall(maze, x, y, 1))
            mvaddch(startX + 2 * x + 1, startY + 2 * y - 1, ACS_LTEE);
    }
    else if (y == maze->nbC - 1)
    {
        if (checkWall(maze, x, y, 3) && checkWall(maze, x, y, 1))
        {
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
        }
        else
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_VLINE);
        if (checkWall(maze, x, y, 3))
            mvaddch(startX + 2 * x - 1, startY + 2 * y + 1, ACS_RTEE);
        else if (checkWall(maze, x, y, 1))
            mvaddch(startX + 2 * x + 1, startY + 2 * y + 1, ACS_RTEE);
    }
}