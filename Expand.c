#include <stdio.h>

struct mac {
    char mname[8];
    char param[10][4];
    char macro[256];
};

struct pt {
    char mname[8];
    int nparams;
    char dummy[10][4];
    char actual[10][4];
};

void expand(char field[10][7], struct mac buffer[10], int m_count){
    struct pt PT;
    createPT(field, buffer, m_count);

    // Take a line from the macro body and write it to the expanded code
    // If there are dummy parameters in that line, substitute them
    // with their actual parameters using the parameter table
    // Assuming 'line' represents the line from the macro body
    char expandedLine[256]; // Buffer to store the expanded line
    int i = 0; // Index for the expandedLine buffer
    
    // Iterate over each character in the line
    for (int j = 0; line[j] != '\0'; j++) {
        // Check if the current character is a dummy parameter
        if (line[j] == '$') {
            // get the parameter index from the line
            int paramIndex = j + 1;
            
            // Substitute the dummy parameter with the actual parameter
            strcpy(&expandedLine[i], PT.actual[paramIndex]);
            
            // Update the index i and skip the remaining characters of the dummy parameter
            i += strlen(PT.actual[paramIndex]);
            j += 2; // Skip the '$' and the digit
        } else {
            // Copy the current character to the expanded line
            expandedLine[i++] = line[j];
        }
    }
    
    // Null-terminate the expanded line
    expandedLine[i] = '\0';

    // havent yet figured out if the expanded line should be returned for the is_macro() to print or should 
    // this function print
}
