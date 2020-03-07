//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <ctype.h>
#include "utility.h"

char * add_extension(char *filename){
    char * extended_filename = (char*)malloc(strlen(filename)+3);
    char * extension = ".as"
    extended_filename = filename;
    return strncat(extended_filename,extension,3);
}

char * next_token(char *seq){
    if(seq == NULL) return NULL;
    while(!isspace(*seq) && !end_of_line(seq))
        seq++;
    seq = skip_spaces(seq);
    if(end_of_line(seq)) return NULL;
    return seq;
}

char * copy_token(char *src, char *dest){
    int i=0;
    if(src == NULL || dest == NULL) return dest;
    while(i<MAX_LINE && !isspace(src[i]) && src[i] != '\0'){
        dest[i]=src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

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


char * skip_spaces(char *ptr){
    if(ptr == NULL) return ptr;
    while(isspace(*ptr))
        ptr++;
    return ptr;
}

int ignore_line(char *line){
    line = skip_spaces(line);
    return *line == ';' || *line == '\n' || *line == '\0';
}
