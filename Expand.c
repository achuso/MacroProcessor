#include <stdio.h>
#include <string.h>

struct mac {
    char mname[8];
    char param[10][4];
    char macro[256];
};

void expand(FILE* file){
    createPT();

    // find the correct macro in the buffer
    for(int i = 0; i < m_count; i++){
        if(strcmp(buffer[i].mname, PT.mname) == 0){
            char* p = buffer[i].macro;

            while(*p != '\0'){
                // if it encounters a '$' sign, this indicates a dummy parameter
                if(*p == '$'){
                    // print the $
                    fprintf(file, "%s", "$");
                    // skip it
                    p++;

                    // to consider multiple characters, i am creating a temporary buffer
                    char temp_param[5] = "";  // maximum of 5 characters for parameter names, like "A1"
                    for(int k = 0; k < 5 && *p != ' ' && *p != '\n' && *p != '\0'; k++, p++)
                        temp_param[k] = *p;
                    if(temp_param[2] == '1' ||  temp_param[2] == '2'){
                        temp_param[3] = '\0';
                    }

                    // matching the dummy parameters
                    for(int j = 1; j < PT.nparams + 1; j++){
                        if(strcmp(temp_param, PT.dummy[j]) == 0){
                            // print the corresponding actual parameter to the output file
                            fprintf(file, "%s", PT.actual[j]);
                        }

                    }

                }
                else {
                    // print the character as is
                    fprintf(file, "%c", *p);
                    p++;
                }
            }

            fprintf(file, "\n");
        }
    }
}
