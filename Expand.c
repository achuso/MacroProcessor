#include <stdio.h>
#include <string.h>

struct mac {
    char mname[8];
    char param[10][4];
    char macro[256];
};

void expand(){
    createPT(field, buffer, m_count);  
    // take a line from the macro body
    // check if there are dummy parameters
        //if there are, substitue them with the actual parameters using the parameter table
    // and append it to f1.asm
    
    // !!! MAY MODIFY THIS TO PASS THE FILE f1 AS PARAMETER INSTEAD, CHECK WITH MAHNY
    FILE* f1 = fopen("f1.asm", "a"); // opens the file f1

    if (file == NULL) {
        printf("Failed to open .asm file.\n");
        return;
    } 
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
        fprintf(f1, "%s\n", line);  
        line = strtok(NULL, "\n"); //  gets the next line 
    }
    fclose(f1);
}