/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This is the header file for utility.c
 * It declares prototypes of functions.
 *
 */


#ifndef ASSEMBLER_UTILITY_H
#define ASSEMBLER_UTILITY_H

char * add_extension(char *filename, char* extension);

char *next_token(char *seq);

char *copy_token(char *src, char *dest);

void write_error(int line_number);

int end_of_line(char *line);

char *skip_spaces(char * ptr);

int ignore_line (char * line);

unsigned int insert_field(unsigned int word, int field);

#endif /*ASSEMBLER_UTILITY_H*/
