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

    char currentLine[100]; // allocating buffer to store current line

    bool inMacro = false;
    while (fgets(currentLine, sizeof(currentLine), inputFile) != NULL)
    { // reading from file, line by line, stores to currentLine

        if(strncmp(currentLine, "#ENDM" , 5) == 0) {
            inMacro = false;
            continue;
        }

        else if(strncmp(currentLine, "#ADD:", 5 ) == 0 || strncmp(currentLine, "#SUB:", 5 ) == 0 ||
                strncmp(currentLine, "#MULT:", 6 ) == 0 || strncmp(currentLine, "#DIV:", 5 ) == 0 ||
                strncmp(currentLine, "#REM:", 5 ) == 0)
            inMacro = true;

        if(!inMacro) {
            parse(currentLine);
            is_macro(outputFile, argv, argc); // this calls expand() and createPT() if necessary
        }
    }

    fclose(outputFile);
    fclose(inputFile);

    return 0;
}