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

int read(char* filename);
void parse(char* line);
void is_macro(const char field[][], char* outputFilename, char** argv);
void expand();
void createPT(const char field[][], struct mac, int);

// read() reads macro definitions atop the input file, stores them in mac buffer and updates m_count accordingly
struct mac {
    char mname[8];
    char param[10][4];
    char macro[256];
};
struct mac buffer[10];
int m_count;

// parse() separates fields in a line and places them in field**
char field[10][7];

// parameter table created by createPT(), utilized by expand() to expand on macro instances
struct pt {
    char mname[8];
    int nparams;
    char dummy[10][4];
    char actual[10][4];
};
struct pt PT;
struct pt emptyPT; // used to empty the PT above

int main(int argc, char** argv) {

    // These could be made into preprocessor directives too
    char* inputFileName = "group5_input.txt";
    char* outputFileName = "group5_output.asm";

    // Open file to read
    FILE* inputFile = fopen(inputFileName, "r");
    if(inputFile == NULL) {
        printf("error opening file. exiting...");
    }
    // Open file to write on
    FILE* outputFile = fopen(outputFileName, "a");
    if(outputFile == NULL) {
        printf("error opening file. exiting...");
    }

    read(inputFileName);

    char* currentLine = malloc(sizeof(char) * 64); // allocating buffer to store current line, 64 chars max.

    while(fgets(currentLine, sizeof(currentLine), inputFile) != NULL) { // reading from file, line by line, stores to currentLine+
        parse(currentLine);
        is_macro(field, outputFileName, argv); // this calls expand() and createPT() if necessary
    }

    free(currentLine);
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}