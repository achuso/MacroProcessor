#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct macro {
    char mname[8];
    char param[10][4];
    char macro[256];
};

struct macro buffer[10];
int m_count;
const char punctuation[] = " ,:";

int read(char* filename) {
    m_count = 0;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File couldn't be opened.\n");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) ) {
        if (strncmp(line, "#ENDM", 5) == 0) {
            m_count++;
        } else if (line[0] == '#' && strstr(line, ":") ) {
            char* p;
            char* lineP = line;

            p = strtok(lineP, punctuation);
            if (p) {
                strcpy(buffer[m_count].mname, p + 1);
            }
            int param_count = 0;
            while (p = strtok(NULL, punctuation)) {
                strncpy(buffer[m_count].param[param_count], p, 4);
                param_count++;
            }
        } else {
            if (line[strlen(line) - 1] != '\0') {
                strcat(line, "\n");
            }
            strcat(buffer[m_count].macro, line);
        }
    }

    fclose(file);
    return m_count;
}

int main() {
   int sayı= read("inputfile.txt");
   printf("%d\n",sayı);
    printf("m_count: %d\n", m_count);

    for (int i = 0; i < m_count; i++) {
        printf("Macro name: %s\n", buffer[i].mname);
        printf("Parameters: ");
        for (int j = 0; j < 10; j++) {
            if (strlen(buffer[i].param[j]) > 0) {
                printf("%s ", buffer[i].param[j]);
            }
        }
        printf("\n");
        printf("Macro body:\n%s\n", buffer[i].macro);
    }

    return 0;
}
