/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This is the header file for first_pass.c
 * It declares prototypes of functions and external variables.
 *
 */

#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include <stdio.h>
#include "dictionaries.h"
#include "label_table.h"

/**------------------------------------------------external variables-------------------------------------------------*/

extern int error;
extern int error_flag;
extern int extern_flag;
extern int entry_flag;
extern int ic;
extern int dc;
extern unsigned int data[];
extern unsigned int code[];
extern label_table *extern_table_head;
extern label_table *table_head;


/**----------------------------------------------------prototypes-----------------------------------------------------*/
/**file and line handling*/
void first_pass(FILE* fp);
void process_line(char* instruction);

/**operation handling*/
void handle_operation(char* operation);
int check_number_validity(char * num);
int number_of_operands(operations type);
int operand_valid_method(operations type, methods source_method, methods dest_method);
int calculate_additional_words(operations type, methods src_method, methods dest_method);
unsigned int encode_first_word(int type, int src, int dest, int src_method, int dest_method);

/**directive handling*/
void handle_directive(char* directive);
void handle_data(char * data);
void handle_string(char * string);
void handle_extern(char * label);
void write_to_data(int num);

/**label handling*/
void handle_label(char* label);
int is_label(char * token, int colon);

/**error handling*/
int overflow();
int is_error();

#endif /*ASSEMBLER_FIRST_PASS_H*/
