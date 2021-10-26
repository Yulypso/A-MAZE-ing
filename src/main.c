#include "../includes/main.h"

/* Project initialisation */
int main(int argc, char const **argv)
{
    ncursesInitialiser();
    colors();
    initScreen();

    generateMaze("./mazes/maze-5.txt", 10, 10);

    attron(COLOR_PAIR(6));
    startRiMaze("./mazes/maze-4.txt");
    attroff(COLOR_PAIR(6));

    getch();
    endwin();
    return EXIT_SUCCESS;
}