#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int read(char*);                     // Ayşegül Tekeli
void parse(char*);                   // Ece Tipici
void createPT();                     // Onat Ribar
void is_macro(FILE*, char**, int);   // Mahny Barazandehtar
void expand(FILE*);                  // Emir Devlet Ertörer

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

int read(char* filename) {
    const char punctuation[] = " ,:";
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
            param_count = 0;
            while ((p = strtok(NULL, punctuation))) {
                strncpy(buffer[m_count].param[param_count], p, 4);// param includes "MACRO" so you should start 1 instead of 0 when you using param array
                param_count++;
            }
            // Counteracts the exception where the last parameter's last character has a '\n' mixed in
            for(int i = 0; i < 4; i++)
                if(buffer[m_count].param[param_count-1][i] == '\n') {
                    buffer[m_count].param[param_count-1][i] = '\0';
                    break;
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
void parse(char* filename) {
    int max_field_index = 0;
    char* myfield;
    bool firstField = true;

    myfield = strtok(filename, " ()=’\t");

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

void is_macro(FILE* outputFile, char** argv, int argc){
    // check if the line starts with an if
    if(strncmp(field[0], "if", 2) == 0){
        
        // check for conditions:
        // example: field format: if $2 ABC M3 200 300
        // $2 is a condition parameter that is given as an argument such that arg[2]
        // get the number after the $, pass it into arg[], and check if its equal to the thing inside the parenthesis
        int argVal = atoi(&field[1][1]);  // to extract the number after the $ and store it as an int
        // OR we can do this field[1][1] - '0';
        if (argc > argVal && strcmp(argv[argVal], field[2]) == 0){
            expand(outputFile);

        }
        else{
            return;
        }

    }
    else if (strncmp(field[0], "ADD", 3) == 0 || strncmp(field[0], "SUB", 3) == 0 || strncmp(field[0], "MULT", 4) == 0 || strncmp(field[0], "DIV", 3) == 0 || strncmp(field[0], "REM", 3) == 0){
        expand(outputFile);

    }

    else{

        //write the line into the .asm file
        fprintf(outputFile, "\t");
        
        for(int i = 0; field[i][0] != '\0'; i++){

            fprintf(outputFile, "%s ", field[i]); // writes the contents of the line into f1

        }

        fprintf(outputFile, "\n");
    }

}

void expand(FILE* file){
    createPT();

    // find the correct macro in the buffer
    for(int i = 0; i < m_count; i++){
        if(strcmp(buffer[i].mname, PT.mname) == 0){
            char* p = buffer[i].macro;

            while(*p != '\0'){
                // if it encounters a '$' sign, this indicates a dummy parameter
                if(*p == '$'){
                    // print the $
                    fprintf(file, "%s", "$");
                    // skip it
                    p++;

                    // to consider multiple characters, i am creating a temporary buffer
                    char temp_param[4] = "";  // maximum of 5 characters for parameter names, like "A1"
                    for(int k = 0; k < 4 && *p != ' ' && *p != '\n' && *p != '\0'; k++, p++)
                        temp_param[k] = *p;

                    // matching the dummy parameters
                    for(int j = 1; j < PT.nparams + 1; j++) {
                        if (strcmp(temp_param, PT.dummy[j]) == 0) {
                            // print the corresponding actual parameter to the output file
                                fprintf(file, "%s", PT.actual[j]);
                                break;
                        }
                    }

                }
                else {
                    // print the character as is
                    fprintf(file, "%c", *p);
                    p++;
                }
            }

            fprintf(file, "\n");
        }
    }
}

// field is the array returned from parse()
// array of macro definitions returned from read()
// m_count, macro def count returned from read()
void createPT() {
    // Initialize/set pt to zero
    //PT = emptyPT;
    memset(&PT, '\0', sizeof(struct pt));
    PT.nparams = 0;

    // handle a regular macro call (i.e. "M1 479 61")
    if(strncmp(field[0], "if", 2) != 0) {
        // set macro name
        strcpy(PT.mname, field[0]);

        // get number of parameters
        for(int i = 1; field[i][0] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++) {
            if (strcmp(buffer[i].mname, field[0]) == 0) {
                for (int j = 1; buffer[i].param[j][0] != '\0'; j++) // j starts 1 one because param[0] has "MACRO"
                    strcpy(PT.dummy[j], buffer[i].param[j]);// get dummy parameter
                break;
            }
        }

        // set actual parameters
        for(int i = 1; field[i][0] != '\0'; i++)
            strcpy(PT.actual[i], field[i]);

    }

    // if macro starts with an if-statement
    // don't check if the condition is true, is_macro() did already
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
        for(int i = 4; field[i][0] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++)
            if(strcmp(buffer[i].mname, PT.mname) == 0) {
                for(int j = 1; buffer[i].param[j][0] != '\0'; j++) {
                    // get dummy parameter
                    strcpy(PT.dummy[j], buffer[i].param[j]); // get dummy parameter
                }
                break;
            }

        // set actual parameters
        // e.g. (if $2 25 ADD 25 10 20) should be (ADD 25 10 20), ditching the first 3 fields
        // ditch macro name too, to access the rest of the parameters
        for(int i = 1; field[i+3][0] != '\0'; i++)
            strcpy(PT.actual[i], field[i+3]);
    }
}