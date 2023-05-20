#include <stdio.h>
#include <string.h>

void expand(); 

void is_macro(const char field[10][7], char* filename){  

    // check if the line starts with an #if

    if(strncmp(field[0], "if", 2)){ 
    
    // check for conditions
    // format: #if $2 ABC M3 200 300 
    // $2 is a condition parameter that is given as an argument such that arg[2]
    // get the number after the $, pass it into arg[], and check if its equal to the thing inside the paranthesis    
    
    }
    // check if the line starts with a macro name
    else if (strncmp(field[0], "ADD", 3) == 0 || strncmp(field[0], "SUB", 3) == 0 || strncmp(field[0], "MULT", 4) == 0 || strncmp(field[0], "DIV", 3) == 0 || strncmp(field[0], "REM", 3) == 0){ 

            expand(); // expands the macro        

    }
    else{
        //write the line into the .asm file
        FILE* f1 = fopen(filename, "a"); // creates/opens the filename and "a" apends data into the file
        
        for(int i = 0; field[i][0] != NULL; i++){

            fprintf(f1, "%s ", field[i]); // writes the contents of the line into f1

        }

        fprintf(f1, "\n");
    }

    
}