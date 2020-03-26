/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This file implements the second-pass functions.
 *
 */

#include "second_pass.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "info.h"
#include "first_pass.h"
#include "utility.h"

/** This function reads a line from the file and manages all the second pass processing.
 * @param fp: FILE pointer to the file to be processed.*/
void second_pass(FILE * fp, char *filename){
    char instruction[MAX_LINE] = "";
    int line_number = 1;

    ic = 0;

    while(fgets(instruction, MAX_LINE, fp) != NULL){
        error = NO_ERR;
        if(!ignore_line(instruction))
            process_line_second_pass(instruction);
        if(is_error())
            write_error(line_number);
        line_number++;
    }

    if(!error_flag)
        build_output_files(filename);
}

/** This function processes a line of the file, identifies its content and sends to sub processing functions.
 * @param instruction: char pointer to the start of the line.**/
void process_line_second_pass(char * instruction){
    char token[MAX_LINE] = "";
    instruction = skip_spaces(instruction);
    if(end_of_line(instruction)) return;
    copy_token(instruction, token);

    if(is_label(token, TRUE)){
        error = NO_ERR;
        instruction = next_token(instruction);
        copy_token(instruction, token);
    }
    if(find_directive(token) != UNKNOWN_DIRECTIVE){
        if(find_directive(token) == ENTRY){
            instruction = next_token(instruction);
            handle_entry(copy_token(instruction, token));
        } else
            return;
    }
    if(is_error())
        return;
    if(find_operation(token) != UNKNOWN_COMMAND){
        handle_encoding(instruction, find_operation(token));
    }
    if(is_error())
        return;
}

/** This function handles the .entry directive.
 * Sets the label's entry field to TRUE.
 * @param label: the name of the entry-declared label.*/
void handle_entry(char * label){
    set_entry(label);
}

/** This function handles the encoding of code words and writing them into the appropriate cell in the code array.
 * @param instruction: char pointer to the start of an operation line (past any labels).
 * @param type: is the operation type (enum operations)*/
void handle_encoding(char *instruction, operations type){
    char first_op[20] = "", second_op[20] = "";
    switch(number_of_operands(type)){
        case 0:
            ic++;
            return;
        case 1:
            copy_token(next_token(instruction),first_op);
            code[++ic] = encode_word(first_op, TRUE);
            ic++;
            return;
        case 2:
            copy_token(next_token(instruction),first_op);
            copy_token(next_token(next_token(next_token(instruction))),second_op);
            if((find_method(first_op) == METHOD_REGISTER || find_method(first_op) == METHOD_INDIRECT_REGISTER)
                && (find_method(second_op) == METHOD_REGISTER || find_method(second_op) == METHOD_INDIRECT_REGISTER)){
                code[++ic] = encode_word_registers(first_op, second_op,ABSOLUTE);
            }else{
                code[++ic] = encode_word(first_op, FALSE);
                code[++ic] = encode_word(second_op, TRUE);
            }
            ic++;
            return;
    }
}

/**This function handles word encoding.
 * The immediate operand words(represented by #) are encoded in this function
 * while other methods are send to be encoded in sub functions.
 * (First word was already encoded in the first pass).
 * @param operand: the operand to be encoded.
 * @param dest: a boolean to indicate if the operand is a destination operand (FALSE if source operand).
 * @return: the encoded word (unsigned int).*/
unsigned int encode_word(char *operand, int dest){
    unsigned int word =0;
    switch (find_method(operand)){
        case METHOD_IMMEDIATE:
            ++operand;
            if(atoi(operand)>=IMMEDIATE_MIN&&atoi(operand)<=IMMEDIATE_MAX){
                word |= atoi(operand);
                word = insert_field(word, ABSOLUTE);
                return word;
            }
            error = IMMEDIATE_INT_OVERFLOW;
            return word;
        case METHOD_DIRECT:
            if(find_label(operand) == NULL){
                error = LABEL_NOT_FOUND;
                return 0;
            }
            return encode_label(operand);
        case METHOD_REGISTER:
            if(dest)
                return encode_word_registers(NULL, operand, ABSOLUTE);
            else
                return encode_word_registers(operand, NULL, ABSOLUTE);
        case METHOD_INDIRECT_REGISTER:
            if(dest)
                return encode_word_registers(NULL, operand, ABSOLUTE);
            else
                return encode_word_registers(operand, NULL, ABSOLUTE);
        case METHOD_UNKNOWN:
        case NONE:
            error = METHOD_UNKNOWN_ERROR;
            return word;
    }
    return word;
}

/**This function encodes the register operand words (indirect register or register methods).
 * @param reg1: is a char pointer to the first(operand) register.
 * @param reg2: is a char pointer to the second(operand) register (if there isn't one than NULL is accepted).
 * @return: the encoded word (unsigned int).*/
unsigned int encode_word_registers(char *reg1, char *reg2, fields field){
    unsigned int word = 0;
    word |= find_register(reg1);
    word <<= REGISTER_BITS;
    word |= find_register(reg2);
    word = insert_field(word, field);
    return word;
}

/**This function finds the number of the desired register (r0-r7).
 * @param reg: is a char pointer to the register.
 * @return: the number of the register(0-7)*/
int find_register(char *reg){
    if(reg == NULL)
        return 0;
    if(isdigit(*++reg))
        return atoi(reg);
    else
        return atoi(++reg);
}

/**This function encodes direct operand words (labels).
 * @param label: the label name.
 * @return: the encoded word (unsigned int).*/
unsigned int encode_label(char *label){
    unsigned int word = 0;
    int address;
    address = get_address(find_label(label));
    word |= address;
    if(get_extern(find_label(label))){
        add_label(&extern_table_head, label, ic+STARTING_OFFSET, TRUE);
        word = insert_field(word, EXTERNAL);
    }
    else
        word = insert_field(word, RELOCATABLE);
    return word;
}

/**This function handles writing the output .ob, .ext and .ent files.
 * @param filename: is the base filename (without the extension) of the original .as file.*/
void build_output_files(char *filename){
    build_object_file(filename);
    if(extern_flag)
        build_extern_file(filename);
    if(entry_flag)
        build_entry_file(filename);
}

/**This function writes the object file using the code and data arrays.
 * @param filename: is the base filename (without the extension) of the original .as file.*/
void build_object_file(char *filename){
    int line=STARTING_OFFSET;
    int count=0;
    char tmp[MAX_LINE];
    char *p;
    FILE *fp;
    filename = add_extension(filename,".ob");
    fp = fopen(filename, "w");
    fprintf(fp, "   %d %d\n",ic,dc);
    while(ic--){
        p=tmp;
        sprintf(tmp, "%05ho", code[count++]); /*convert to octal base and store in tmp*/
        p += strlen(tmp)-CODE_SIZE; /*point to the last 5 characters.*/
        fprintf(fp,"%04d  %.5s\n",line++,p);
    }
    count=0;
    while(dc--){
        p=tmp;
        sprintf(tmp,"%05ho", data[count++]); /*convert to octal base and store in tmp*/
        p += strlen(tmp)-CODE_SIZE; /*point to the last 5 characters.*/
        fprintf(fp, "%04d  %.5s\n",line++,p);
    }
    printf("%s was created.\n",filename);
    free(filename);
    fclose(fp);
}

/**This function writes the .ext output file.
 * This is done using the linked list of the external labels (extern_table_head) declaration and usage in the .as file.
 * @param filename: is the base filename (without the extension) of the original .as file.*/
void build_extern_file(char *filename){
    FILE *fp;
    label_table *p = extern_table_head;
    filename = add_extension(filename,".ext");
    fp = fopen(filename, "w");
    while(p != NULL){
        fprintf(fp,"%s\t%05d\n",p->label,p->address);
        p = p->next;
    }
    printf("%s was created.\n",filename);
    free(filename);
    fclose(fp);
}

/**This file writes the .ent output file.
 * This is done using the table_head linked list and the entry boolean field in each node.
 * @param filename: is the base filename (without the extension) of the original .as file.*/
void build_entry_file(char *filename){
    label_table *p = table_head;
    FILE *fp;
    filename = add_extension(filename,".ent");
    fp = fopen(filename, "w");
    while(p!=NULL){
        if(p->entry)
            fprintf(fp,"%s\t%05d\n",p->label,p->address);
        p = p->next;
    }
    printf("%s was created.\n",filename);
    free(filename);
    fclose(fp);
}