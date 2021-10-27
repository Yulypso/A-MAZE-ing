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

void displaySolverTrace(volatile Maze *maze)
{
    mvaddch(startX + 2 * maze->solver.x, startY + 2 * maze->solver.y, ACS_BULLET);
}

void hideSolver(volatile Maze *maze)
{
    mvaddch(startX + 2 * maze->solver.x, startY + 2 * maze->solver.y, ' ');
}

// Increments counter of the current cell (max markers = 15)
void markCell(volatile Maze *maze, unsigned short int nbMarks)
{
    if (getNbCellMarks(maze, maze->solver.x, maze->solver.y) + nbMarks <= 15)
        *(*(maze->board + maze->solver.x) + maze->solver.y) |= (*(*(maze->board + maze->solver.x) + maze->solver.y) += nbMarks << 4);
    else
        *(*(maze->board + maze->solver.x) + maze->solver.y) |= 15 << 4;
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

void displayControlPanel(volatile Maze *maze)
{
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
    mvprintw(7, 100, "isDeadEnd: %hd", isDeadEnd(maze));
    mvprintw(9, 100, "nb cell marks: %hd", getNbCellMarks(maze, maze->solver.x, maze->solver.y));
    mvprintw(11, 100, "is Exit: %hd", maze->solver.x == maze->exit.x && maze->solver.y == maze->exit.y);
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

    return *(equalNbMarkersIndex + rand() % random);
}

unsigned short int isDeadEnd(volatile Maze *maze)
{
    if ((!checkUpperWall(maze) + !checkRightWall(maze) + !checkBottomWall(maze) + !checkLeftWall(maze)) == 3)
        return 1;
    return 0;
}

unsigned short int isSuperDeadEnd(volatile Maze *maze)
{
    if ((!checkUpperWall(maze) + !checkRightWall(maze) + !checkBottomWall(maze) + !checkLeftWall(maze)) == 4)
        return 1;
    return 0;
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

    return getMinTabIndex(markers);
}

void solveMaze(volatile Maze *maze)
{
    unsigned short int solving = 1;
    while (!(maze->solver.x == maze->exit.x && maze->solver.y == maze->exit.y) && solving)
    {
        displayEntrance(maze);
        displayExit(maze);

        if (isSuperDeadEnd(maze))
        {
            attron(COLOR_PAIR(1));
            mvprintw(1, 12, "No solution found");
            attroff(COLOR_PAIR(1));
            solving = 0;
            break;
        }
        isDeadEnd(maze) ? markCell(maze, 3) : markCell(maze, 1);

        switch (determineBestPath(maze))
        {
        case 3:
            hideSolver(maze);
            displaySolverTrace(maze);
            solverGoUpper(maze);
            break;
        case 2:
            hideSolver(maze);
            displaySolverTrace(maze);
            solverGoRight(maze);
            break;
        case 1:
            hideSolver(maze);
            displaySolverTrace(maze);
            solverGoBottom(maze);
            break;
        case 0:
            hideSolver(maze);
            displaySolverTrace(maze);
            solverGoLeft(maze);
            break;
        case 4:
            attron(COLOR_PAIR(1));
            mvprintw(1, 12, "No solution found");
            attroff(COLOR_PAIR(1));
            solving = 0;
            break;
        }

        //for (volatile unsigned int i = 0; i < 2000000; ++i)
        for (volatile unsigned int i = 0; i < 10; ++i)
        {
            //if (i == 1999999)
            if (i == 0)
            {
                displaySolver(maze);
                /* For development only */
                //displayControlPanel(maze);
                refresh();
            }
            mvprintw(0, 0, "");
        }
    }
    if (solving == 1)
    {
        attron(COLOR_PAIR(3));
        mvprintw(1, 12, "Found a solution!");
        attroff(COLOR_PAIR(3));
    }
}