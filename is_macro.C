#include <stdio.h>
#include <string.h>

void expand(); 

void is_macro(const char* line){

    // check if the line starts with a macro name or an #if

    if(strncmp(line, "#if", 3) == 0 || strncmp(line, "ADD", 3) == 0 || strncmp(line, "SUB", 3) == 0 || strncmp(line, "MULT", 4) == 0 || strncmp(line, "DIV", 3) == 0 || strncmp(line, "REM", 3) == 0){

        expand(); // add the macro name inside 

    }
    else{
        //write the line into the .asm file
        FILE* f1 = fopen("f1.asm", "a"); // creates/opens the f1.asm files and "a" apends data into the file
        fprintf(f1, "%s\n", line); // writes the contents of the line into f1
    }

    
}