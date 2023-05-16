#include <stdio.h>
#include <string.h>


void is_macro(const char* line){

    // check if the line starts with a macro name or an #if

    if(strncmp(line, "#if", 3 ) || ){

        //expand(macro_name);

    }
    else{
        //write the line into the .asm file
    }

    
}