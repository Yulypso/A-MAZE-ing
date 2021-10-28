#include "../includes/utils.h"

/* Utils */

extern volatile Maze *maze;
extern volatile Maze *mazeTmp;
extern volatile unsigned short int NB_BITS_CELL;
extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

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
void displayCellBits(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dx, unsigned short int dy)
{
    for (short int i = NB_BITS_CELL - 1; i >= 0; --i)
        mvprintw(dx, dy - i, "%hd", (*(*(maze->board + x) + y) >> i) & 1);
}

void displaySolverCoords(volatile Maze *maze, unsigned short int dx, unsigned short int dy)
{
    mvprintw(dx, dy, "Solver(x, y):             ", maze->solver.x, maze->solver.y);
    mvprintw(dx, dy, "Solver(x, y): (%hd, %hd)", maze->solver.x, maze->solver.y);
}

void displayCellNbMarks(volatile Maze *maze, unsigned short int x, unsigned short int y, unsigned short int dx, unsigned short int dy)
{
    mvprintw(dx, dy, "Nb marks:      ");
    mvprintw(dx, dy, "Nb marks: %hd", (*(*(maze->board + maze->solver.x) + maze->solver.y) >> 4) & 15);
}