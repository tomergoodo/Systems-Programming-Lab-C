/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This file is the implementation of the different utility functions.
 *
 */



#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "info.h"
#include "dictionaries.h"

extern int error;

/**This function appends an extension to a string.
 * This is used to append extension to file names.
 * @param filename: the string to append to (the file name).
 * @param extension: the string to append (the extension).
 * @return: a pointer to the appended string.*/
char * add_extension(char *filename, char* extension){
    char * extended_filename = (char*)malloc(strlen(filename)+strlen(extension));
    strcpy(extended_filename,filename);
    return strncat(extended_filename,extension,strlen(extension));
}

/**This function finds the next token of the line.
 * @param seq: the current pointer.
 * @return: a pointer to the next non-whitespace token. Returns NULL if nedt token is end of line.*/
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

/**This function copies the current token(until a whitespace or end of line is reached).
 * @param src: the start pointer, the source pointer.
 * @param dest: the detination pointer.
 * @return: dest.*/
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

/**This function writes errors to stderr.
 * The error type is determined by the error global variable.
 * @param line_number: the line number of the error.*/
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
            fprintf(stderr, "Label can not be a name of an instruction, directive or a register.\n");
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
        case ILLEGAL_COMMA_DATA:
            fprintf(stderr, "Illegal comma.\n");
            break;
        case DATA_SYNTAX_ERROR:
            fprintf(stderr, "Data variables must be integers.\n");
            break;
        case DATA_INT_OVERFLOW:
            fprintf(stderr, "Data variables must be between -2^15 and 2^15.\n");
            break;
        case MISSING_COMMA_DATA:
            fprintf(stderr, "Data variables must be separated with commas.\n");
            break;
        case STRING_SYNTAX_ERROR:
            fprintf(stderr, "Strings must start and end with %c.\n",'"');
            break;
        case NUMBER_OF_STRINGS_ERROR:
            fprintf(stderr, "String directive accepts only one parameter.\n");
            break;
        case MISSING_COMMA_OPERATION:
            fprintf(stderr, "Operands must be separated with commas.\n");
            break;
        case ADDRESS_METHOD_ERROR:
            fprintf(stderr, "Address method isn't compatible with operation.\n");
            break;
        case NUMBER_OF_OPERANDS_ERROR:
            fprintf(stderr, "Incorrect number of operands.\n");
            break;
        case IMMEDIATE_INT_OVERFLOW:
            fprintf(stderr, "Immediate variables must be between -2^12 and 2^12.\n");
            break;
        case METHOD_UNKNOWN_ERROR:
            fprintf(stderr, "Unrecognized addressing method.\n");
            break;
        case ENTRY_NOT_FOUND:
            fprintf(stderr, "Entry-declared label was not found in file.\n");
            break;
        case LABEL_NOT_FOUND:
            fprintf(stderr, "Label was not declared in file.\n");
            break;
    }
}

/**This function checks if end of line is reached.
 * @param line: the char pointer to check.
 * @return: TRUE if reached, FALSE otherwise.*/
int end_of_line(char *line)
{
    return line == NULL || *line == '\0' || *line == '\n';
}

/**This function skips whitespaces.
 * @param ptr: the char pointer to move.
 * @return: the pointer after skipping whitespaces.*/
char * skip_spaces(char *ptr){
    if(ptr == NULL) return ptr;
    while(isspace(*ptr))
        ptr++;
    return ptr;
}

/**This function checks if the assembler should ignore the line.
 * Should ignore if comment line (starts with ;) or blank line.
 * @param line: the pointer to the start of a line.
 * @return: TRUE if should be ignored, FALSE otherwise.*/
int ignore_line(char *line){
    line = skip_spaces(line);
    return *line == ';' || *line == '\n' || *line == '\0';
}

/**This function inserts the field (ABSOLUTE, RELOCATABLE OR EXTERNAL) to an encoded word.
 * @param word: the word to be modified (insert the field to).
 * @param field: the field to insert (type enum fields converted to int).
 * @return: the modified word.*/
unsigned int insert_field(unsigned int word, int field){
    word <<= FIELD_BITS;
    word |= field;
    return word;
}
