
int read(char* filename) {
    const char punctuation[] = " ,:";
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
            param_count = 0;
            while ((p = strtok(NULL, punctuation))) {
                strncpy(buffer[m_count].param[param_count], p, 4);// param includes "MACRO" so you should start 1 instead of 0 when you using param array
                param_count++;
            }
            // Counteracts the exception where the last parameter's last character has a '\n' mixed in
            for(int i = 0; i < 4; i++)
                if(buffer[m_count].param[param_count-1][i] == '\n') {
                    buffer[m_count].param[param_count-1][i] = '\0';
                    break;
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
