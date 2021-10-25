#include "../includes/utils.h"

extern volatile Maze maze;
extern volatile unsigned short int NB_BITS_CELL;
extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

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

/* Displays all cell's 16 bits b15 -> b0 */
void displayCellBits(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 1; i >= 0; --i)
        printw("%hd ", (*(*(maze->board + x) + y) >> i) & 1);
}

/* Displays only wall bit positions b3 b2 b1 b0 at solver position (x,y)*/
void displayCellAtPosition(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 13; i >= 0; --i)
    {
        if ((*(*(maze->board + x) + y) >> i) & 1)
        {

            switch (i)
            {
            case 0:
                mvaddch(startX + 1 * x, startY + 1 * y - 1, ACS_VLINE);
                break;

            case 1:
                mvaddch(startX + 1 * x + 1, startY + 1 * y, ACS_HLINE);
                break;

            case 2:
                mvaddch(startX + 1 * x, startY + 1 * y + 1, ACS_VLINE);
                break;

            case 3:
                mvaddch(startX + 1 * x - 1, startY + 1 * y, ACS_HLINE);
                break;

            default:
                break;
            }
        }
    }
}