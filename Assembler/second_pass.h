//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#include "label_table.h"


extern int ic;
extern int dc;
extern int error;
extern int error_flag;
extern int extern_flag;
extern int entry_flag;
extern unsigned int data[];
extern unsigned int code[];
extern label_table *extern_table_head;
extern label_table *table_head;

void second_pass(FILE *fp, char *filename);
void process_line_second_pass(char *instruction);
void handle_entry(char * label);

void handle_encoding(char *instruction, operations type);
unsigned int encode_word(char *operand, int dest);
unsigned int encode_word_registers(char *reg1, char *reg2, fields field);
int find_register(char *reg);
unsigned int encode_label(char *label);

void build_output_files(char *filename);
void build_object_file(char *filename);
void build_extern_file(char *filename);
void build_entry_file(char *filename);

#endif //ASSEMBLER_SECOND_PASS_H
