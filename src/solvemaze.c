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

void displayMarkers(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short j = 0; j < maze->nbC; ++j)
        {
            move(startX + i, startY + j);
            mvprintw(startX + 2 * i, 30 + startY + 3 * j, "%hd ", getNbCellMarks(maze, i, j));
        }
}

void displayMainPath(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short j = 0; j < maze->nbC; ++j)
            !isMainPathCellMarked(maze, i, j) ?: mvaddch(startX + 2 * i, startY + 2 * j, ACS_BULLET);
}

void displayMountains(volatile Maze *maze)
{
    for (unsigned short int i = 0; i < maze->nbL; ++i)
        for (unsigned short j = 0; j < maze->nbC; ++j)
            !isMountain(maze, i, j) ?: mvaddch(startX + 2 * i, startY + 2 * j, ACS_BLOCK);
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

void markMainPathCell(volatile Maze *maze)
{
    *(*(maze->board + maze->solver.x) + maze->solver.y) |= 3 << 14;
}

void unmarkMainPathCell(volatile Maze *maze)
{
    *(*(maze->board + maze->solver.x) + maze->solver.y) &= ~(1UL << 15);
}

unsigned short int isMainPathCellAlreadyMarked(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    return ((((*(*(maze->board + x) + y) >> 14) & 1) == 1) && (((*(*(maze->board + x) + y) >> 15) & 0) == 0));
}

unsigned short int isMainPathCellMarked(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    return (*(*(maze->board + x) + y) >> 15) & 1;
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

    displayMarkers(maze);

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

unsigned short int isHalfDeadEnd(volatile Maze *maze)
{
    if ((!checkUpperWall(maze) + !checkRightWall(maze) + !checkBottomWall(maze) + !checkLeftWall(maze)) == 2)
        return 1;
    return 0;
}

unsigned short int isSuperDeadEnd(volatile Maze *maze)
{
    if ((!checkUpperWall(maze) + !checkRightWall(maze) + !checkBottomWall(maze) + !checkLeftWall(maze)) == 4)
        return 1;
    return 0;
}

unsigned short int isMountain(volatile Maze *maze, unsigned short int x, unsigned short int y)
{
    if (getNbCellMarks(maze, x, y) == 15)
        return getNbCellMarks(maze, x, y) / 2;
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

unsigned short int checkSpeed(unsigned short int speed)
{
    if (speed < 1)
        return 1;
    return speed;
}

void solveMaze(volatile Maze *maze, unsigned short int speed, unsigned short int dev)
{
    initRand();
    unsigned short int solving = 1;
    while (!(maze->solver.x == maze->exit.x && maze->solver.y == maze->exit.y) && solving)
    {
        if (isSuperDeadEnd(maze))
        {
            attron(COLOR_PAIR(1));
            mvprintw(1, 12, "No solution found");
            attroff(COLOR_PAIR(1));
            solving = 0;
            break;
        }

        if (isDeadEnd(maze))
        {
            markCell(maze, 15);
            unmarkMainPathCell(maze);
        }
        else
            markCell(maze, 1);

        if (isMountain(maze, maze->solver.x, maze->solver.y))
            unmarkMainPathCell(maze);

        if (isMainPathCellAlreadyMarked(maze, maze->solver.x, maze->solver.y) && isHalfDeadEnd(maze))
            unmarkMainPathCell(maze);
        else
            markMainPathCell(maze);

        for (volatile unsigned int i = 0; i < (2000000 / checkSpeed(speed)); ++i)
        {
            if (i == (2000000 / checkSpeed(speed)) - 1)
            {
                attron(COLOR_PAIR(5));
                displayMountains(maze);
                attroff(COLOR_PAIR(5));
                displayEntrance(maze);
                displayExit(maze);
                displaySolver(maze);
                /* For development only */
                dev ?: displayControlPanel(maze);
                refresh();
            }
            mvprintw(0, 0, "");
        }

        switch (determineBestPath(maze))
        {
        case 3:
            hideSolver(maze);
            //displaySolverTrace(maze);
            !isMainPathCellMarked(maze, maze->solver.x - 1, maze->solver.y) ?: unmarkMainPathCell(maze);
            solverGoUpper(maze);
            if (isMountain(maze, maze->solver.x + 1, maze->solver.y) && isHalfDeadEnd(maze))
                markCell(maze, 15);
            break;
        case 2:
            hideSolver(maze);
            //displaySolverTrace(maze);
            !isMainPathCellMarked(maze, maze->solver.x, maze->solver.y + 1) ?: unmarkMainPathCell(maze);
            solverGoRight(maze);
            if (isMountain(maze, maze->solver.x, maze->solver.y - 1) && isHalfDeadEnd(maze))
                markCell(maze, 15);
            break;
        case 1:
            hideSolver(maze);
            //displaySolverTrace(maze);
            !isMainPathCellMarked(maze, maze->solver.x + 1, maze->solver.y) ?: unmarkMainPathCell(maze);
            solverGoBottom(maze);
            if (isMountain(maze, maze->solver.x - 1, maze->solver.y) && isHalfDeadEnd(maze))
                markCell(maze, 15);
            break;
        case 0:
            hideSolver(maze);
            //displaySolverTrace(maze);
            !isMainPathCellMarked(maze, maze->solver.x, maze->solver.y - 1) ?: unmarkMainPathCell(maze);
            solverGoLeft(maze);
            if (isMountain(maze, maze->solver.x, maze->solver.y + 1) && isHalfDeadEnd(maze))
                markCell(maze, 15);
            break;
        case 4:
            attron(COLOR_PAIR(1));
            mvprintw(1, 12, "No solution found");
            attroff(COLOR_PAIR(1));
            solving = 0;
            break;
        }
    }
    if (solving == 1)
    {
        attron(COLOR_PAIR(3));
        mvprintw(1, 12, "Found a solution!");
        displayMainPath(maze);
        displayEntrance(maze);
        displayExit(maze);
        displaySolver(maze);
        refresh();
        dev ?: displayControlPanel(maze);
        attroff(COLOR_PAIR(3));
    }
}