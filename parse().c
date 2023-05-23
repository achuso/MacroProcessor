#include <stdio.h>
#include <string.h>
//max 10 fields in a line, each field max 6 characters
char field[10][7];
void parse(char* filename) {
    int max_field_index = 0;
    char* myfield;
    myfield = strtok(filename, " ");
    while(myfield != NULL && max_field_index < 10){
        strncpy(field[max_field_index], myfield, 6);
        field[max_field_index][6] = '\0';
        max_field_index++;
        myfield = strtok(NULL," ");
    }
}

