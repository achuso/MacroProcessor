#include <stdio.h>
#include <string.h>

void expand(); 

void is_macro(const char* line){

    // check if the line starts with an #if

    if(strncmp(line, "#if", 3) == 0){
    
    // check for conditions
    // format: #if $2 ABC M3 200 300 
    // $2 is a condition parameter that is given as an argument such that arg[2]
    // get the number after the $, pass it into arg[], and check if its equal to the thing inside the paranthesis    
    
    }
    // check if the line starts with a macro name
    else if (strncmp(line, "ADD", 3) == 0 || strncmp(line, "SUB", 3) == 0 || strncmp(line, "MULT", 4) == 0 || strncmp(line, "DIV", 3) == 0 || strncmp(line, "REM", 3) == 0){ 

            expand(); // expands the macro        

    }
    else{
        //write the line into the .asm file
        FILE* f1 = fopen("f1.asm", "a"); // creates/opens the f1.asm files and "a" apends data into the file
        fprintf(f1, "%s\n", line); // writes the contents of the line into f1
    }

    
}