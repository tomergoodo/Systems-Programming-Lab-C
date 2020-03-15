//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_UTILITY_H
#define ASSEMBLER_UTILITY_H
#define MAX_LINE 120
#define MAX_LABEL_LENGTH_FULL 32
#define FIELD_BITS 3

char * add_extension(char *filename, char* extension, int length);

char *next_token(char *seq);

char *copy_token(char *src, char *dest);

void write_error(int line_number);

int end_of_line(char *line);

char *skip_spaces(char * ptr);

int ignore_line (char * line);

unsigned int insert_field(unsigned int word, int field);

unsigned int dec_to_octal(unsigned int dec);
#endif //ASSEMBLER_UTILITY_H
