#include "../includes/init.h"

extern volatile unsigned short int maxX, maxY;
extern volatile unsigned short int startX, startY;

/* Ncurses library initialisation */
void ncursesInitialiser()
{
    initscr();            // Démarre le mode ncurses
    cbreak();             // Pour les saisies clavier (desac. mise en buffer)
    noecho();             // Désactive l'affichage des caractères saisis
    keypad(stdscr, TRUE); // Active les touches spécifiques
    refresh();            // Met a jour l'affichage */
    curs_set(FALSE);      // Masque le curseur */
}

void colors()
{
    if (has_colors() == FALSE)
    {
        endwin();
        fprintf(stderr, "Le terminal ne supporte pas les couleurs.\n");
        exit(EXIT_FAILURE);
    }

    start_color();

    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_BLUE, 300, 300, 1000);
    init_color(COLOR_BLACK, 0, 0, 0);
    init_color(COLOR_WHITE, 1000, 1000, 1000);
    init_color(COLOR_GREEN, 0, 1000, 0);
    init_color(COLOR_CYAN, 600, 800, 1000);
    init_color(COLOR_MAGENTA, 600, 400, 600); /*(== BROWN)*/

    //assume_default_colors(COLOR_WHITE, COLOR_BLACK);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLACK);
}

void initScreen()
{
    getmaxyx(stdscr, maxX, maxY);
    startX = maxX / 8;
    startY = maxY / 2;
}