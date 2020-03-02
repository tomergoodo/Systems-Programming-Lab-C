//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <ctype.h>
#include "utility.h"

void write_error(int line_number){
    fprintf(stderr, "ERROR (line %d): ", line_num);
    switch(error){
        case SYNTAX_ERR:
            fprintf(stderr, "first non-blank character must be a letter or a dot.\n");
            break;
        //case :
            /*....*/
    }
}
int end_of_line(char *line)
{
    return line == NULL || *line == '\0' || *line == '\n';
}


char * skip_spaces(char * ptr){
    if(ptr == NULL) return ptr;
    while(isspace(*ptr))
        ptr++;
    return ptr;
}

int ignore_line(char * line){
    line = skip_spaces(line);
    return *line == ';' || *line == '\n' || *line == '\0';
}
