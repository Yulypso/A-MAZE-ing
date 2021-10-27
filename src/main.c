#include "../includes/main.h"

/* Project initialisation */
int main(int argc, char const **argv)
{
    ncursesInitialiser();
    colors();
    initScreen();

    generateMaze("./mazes/maze-5.txt", 30, 200);

    erase();
    clear();
    attron(COLOR_PAIR(6));
    startRiMaze("./mazes/maze-5.txt");
    attroff(COLOR_PAIR(6));

    getch();
    endwin();
    return EXIT_SUCCESS;
}