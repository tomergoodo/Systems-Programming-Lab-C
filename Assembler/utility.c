//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utility.h"
#include "dictionaries.h"

extern int error;

char * add_extension(char *filename){
    char * extended_filename = (char*)malloc(strlen(filename)+3);
    char * extension = ".as";
    strcpy(extended_filename,filename);
    return strncat(extended_filename,extension,3);
}

char * next_token(char *seq){
    if(seq == NULL) return NULL;
    if(*seq == ','){
        seq++;
        seq = skip_spaces(seq);
        if(end_of_line(seq)) return NULL;
        return seq;
    }
    while(!isspace(*seq) && !end_of_line(seq) && *seq != ',')
        seq++;
    seq = skip_spaces(seq);
    if(end_of_line(seq)) return NULL;
    return seq;
}

char * copy_token(char *src, char *dest){
    int i=0;
    if(src == NULL || dest == NULL) return dest;
    while(i<MAX_LINE && !isspace(src[i]) && src[i] != '\0' && src[i] != ','){
        dest[i] = src[i];
        i++;
    }
    if(src[0] == ',') {
        dest[0] = src[0];
        dest[1] =  '\0';
        return dest;
    }

    dest[i] = '\0';
    return dest;
}

void write_error(int line_number){
    fprintf(stderr, "ERROR (line %d): \n", line_number);
    switch(error){
        case SYNTAX_ERR:
            fprintf(stderr, "First non-blank character must be a letter or a dot.\n");
            break;
        case LABEL_SYNTAX:
            fprintf(stderr, "Label must start with an alphabetic character and continue with alphanumeric characters.\n");
            break;
        case LABEL_LENGTH:
            fprintf(stderr, "Label must be max %d characters long.\n", MAX_LABEL_LENGTH_FULL);
            break;
        case EMPTY_LABEL_LINE:
            fprintf(stderr, "A label can not be followed by an empty line.\n");
            break;
        case LABEL_DOUBLE_DEFINITION:
            fprintf(stderr, "Label is defined more than once.\n");
            break;
        case LABEL_CONFLICTING_NAME:
            fprintf(stderr, "Label can not be a name of instruction or register.\n");
            break;
        case LABEL_SYNTAX_COLON:
            fprintf(stderr, "Label definition must end with a colon.\n");
            break;
        case COMMAND_NOT_FOUND:
            fprintf(stderr,"Command not found.\n");
            break;
        case DIRECTIVE_NOT_FOUND:
            fprintf(stderr,"Directive not found.\n");
            break;
        case DATA_SYNTAX_ERROR:
            fprintf(stderr, "Data variables must be integers.\n");
            break;
        case MISSING_COMMA_DATA:
            fprintf(stderr, "Data variables must be separated with commas.\n");
            break;
        case STRING_SYNTAX_ERROR:
            fprintf(stderr, "Strings must start and end with "".\n");
            break;
        case MISSING_COMMA_OPERATION:
            fprintf(stderr, "operands must be separated with commas.\n");
            break;
        case ADDRESS_METHOD_ERROR:
            fprintf(stderr, "Address method isn't compatible with operation.\n");
            break;
        case NUMBER_OF_OPERANDS_ERROR:
            fprintf(stderr, "Unexpected number of operands.\n");
            break;
        case METHOD_UNKNOWN_ERROR:
            fprintf(stderr, "Unrecognized addressing method.\n");
            break;

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
