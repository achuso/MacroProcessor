#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void parse(char* filename, char field[10][7]) {
    int max_field_index = 0;
    char* myfield;
    bool firstField = true;

    myfield = strtok(filename, " ");

    while (myfield != NULL && max_field_index < 10) {
        int length = strlen(myfield);
        int j = 0;
        
        if(firstField && myfield[0] == '#'){
            for(int i = 0; i < length; i++){
                myfield[i] = myfield[i+1];
            }
        }
        firstField = false;
        
        if (strcmp(myfield, "MACRO") == 0) {
            myfield = strtok(NULL, " ");
        }
        
        for (int i = 0; i < length; i++) {
            if (myfield[i] != ':' && myfield[i] != ',') {
                field[max_field_index][j] = myfield[i];
                j++;
            }
        }
        field[max_field_index][j] = '\0';
        max_field_index++;

        myfield = strtok(NULL, " ");
    }

}

