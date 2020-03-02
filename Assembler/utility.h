//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_UTILITY_H
#define ASSEMBLER_UTILITY_H

void write_error(int line_number);

int end_of_line(char *line);

char * skip_spaces(char * ptr);

int ignore_line (char * line);

#endif //ASSEMBLER_UTILITY_H
