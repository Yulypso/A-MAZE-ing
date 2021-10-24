#include "../includes/main.h"
#include "../includes/init.h"

/* Project initialization */
int main(int argc, char const **argv)
{
    ncurses_initialiser();
    colors();

    mvprintw(10, 50, "Welcome in RiMaze");

    getch();
    endwin();
    return EXIT_SUCCESS;
}
