#include <stdio.h>
#include <string.h>
#include <stdlib.h>  

void expand(); 

void is_macro(const char field[10][7], FILE* outputFilename, char** argv){  

    // check if the line starts with an if

    if(strncmp(field[0], "if", 2) == 0){


    // check for conditions:
    // example: field format: if $2 ABC M3 200 300 
    // $2 is a condition parameter that is given as an argument such that arg[2]
    // get the number after the $, pass it into arg[], and check if its equal to the thing inside the paranthesis
        
        int argVal = atoi(&field[1][1]);  // to extract the number after the $ and store it as an int
        // OR we can do this field[1][1] - '0';

        if (strcmp(argv[argVal], field[2]) == 0){

            expand();
            
        }
        else{
            return;
        }
    
    }
    // check if the line starts with a macro name
    else if (strncmp(field[0], "ADD", 3) == 0 || strncmp(field[0], "SUB", 3) == 0 || strncmp(field[0], "MULT", 4) == 0 || strncmp(field[0], "DIV", 3) == 0 || strncmp(field[0], "REM", 3) == 0){ 

            expand();        

    }
    else{
        //write the line into the .asm file
        //FILE* f1 = fopen(outputFilename, "a"); // creates/opens the filename and "a" apends data into the file
        
        for(int i = 0; field[i][0] != '\0'; i++){

            fprintf(outputFilename, "%s ", field[i]); // writes the contents of the line into f1

        }

        fprintf(outputFilename, "\n");
        //fclose(f1);
    }
    
}