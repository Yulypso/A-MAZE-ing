#include "../includes/generatemaze.h"

/* Generate maze and save it into file */

void generateMazeToFile(char *fileName, unsigned short int nbL, unsigned short int nbC, Entrance en, Exit ex)
{
    FILE *file = fopen(fileName, "w");

    if (file == NULL)
    {
        fprintf(stderr, "[Error]: Can't open %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%hd %hd %hd %hd %hd %hd\n", nbL, nbC, en.x, en.y, ex.x, ex.y);

    fclose(file);
}

void generateMaze(char *fileName, unsigned short int nbL, unsigned short int nbC, unsigned short int enX, unsigned short int enY, unsigned short int exX, unsigned short int exY)
{
    Entrance en = {.x = enX, .y = enY};
    Exit ex = {.x = exX, .y = exY};
    generateMazeToFile("./mazes/maze-4.txt", 10, 10, en, ex);
}