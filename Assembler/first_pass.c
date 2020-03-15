//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utility.h"
#include "label_table.h"
#include "first_pass.h"



void process_file(FILE* fp){
    char instruction[MAX_LINE] = "";
    int line_number = 1;
    while(fgets(instruction, MAX_LINE, fp) != NULL){
        error = NO_ERR;
        if(!ignore_line(instruction))
            process_line(instruction);
        if(is_error())
            write_error(line_number);
        line_number++;
    }
    update_label_table();
}

void process_line(char* instruction){
    char token[MAX_LINE] = "";

    instruction = skip_spaces(instruction);

    if(end_of_line(instruction)) return; //blank or comment line
    if(!isalpha(*instruction) && *instruction != '.') {
        error = SYNTAX_ERR;
        return;
    }

    copy_token(instruction, token);
    if(is_label(token, TRUE)){
        handle_label(instruction);
        instruction = next_token(instruction);
        copy_token(instruction, token);
        if(find_directive(token) == ENTRY || find_directive(token) == EXTERN)
            remove_last_label();
    }
    if(is_error())
        return;

    if(find_directive(token) != UNKNOWN_DIRECTIVE){
        handle_directive(instruction);
    }
    else if(find_operation(token) != UNKNOWN_COMMAND){
        handle_operation(instruction);
    }
    else{
        error = COMMAND_NOT_FOUND;
    }
}


void handle_directive(char* directive){
    char token[MAX_LINE] = "";
    copy_token(directive,token);
    switch(find_directive(token)){
        case DATA:
            directive = next_token(directive);
            handle_data(directive);
            break;
        case STRING:
            directive = next_token(directive);
            handle_string(directive);
            break;
        case EXTERN:
            directive = next_token(directive);
            handle_extern(directive);
            break;
        case ENTRY:
            entry_flag = TRUE;
            break;
        case UNKNOWN_DIRECTIVE:
            error = DIRECTIVE_NOT_FOUND;
            break;
    }
}

void handle_data(char * data){
    char token[MAX_LINE] = "";
    copy_token(data,token);

    while(!end_of_line(data)){
        if(!check_number_validity(token)){
            error = DATA_SYNTAX_ERROR;
            return;
        }
        else{
            write_to_data(atoi(token));
        }

        data = next_token(data);
        copy_token(data,token);

        if(*token != ',' && !end_of_line(data)){
            error = MISSING_COMMA_DATA;
            return;
        }
        else if(!end_of_line(data)){
            data = next_token(data);
            copy_token(data,token);
        }
    }
}

int check_number_validity(char * num){
    if(*num == '-' || *num == '+')
        num++;
    while(*num != '\0' && *num != ',' && !isspace(*num)){
        if(!isdigit(*num))
            return FALSE;
        num++;
    }
    return TRUE;
}

void write_to_data(int num){
    data[dc++] = (unsigned int) num;
}

void handle_string(char * string){
    if(*string != '"'){
        error = STRING_SYNTAX_ERROR;
        return;
    }else{
        string++;
    }
    while(*string != '"' && !end_of_line(string)){
        write_to_data((int) *string);
        string++;
    }
    write_to_data(0);
}

void handle_extern(char * label){
    extern_flag = TRUE;
    if(is_label(label,FALSE)){
        if(isspace(label[strlen(label)-1]))
            label[strlen(label)-1] = '\0';
        add_label(table_head, label,0,TRUE);
    }
}


void handle_label(char* instruction){
    char label[MAX_LINE] = "";
    char token[MAX_LINE] = "";
    copy_token(instruction, label);
    instruction = next_token(instruction);
    copy_token(instruction,token);
    if(find_directive(token) != UNKNOWN_DIRECTIVE){
        if(label[strlen(label)-1] == ':' || isspace(label[strlen(label)-1])) {
            label[strlen(label) - 1] = '\0';
        }
        add_label(table_head, label, dc, FALSE, FALSE, TRUE);
    }
    else if(find_operation(token) != UNKNOWN_COMMAND){
        if(label[strlen(label)-1] == ':' || isspace(label[strlen(label)-1])){
            label[strlen(label)-1] = '\0';
        }
        add_label(table_head, label, ic+100, FALSE, FALSE, FALSE);
    }
    else if(end_of_line(instruction)){
        error = EMPTY_LABEL_LINE;
        return;
    }
    else{
        error = COMMAND_NOT_FOUND;
        return;
    }
}


int is_label(char * token, int colon){
    int i = 0;
    if(colon && (find_operation(token) != UNKNOWN_COMMAND || find_directive(token) != UNKNOWN_DIRECTIVE)){
        return FALSE;
    }
    if(colon && token[strlen(token)-1] != ':'){
        error = LABEL_SYNTAX_COLON;
        return FALSE;
    }
    if(strlen(token) > (colon ? MAX_LABEL_LENGTH : (MAX_LABEL_LENGTH-1))){
        error = LABEL_LENGTH;
        return FALSE;
    }
    if(!isalpha(*token)){
        error = LABEL_SYNTAX;
        return FALSE;
    }

    if(colon) token[strlen(token)-1] = '\0';

    if(colon && find_label(token) != NULL){
        error = LABEL_DOUBLE_DEFINITION;
        return FALSE;
    }

    if(find_operation(token) != UNKNOWN_COMMAND){
        error = LABEL_CONFLICTING_NAME;
        return FALSE;
    }

    while(end_of_line(token)){
        if(!isalnum(token[i])){
            error = LABEL_SYNTAX;
            return FALSE;
        }
        i++;
    }
    return TRUE;
}


void handle_operation(char* operation){
    char command[MAX_LINE] = "";
    int num_of_operands;
    char first_op[20] = "",second_op[20] = "";

    copy_token(operation,command); //get command
    operation = next_token(operation); //operation -> first operator

    num_of_operands = number_of_operands(find_operation(command));

    if(!num_of_operands && end_of_line(next_token(operation))){
        code[ic] = encode_first_word(find_operation(command), FALSE, FALSE, 0, 0);
        ic += calculate_additional_words(find_operation(command), NONE, NONE);
        return;
    }

    if(num_of_operands){
        copy_token(operation,first_op);
        num_of_operands--;
    }else if(!end_of_line(operation)){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(!num_of_operands && end_of_line(next_token(operation))){
        if(!operand_valid_method(find_operation(command), NONE, find_method(first_op))){
            error = ADDRESS_METHOD_ERROR;
            return;
        }
        code[ic] = encode_first_word(find_operation(command), TRUE, FALSE, find_method(first_op), 0);
        ic += calculate_additional_words(find_operation(command), NONE, find_method(first_op));
        return;
    }

    operation = next_token(operation); //operation -> comma
    if(operation == NULL && num_of_operands){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(*operation != ',' && !end_of_line(operation)){
        error = MISSING_COMMA_OPERATION;
        return;
    }

    operation = next_token(operation); //operation -> second operator

    if(num_of_operands){
        copy_token(operation, second_op);
        num_of_operands--;
    }else if(!end_of_line(operation)){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }

    if(!num_of_operands && !end_of_line(next_token(operation))){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(!operand_valid_method(find_operation(command), find_method(second_op), find_method(first_op))){
        error = ADDRESS_METHOD_ERROR;
        return;
    }
    code[ic] = encode_first_word(find_operation(command), TRUE, TRUE, find_method(second_op), find_method(first_op));
    ic += calculate_additional_words(find_operation(command), find_method(second_op), find_method(first_op));
}

unsigned int encode_first_word(int opcode, int src, int dest, int src_method, int dest_method){
    unsigned int word = 0;
    word = opcode;
    word <<= METHOD_BITS;
    if(src && dest)
        word |= src_method;
    word <<= METHOD_BITS;
    if(src && dest)
        word |= dest_method;
    else if (src){
        word |= src_method;
    }
    word = insert_field(word, ABSOLUTE);
    return word;
}

int operand_valid_method(operations type, methods source_method, methods dest_method){
    switch(type){
    /*
     * mov, add and sub:
     * src: 0,1,2,3
     * dest: 1,2,3
     */
        case MOV:
        case ADD:
        case SUB:
            return (source_method == METHOD_IMMEDIATE || source_method == METHOD_DIRECT
                || source_method == METHOD_INDIRECT_REGISTER|| source_method == METHOD_REGISTER)
                && (dest_method == METHOD_DIRECT || dest_method == METHOD_INDIRECT_REGISTER
                || dest_method == METHOD_REGISTER);
    /*
     * cmp:
     * src: 0,1,2,3
     * dest: 0,1,2,3
     */
        case CMP:
            return (source_method == METHOD_IMMEDIATE || source_method == METHOD_DIRECT
                    || source_method == METHOD_INDIRECT_REGISTER|| source_method == METHOD_REGISTER)
                    && (dest_method == METHOD_IMMEDIATE || dest_method == METHOD_DIRECT
                    || dest_method == METHOD_INDIRECT_REGISTER || dest_method == METHOD_REGISTER);
    /*
     * lea:
     * src: 1
     * dest: 1,2,3
     */
        case LEA:
            return (source_method == METHOD_DIRECT) && (dest_method == METHOD_DIRECT || dest_method == METHOD_INDIRECT_REGISTER || dest_method == METHOD_REGISTER);
    /*
     * clr, not, inc, dec, red:
     * no src operand
     * dest: 1,2,3
     */
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
            return dest_method == METHOD_DIRECT || dest_method == METHOD_INDIRECT_REGISTER || dest_method == METHOD_REGISTER;

    /*
     * prn:
     * no src operand
     * dest: 0,1,2,3
     */
        case PRN:
            return dest_method == METHOD_IMMEDIATE || dest_method == METHOD_DIRECT
            || dest_method == METHOD_INDIRECT_REGISTER || dest_method == METHOD_REGISTER;
    /*
     * jmp, bne, jsr:
     * no src operand
     * dest: 1,2
     */
        case JMP:
        case BNE:
        case JSR:
            return dest_method == METHOD_IMMEDIATE || dest_method == METHOD_INDIRECT_REGISTER;
    /*
     * rts, stop:
     * no src operand
     * no dest operand
     */
        case RTS:
        case STOP:
            return TRUE;
        case UNKNOWN_COMMAND:
            error = COMMAND_NOT_FOUND;
            return TRUE;
    }
}

int number_of_operands(operations type){
    switch(type){
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
            return 2;
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
        case RTS:
            return 1;
        case STOP:
            return 0;
        case UNKNOWN_COMMAND:
            error = COMMAND_NOT_FOUND;
            return 0;
    }
    return 0;
}


int calculate_additional_words(operations type, methods src_method, methods dest_method){
    int words = 1, register_flag = FALSE;
    if(src_method != METHOD_REGISTER && src_method != NONE && src_method != METHOD_UNKNOWN)
        words++;
    else if(src_method == METHOD_REGISTER){
        register_flag = TRUE;
        words++;
    }
    if(dest_method != METHOD_REGISTER && dest_method != NONE && dest_method != METHOD_UNKNOWN)
        words++;
    else if(dest_method == METHOD_REGISTER && !register_flag)
        words++;
    return words;
}

int is_error(){
    if(error == NO_ERR)
        return FALSE;
    error_flag = TRUE;
    return TRUE;
}