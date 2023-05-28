#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <stdbool.h>

struct mac {
    char mname[8]; //macro name
    char param[10][4]; // max. 10 paramaters, each parameter max. 3 characters
    char macro[256]; // macro body
};

struct mac buffer[10]; // memory buffer for 10 macro definitions
int m_count; // counts the number of macro definitions
int param_count;


char field[10][7];

struct pt {
    char mname[8];          // macro name
    int  nparams;           // number of parameters
    char dummy[10][4];      // dummy parameters
    char actual[10][4];     // actual parameters
};
struct pt PT;
const char punctuation[] = " ,:";

int read(char* filename) {
    m_count = 0;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File couldn't be opened.\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) ) {
        if (strncmp(line, "#ENDM", 5) == 0) {
            m_count++;
        } else if (line[0] == '#' && strstr(line, ":") ) {
            char* p;
            char* lineP = line;

            p = strtok(lineP, punctuation);
            if (p) {
                strcpy(buffer[m_count].mname, p + 1);
            }
            int param_count = 0;
            while (p = strtok(NULL, punctuation)) {
                strncpy(buffer[m_count].param[param_count], p, 4);
                param_count++;
            }
        } else {
            if (line[strlen(line) - 1] != '\0') {
                strcat(line, "\n");
            }
            strcat(buffer[m_count].macro, line);
        }
    }

    fclose(file);
    return m_count;
}

// ISOLATE MACRO NAMES IN FIELD (FOR EXAMPLE "#ADD:" BECOMES "ADD")
// REEVALUATE FUNCTION WITH FIELD[][]
void parse(char* filename, char field[10][7]) {
    int max_field_index = 0;
    char* myfield;
    bool firstField = true;

    myfield = strtok(filename, " ()=’");

    while (myfield != NULL && max_field_index < 10) {
        int length = strlen(myfield);
        int j = 0;

        if (firstField && myfield[0] == '#') {
            for (int i = 0; i < length; i++) {
                myfield[i] = myfield[i + 1];
            }
        }
        firstField = false;

        if (strcmp(myfield, "MACRO") == 0) {
            myfield = strtok(NULL, " ()=’");
        }

        for (int i = 0; i < length; i++) {
            if (myfield[i] != ':' && myfield[i] != ',') {
                field[max_field_index][j] = myfield[i];
                j++;
            }
        }
        field[max_field_index][j] = '\0';
        max_field_index++;

        myfield = strtok(NULL, " ()=’");
    }
}

void is_macro(char field[10][7], FILE* outputFile, char** argv, struct pt PT, struct mac buffer[10], int m_count){  

    // check if the line starts with an if

    if(strncmp(field[0], "if", 2) == 0){


    // check for conditions:
    // example: field format: if $2 ABC M3 200 300 
    // $2 is a condition parameter that is given as an argument such that arg[2]
    // get the number after the $, pass it into arg[], and check if its equal to the thing inside the paranthesis
        
        int argVal = atoi(&field[1][1]);  // to extract the number after the $ and store it as an int
        // OR we can do this field[1][1] - '0';

        if (strcmp(argv[argVal], field[2]) == 0){

            expand(outputFile, PT, field, buffer, m_count);
            
        }
        else{
            return;
        }
    
    }
    // check if the line starts with a macro name
    else if (strncmp(field[0], "ADD", 3) == 0 || strncmp(field[0], "SUB", 3) == 0 || strncmp(field[0], "MULT", 4) == 0 || strncmp(field[0], "DIV", 3) == 0 || strncmp(field[0], "REM", 3) == 0){ 

            expand(outputFile, PT, field, buffer, m_count);       

    }
    else{
        //write the line into the .asm file       
        
        for(int i = 0; field[i][0] != '\0'; i++){

            fprintf(outputFile, "%s ", field[i]); // writes the contents of the line into f1

        }

        fprintf(outputFile, "\n");
    }
    
}

void expand(FILE* file, struct pt PT, char field[10][7], struct mac buffer[10], int m_count){ // now the function recieves the file as a parameter
    createPT(field, buffer, m_count);  
    // take a line from the macro body
    // check if there are dummy parameters
        // if there are, substitue them with the actual parameters using the parameter table
    // and append it to f1.asm

    // iterate and find the macro definition for the current macro.
    struct mac* macroDef = NULL;
    for(int i = 0; i < m_count; i++) {
        if (strcmp(buffer[i].mname, PT.mname) == 0) {
            macroDef = &buffer[i];
            break;
        }
    }    
    // If the macro definition was not found return.
    if (macroDef == NULL) {
        printf("Macro definition not found.\n");
        return;
    }
    
    // For each line in the macro definition.
    char* line = strtok(macroDef->macro, "\n"); // goes line by line through the macro body
    while (line != NULL) {
        // For each dummy parameter in the parameter table.
        for(int i = 0; i < PT.nparams; i++) {
            // Replace all occurrences of the dummy parameter in the line with its corresponding actual parameter.
            while ((line = strstr(line, PT.dummy[i])) != NULL) { // searches for the first occurrence of the dummy parameter in the current line.
                // copies the actual parameter over the dummy parameter in the line
                strncpy(line, PT.actual[i], strlen(PT.actual[i]));
                // Move the line pointer forward by the length of the actual parameter.
                line += strlen(PT.actual[i]);
            }
        }       
        // Write the line to the .asm file.
        fprintf(file, "%s\n", line);  
        line = strtok(NULL, "\n"); //  gets the next line 
    }
    
}

// field is the array returned from parse()
// array of macro definitions returned from read()
// m_count, macro def count returned from read()
void createPT(char field[10][7], struct mac buffer[10], int m_count) { 
    // Initialize/set pt to zero
    //PT = emptyPT;
    memset(&PT, 0, sizeof(struct pt));
    PT.nparams = 0;

    // handle a regular macro call (i.e. "M1 479 61")
    if(field[0][0] != '#') {

        // set macro name
        strcpy(PT.mname, field[0]);

        // get number of parameters
        for(int i = 1; field[i][0] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++)
            if(strcmp(buffer[i].mname, field[0]) == 0) {
                for(int j = 0; buffer[i].param[j][0] != '\0'; j++) {
                    // get dummy parameter
                    for(int k = 0; k < 7; k++) {
                        PT.dummy[j][k] = buffer[i].param[j][k];
                    }
                }
                break;
            }

        // set actual parameters
        for(int i = 1; field[i][0] != '\0'; i++)
            strcpy(PT.actual[i-1], field[i]);
    }
    // if macro starts with an if-statement
    else {
        /*
        1st field is the #if substring
        2nd and 3rd fields are the condition variables
        4th field is the macro name
        5th and the rest are parameters
        */

        // set macro name
        strcpy(PT.mname, field[3]);

        // get number of parameters
        for(int i = 1; field[i][4] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++)
            // !!!!! CHECK ON WITH HOW AYSEGUL STORES MACRO NAMES TO BUFFER, IF SHE'S STORING THE # AT THE BEGINNING, EDIT BOTTON LINE ACCORDINGLY
            if(strncmp(buffer[i].mname, field[3], 7) == 0) {
                for(int j = 0; buffer[i].param[j][0] != '\0'; j++) {
                    // get dummy parameter
                    for(int k = 0; k < 7; k++) {
                        PT.dummy[j][k] = buffer[i].param[j][k];
                    }
                }
                break;
            }

        // set actual parameters
        for(int i = 4; field[i][0] != '\0'; i++)
            strcpy(PT.actual[i-4], field[i]);
    }

    return;
}
