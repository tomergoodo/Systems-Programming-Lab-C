//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

void process_file(FILE* fp);
void process_line(char* instruction);
void handle_operation(char* operation);
void handle_operation(char* directive);
void handle_label(char* label);
int is_error();

#endif //ASSEMBLER_FIRST_PASS_H
