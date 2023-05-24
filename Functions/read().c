#include <stdio.h>
#include <string.h>

struct mac {
    char mname[8]; //macro name
    char param[10][4]; // max. 10 paramaters, each parameter max. 3 characters
    char macro[256]; // macro body
};

struct mac buffer[10]; // memory buffer for 10 macro definitions
int m_count; // counts the number of macro definitions
int param_count;

int read(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("file couldn't open.\n");
        return 0;
    }

    char line[256];
    m_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "#", 1) == 0) //if line start #MNAME and its compare 5 characters
        {
            // Read macro definition
            sscanf(line, "#%s", buffer[m_count].mname);//MNAME add buffer
            fgets(line, sizeof(line), file);
            param_count = 0;
            while (strncmp(line, "#ENDM", 5) != 0) {
                sscanf(line, "%s", buffer[m_count].param[param_count]);//buffer have mname param andmacro
                fgets(line, sizeof(line), file);
                param_count++;
            }
            fgets(line, sizeof(line), file);
            strcpy(buffer[m_count].macro, line); //line buffer.macro içine kopyalandı
            m_count++;
        }
    }

    fclose(file);
    return m_count;
}
