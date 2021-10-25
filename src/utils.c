#include "../includes/utils.h"

extern volatile Maze *maze;
extern volatile Maze *mazeTmp;
extern volatile unsigned short int NB_BITS_CELL;
extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

/* Utils */

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

/* For console purpose only */
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

/* For console purpose only: Displays all cell's 16 bits b15 -> b0 */
void displayCellBits(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    for (short int i = NB_BITS_CELL - 1; i >= 0; --i)
        printw("%hd ", (*(*(maze->board + x) + y) >> i) & 1);
}
