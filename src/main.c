#include "../includes/main.h"

/* Project initialisation */
int main(int argc, char const **argv)
{
    ncursesInitialiser();
    colors();
    startRiMaze("./mazes/maze-1.txt");
    getch();
    endwin();
    return EXIT_SUCCESS;
}