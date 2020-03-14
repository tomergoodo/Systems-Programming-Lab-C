//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#define MAX_LINE 120
#define REGISTER_BITS 3

extern int ic;
extern int dc;
extern int error;
extern int error_flag;
extern unsigned int data[];
extern unsigned int code[];



void second_pass(FILE *fp, char *filename);

void process_line_second_pass(char *instruction);

void handle_encoding(char *instruction, operations type);

unsigned int encode_word(char *operand);

unsigned int encode_word_registers(char *reg1, char *reg2, fields field);

int find_register(char * reg);

unsigned int encode_label(char *label);

void build_output_files(char *filename);

#endif //ASSEMBLER_SECOND_PASS_H
