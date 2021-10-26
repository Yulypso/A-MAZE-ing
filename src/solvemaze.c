#include "../includes/solvemaze.h"

/* Solve maze */

extern volatile Maze *maze;
extern volatile unsigned short int startX, startY;

void solverGoUpper(volatile Maze *maze)
{
    maze->solver.x -= 1;
}

void solverGoRight(volatile Maze *maze)
{
    maze->solver.y += 1;
}

void solverGoBottom(volatile Maze *maze)
{
    maze->solver.x += 1;
}

void solverGoLeft(volatile Maze *maze)
{
    maze->solver.y -= 1;
}

void displaySolver(volatile Maze *maze)
{
    mvaddch(startX + 2 * maze->solver.x, startY + 2 * maze->solver.y, ACS_DIAMOND);
}

// Increments counter of the current cell
void markCell(volatile Maze *maze, unsigned short int nbMarks)
{
    *(*(maze->board + maze->solver.x) + maze->solver.y) |= (*(*(maze->board + maze->solver.x) + maze->solver.y) += nbMarks << 4);
}

unsigned short int getNbCellMarks(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    return (*(*(maze->board + x) + y) >> 4) & 15;
}

unsigned short int checkUpperWall(volatile Maze *maze)
{
    if ((*(*(maze->board + maze->solver.x) + maze->solver.y) & 8) >> 3 == 1)
        return 0;
    return 1;
}

unsigned short int checkRightWall(volatile Maze *maze)
{
    if ((*(*(maze->board + maze->solver.x) + maze->solver.y) & 4) >> 2 == 1)
        return 0;
    return 1;
}

unsigned short int checkBottomWall(volatile Maze *maze)
{
    if ((*(*(maze->board + maze->solver.x) + maze->solver.y) & 2) >> 1 == 1)
        return 0;
    return 1;
}

unsigned short int checkLeftWall(volatile Maze *maze)
{
    if ((*(*(maze->board + maze->solver.x) + maze->solver.y) & 1) >> 0 == 1)
        return 0;
    return 1;
}

// return index of min value and random index for equals min value
unsigned short int getMinTabIndex(unsigned short int *tab)
{
    unsigned short int equalNbMarkersIndex[4] = {4, 4, 4, 4};
    unsigned short int min = 15, nbEquals = 0;

    for (unsigned short int i = 0; i < 4; ++i)
    {
        if (*(tab + i) != 16)
        {

            if (*(tab + i) < min)
            {
                min = *(tab + i);
                nbEquals = 0;
                *(equalNbMarkersIndex + 0) = i;
                *(equalNbMarkersIndex + 1) = 4;
                *(equalNbMarkersIndex + 2) = 4;
                *(equalNbMarkersIndex + 3) = 4;
            }
            else if (*(tab + i) == min)
            {
                ++nbEquals;
                switch (nbEquals)
                {
                case 1:
                    *(equalNbMarkersIndex + 1) = i;
                    break;
                case 2:
                    *(equalNbMarkersIndex + 2) = i;
                    break;
                case 3:
                    *(equalNbMarkersIndex + 3) = i;
                    break;
                default:
                    break;
                }
            }
        }
    }

    unsigned short int random = 0;
    for (unsigned short int i = 0; i < 4; ++i)
    {
        if (*(equalNbMarkersIndex + i) != 4)
            ++random;
    }

    mvprintw(8, 100, "%hd", *(equalNbMarkersIndex + rand() % random));

    return *(equalNbMarkersIndex + rand() % random);
}

unsigned short int determineBestPath(volatile Maze *maze)
{
    unsigned short int markers[4] = {16, 16, 16, 16};

    if (checkUpperWall(maze))
        markers[3] = getNbCellMarks(maze, maze->solver.x - 1, maze->solver.y);
    if (checkRightWall(maze))
        markers[2] = getNbCellMarks(maze, maze->solver.x, maze->solver.y + 1);
    if (checkBottomWall(maze))
        markers[1] = getNbCellMarks(maze, maze->solver.x + 1, maze->solver.y);
    if (checkLeftWall(maze))
        markers[0] = getNbCellMarks(maze, maze->solver.x, maze->solver.y - 1);

    return markers[getMinTabIndex(markers)];
}

void solveMaze(volatile Maze *maze)
{
    //solverGoRight(maze);
    //solverGoBottom(maze);
    //markCell(maze, 1);
    //markCell(maze, 1);
    //markCell(maze);
    //solverGoRight(maze);
    //solverGoLeft(maze);

    displaySolver(maze);
    displaySolverCoords(maze, 0, 35);
    displayCellBits(maze, maze->solver.x, maze->solver.y, 0, 75);
    displayCellNbMarks(maze, maze->solver.x, maze->solver.y, 0, 80);

    if (checkUpperWall(maze))
        mvprintw(0, 100, "Upper path found");
    if (checkRightWall(maze))
        mvprintw(1, 100, "Right path found");
    if (checkBottomWall(maze))
        mvprintw(2, 100, "Bottom path found");
    if (checkLeftWall(maze))
        mvprintw(3, 100, "Left path found");

    mvprintw(5, 100, "Best path: %hd", determineBestPath(maze));
}