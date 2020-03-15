//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include "first_pass.h"
#include "dictionaries.h"
#include "utility.h"
#include "second_pass.h"

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

void process_line_second_pass(char * instruction){
    char token[MAX_LINE] = "";
    instruction = skip_spaces(instruction);
    if(end_of_line(instruction)) return;
    copy_token(instruction, token);

    if(is_label(token, TRUE)){
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

void handle_entry(char * label){
    set_entry(label);
}

void handle_encoding(char *instruction, operations type){
    char first_op[20] = "", second_op[20] = "";
    switch(number_of_operands(type)){
        case 0:
            ic++;
            return;
        case 1:
            copy_token(next_token(instruction),first_op);
            code[++ic] = encode_word(first_op);
            ic++;
            return;
        case 2:
            copy_token(next_token(instruction),first_op);
            copy_token(next_token(next_token(next_token(instruction))),second_op);
            if((find_method(first_op) == METHOD_REGISTER || find_method(first_op) == METHOD_INDIRECT_REGISTER)
                && (find_method(second_op) == METHOD_REGISTER || find_method(second_op) == METHOD_INDIRECT_REGISTER)){
                code[++ic] = encode_word_registers(second_op, first_op,ABSOLUTE);
            }else{
                code[++ic] = encode_word(first_op);
                code[++ic] = encode_word(second_op);
            }
            ic++;
            return;
    }
}

unsigned int encode_word(char *operand){
    unsigned int word =0;
    switch (find_method(operand)){
        case METHOD_IMMEDIATE:
            word |= atoi(++operand);
            word = insert_field(word, ABSOLUTE);
            return word;
        case METHOD_DIRECT:
            if(find_label(operand) == NULL){
                error = LABEL_NOT_FOUND;
                return 0;
            }
            return encode_label(operand);
        case METHOD_REGISTER:
            return encode_word_registers(NULL, operand, ABSOLUTE);
        case METHOD_INDIRECT_REGISTER:
            return encode_word_registers(NULL, operand, RELOCATABLE);
    }
    return word;
}

unsigned int encode_word_registers(char *reg1, char *reg2, fields field){
    unsigned int word = 0;
    word |= find_register(reg1);
    word <<= REGISTER_BITS;
    word |= find_register(reg2);
    word = insert_field(word, field);
    return word;
}

int find_register(char *reg){
    if(reg == NULL)
        return 0;
    return atoi(++reg);
}

unsigned int encode_label(char *label){
    unsigned int word = 0;
    int address;
    address = get_address(find_label(label));
    word |= address;
    if(get_extern(find_label(label))){
        add_label(extern_table_head, label, ic, TRUE);
        word = insert_field(word, EXTERN);
    }
    else
        word = insert_field(word, RELOCATABLE);
    return word;
}


void build_output_files(char *filename){
    build_object_file(filename);
    if(extern_flag)
        build_extern_file(filename);
    if(entry_flag)
        build_entry_file(filename);
}

void build_object_file(char *filename){
    int line=100;
    int count=0;
    filename = add_extension(filename,".ob");
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "   %d %d\n",ic,dc);
    while(ic--){
        fprintf(fp,"%04d  %05d\n",line++,dec_to_octal(code[count++]));
    }
    count=0;
    while(dc--){
        fprintf(fp, "%04d %05d\n",line++,dec_to_octal(data[count++]));
    }
    printf("%s was created.\n",filename);
    fclose(fp);
}

void build_extern_file(char *filename){
    extern_table *p = extern_table_head;
    filename = add_extension(filename,".ext");
    FILE *fp = fopen(filename, "w");
    while(p != NULL){
        fprintf(fp,"%s\t%05d",p->label,dec_to_octal(p->address));
        p = p->next;
    }
    printf("%s was created.\n",filename);
    fclose(fp);
}

void build_entry_file(char *filename){
    label_table *p = table_head;
    filename = add_extension(filename,".ent");
    FILE *fp = fopen(filename, "w");
    while(p!=NULL){
        if(p->entry)
            fprintf(fp,"%s\t%05d",p->label,dec_to_octal(p->address));
        p = p->next;
    }
    printf("%s was created.\n",filename);
    fclose(fp);
}