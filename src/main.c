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

    unsigned short int speed, nbL, nbC, dev = 0;
    char *inputFileName = NULL;
    char *outputFileName = NULL;
    inputFileName = (char *)malloc(50 * sizeof(char));
    outputFileName = (char *)malloc(50 * sizeof(char));
    strcpy(inputFileName, "./mazes/");
    strcpy(outputFileName, "./mazes/");

    unsigned short int isReady = 0;

    for (unsigned short int i = 0; i < argc; ++i)
    {
        if (!strcmp(argv[1], "-d"))
            dev = 1;
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
        startRiMaze(inputFileName, 1, dev);
        break;

    case 2:
    case 3:
        if (!strcmp(argv[1], "-h"))
        {
            mvprintw(5, 5, "[RiMaze Manual]");
            mvprintw(7, 5, "-i <input file name>          Input file name, input file will be read from mazes/ directory");
            mvprintw(8, 5, "-o <output file name>         Output file name, output file will be store in mazes/ directory");
            mvprintw(9, 5, "-g <nb lines> <nb columns>    Generate maze with a specific size");
            mvprintw(10, 5, "-r <speed>                    Rimaze solving the input maze with a specific speed (-1: maximum speed)");
        }
        else if (!strcmp(argv[1], "-d"))
            mvprintw(0, 0, "");
        else
            mvprintw(5, 5, "[Error]: Unknown command\n\t-> Use -h to see help");
        break;

    case 5:
        if (isReady == 2)
            startRiMaze(inputFileName, speed, dev);
        break;

    case 6:
        if (isReady == 2 && (nbL > 0 && nbC > 0))
        {
            mvprintw(5, 5, "Generated maze successful: %s", outputFileName);
            generateMaze(outputFileName, nbL, nbC);
        }
        else if (isReady == 2)
            startRiMaze(inputFileName, speed, dev);
        else
            mvprintw(5, 5, "[Error]: Generated maze unsuccessful: %s\n\t-> Nb lines and nb columns must be greater than 0\n", outputFileName);
        break;

    case 10:
    case 11:
        if (isReady == 4 && (nbL > 0 && nbC > 0))
        {
            generateMaze(outputFileName, nbL, nbC);
            startRiMaze(inputFileName, speed, dev);
        }
        else
            mvprintw(5, 5, "[Error]: Generated maze unsuccessful: %s\n\t-> Nb lines and nb columns must be greater than 0\n", outputFileName);
        break;

    default:
        mvprintw(5, 5, "[Error]: Unknown command\n\t-> Use -h to see help");
    }

    attroff(COLOR_PAIR(6));
    free(inputFileName);
    free(outputFileName);
    getch();
    endwin();
    return EXIT_SUCCESS;
}