/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This is the header file for second_pass.c
 * It declares prototypes of functions and external variables.
 */

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#include <stdio.h>
#include "label_table.h"
#include "dictionaries.h"

/**------------------------------------------------external variables-------------------------------------------------*/

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


/**----------------------------------------------------prototypes-----------------------------------------------------*/
/**file and line handling*/
void second_pass(FILE *fp, char *filename);
void process_line_second_pass(char *instruction);

/**entry directive handling*/
void handle_entry(char * label);

/**encoding handling*/
void handle_encoding(char *instruction, operations type);
unsigned int encode_word(char *operand, int dest);
unsigned int encode_word_registers(char *reg1, char *reg2, fields field);
int find_register(char *reg);
unsigned int encode_label(char *label);

/**output files handling*/
void build_output_files(char *filename);
void build_object_file(char *filename);
void build_extern_file(char *filename);
void build_entry_file(char *filename);

#endif /*ASSEMBLER_SECOND_PASS_H*/
