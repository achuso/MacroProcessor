// CSE 232 Group 5
// Term Project - Macroprocessors
// Written by:
// Onat Ribar           - createPT()
// Mahny Barazandehtar  - is_macro()
// Ayşegül Tekeli       - read()
// Ece Tipici           - parse()
// Emir Devlet Ertörer  - expand()

#include <stdio.h>
#include <string.h>
#include "group5_functions.h"

int main(int argc, char **argv)
{

    // These could be made into preprocessor directives too
    char *inputFileName = "group5_input.txt";
    char *outputFileName = "group5_output.asm";

    // Open file to read
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL)
    {
        printf("error opening file. exiting...");
    }
    // Open file to write on
    FILE *outputFile = fopen(outputFileName, "a");
    if (outputFile == NULL)
    {
        printf("error opening file. exiting...");
    }

    read(inputFileName);

    char *currentLine = malloc(sizeof(char) * 64); // allocating buffer to store current line, 64 chars max.

    while (fgets(currentLine, strlen(currentLine) + 1, inputFile) != NULL)
    { // reading from file, line by line, stores to currentLine+
        parse(currentLine, field);
        is_macro(field, outputFileName, argv, PT, buffer, m_count); // this calls expand() and createPT() if necessary
    }

    free(currentLine);
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}