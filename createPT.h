#include <string.h>

struct pt {
    char mname[8];          // macro name
    int  nparams = 0;       // number of parameters
    char dummy[10][4];      // dummy parameters
    char actual[10][4];     // actual parameters
};
struct pt PT;
static const struct pt emptyPT;

struct mac {
    char mname[8];
    char param[10][4];
    char macro[256];
};

// field is the array returned from parse()
// array of macro definitions returned from read()
// m_count, macro def count returned from read()
void createPT(char field[10][7], struct mac buffer[10], int m_count) { 
    // Initialize/set pt to zero
    PT = emptyPT;

    // handle a regular macro call (i.e. "M1 479 61")
    if(field[0][0] != '#') {

        // set macro name
        strcopy(PT.mname, field[0]);

        // get number of parameters
        for(int i = 1; field[i][0] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++)
            // !!!!! CHECK ON WITH HOW AYSEGUL STORES MACRO NAMES TO BUFFER, IF SHE'S STORING THE # AT THE BEGINNING, EDIT BOTTON LINE ACCORDINGLY
            if(strcmp(buffer[i].mname, field[0], 7) == 0) {
                for(int j = 0; buffer[i].param[j][0] != '\0') {
                    // get dummy parameter
                    PT.dummy[j] = buffer[i].param[j];
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
        strcopy(PT.mname, field[3]);

        // get number of parameters
        for(int i = 1; field[i][4] != '\0'; i++, PT.nparams++);

        // to get dummy parameters, find the associated macro def.
        for(int i = 0; i < m_count; i++)
            // !!!!! CHECK ON WITH HOW AYSEGUL STORES MACRO NAMES TO BUFFER, IF SHE'S STORING THE # AT THE BEGINNING, EDIT BOTTON LINE ACCORDINGLY
            if(strcmp(buffer[i].mname, field[3], 7) == 0) {
                for(int j = 0; buffer[i].param[j][0] != '\0') {
                    // get dummy parameter
                    PT.dummy[j] = buffer[i].param[j];
                }
                break;
            }

        // set actual parameters
        for(int i = 4; field[i][0] != '\0'; i++)
            strcpy(PT.actual[i-4], field[i]);
    }

    return;
}