#include "../includes/main.h"

/* Project starting point */
int main(int argc, char const **argv)
{
    ncursesInitialiser();
    colors();
    initScreen();
    erase();
    clear();
    attron(COLOR_PAIR(6));

    unsigned short int speed, nbL, nbC;
    char *inputFileName = NULL;
    char *outputFileName = NULL;
    inputFileName = (char *)malloc(50 * sizeof(char));
    outputFileName = (char *)malloc(50 * sizeof(char));
    strcpy(inputFileName, "./mazes/");
    strcpy(outputFileName, "./mazes/");

    unsigned short int isReady = 0;

    for (unsigned short int i = 0; i < argc; ++i)
    {
        if (!(strcmp(argv[i], "-r")))
        {
            speed = atoi(argv[i + 1]);
            ++isReady;
        }
        if (!(strcmp(argv[i], "-i")))
        {
            strcat(inputFileName, argv[i + 1]);
            ++isReady;
        }
        if (!(strcmp(argv[i], "-o")))
        {
            strcat(outputFileName, argv[i + 1]);
            ++isReady;
        }
        if (!(strcmp(argv[i], "-g")))
        {
            nbL = atoi(argv[i + 1]);
            nbC = atoi(argv[i + 2]);
            ++isReady;
        }
    }

    switch (argc)
    {
    case 1:
        strcat(outputFileName, "maze.txt");
        nbL = 10;
        nbC = 10;
        generateMaze(outputFileName, nbL, nbC);

        strcat(inputFileName, "maze.txt");
        speed = 1;
        startRiMaze(inputFileName, 1);
        break;

    case 5:
        if (isReady == 2)
            startRiMaze(inputFileName, speed);
        break;

    case 6:
        if (isReady == 2)
            startRiMaze(inputFileName, speed);
        break;

    case 10:
        if (isReady == 4)
        {
            generateMaze(outputFileName, nbL, nbC);
            startRiMaze(inputFileName, speed);
        }
        break;
    }

    attroff(COLOR_PAIR(6));
    free(inputFileName);
    free(outputFileName);
    getch();
    endwin();
    return EXIT_SUCCESS;
}