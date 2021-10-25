#include "../includes/main.h"

/* Project initialisation */
int main(int argc, char const **argv)
{
    ncursesInitialiser();
    colors();
    initScreen();
    attron(COLOR_PAIR(6));
    startRiMaze("./mazes/maze-2.txt");
    attroff(COLOR_PAIR(6));
    getch();
    endwin();
    return EXIT_SUCCESS;
}