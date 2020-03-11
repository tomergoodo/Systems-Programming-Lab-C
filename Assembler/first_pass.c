//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <stdlib.h>
#include "first_pass.h"
#include "dictionaries.h"
#include "utility.h"
#include "label_table.h"
#define MAX_LINE 120
#define TRUE 1
#define FALSE 0

extern int error;
extern int ic;
extern int dc;
extern unsigned int data [2000];
extern unsigned int code [2000];

void process_file(FILE* fp){
    char instruction[MAX_LINE];
    int line_number = 1;
    while(fgets(instruction, MAX_LINE, fp) != NULL){
        error = NO_ERR;
        if(!ignore_line(instruction))
            process_line(instruction);
        if(is_error())
            write_error(line_number);
        line_number++;
    }
}

void process_line(char* instruction){
    char token[MAX_LINE];

    instruction = skip_spaces(instruction);

    if(end_of_line(instruction)) return; //blank or comment line
    if(!isalpha(*line) && *line != '.') {
        error = SYNTAX_ERR;
        return;
    }

    copy_token(instruction, token);
    if(is_label(token)){
        handle_label(instruction);
        instruction = next_token(instruction);
        copy_token(instruction, token);
        if(find_directive(token) == ENTRY)
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
    char *token;
    copy_token(directive,token);
    switch(find_directive(token)){
        case DATA:
            directive = next_token(directive);
            copy_token(directive, token);
            handle_data(token);
            break;
        case STRING:
            directive = next_token(directive);
            copy_token(directive, token);
            handle_string(token);
            break;
        case EXTERN:
            directive = next_token(directive);
            copy_token(directive, token);
            handle_extern(token);
            break;
        case ENTRY:
            break;
        case UNKNOWN_TYPE:
            error = DIRECTIVE_NOT_FOUND;
            break;
    }
}

void handle_data(char * data){
    char token[MAX_LINE];
    int token;
    copy_token(data,token);

    while(!end_of_line(token)){
        if(!check_number_validity(token)){
            error = DATA_SYNTAX_ERROR;
            return;
        }
        else{
            token = atoi(token);
            write_to_data(token);
        }

        data = next_token(data);
        copy_token(data,token);

        if(token != ',' && !end_of_line(data)){
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
    while(*num != '\0'){
        if(!isdigit(num))
            return FALSE;
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
    if(is_label(label,FALSE)){
        add_label(label,0,TRUE);
    }
}


void handle_label(char* instruction){
    char *label, *token;
    copy_token(instruction, label);
    token = next_token(label);
    if(find_directive(token) != UNKNOWN_DIRECTIVE){
        add_label(label, dc, FALSE, FALSE, TRUE);
    }
    else if(find_operation(token) != UNKNOWN_COMMAND){
        add_label(label, ic, FALSE, FALSE, FALSE)
    }
    else if(end_of_line(token)){
        error = EMPTY_LABEL_LINE;
        return;
    }
    else{
        error = COMMAND_NOT_FOUND;
        return;
    }
}


int is_label(char * token, int colon){
    int i;
    if(colon && find_label(label)){
        error = LABEL_DOUBLE_DEFINITION;
        return;
    }
    if(!isalpha(*token)){
        error = LABEL_SYNTAX;
        return FALSE;
    }
    if(colon && token[strlen(token)-1] != ':'){
        error = LABEL_SYNTAX;
        return FALSE;
    }
    if(strlen(token) > (colon ? MAX_LABEL_LENGTH : (MAX_LABEL_LENGTH-1))){
        error = LABEL_LENGTH;
        return FALSE;
    }
    if(colon) token[strlen(token)-1] = '\0';
    if(find_operation(token) != UNKNOWN_COMMAND){
        error = LABEL_SYNTAX;
    }
    while(token[i] != '\0'){
        if(!isalnum(token[i])){
            error = LABEL_SYNTAX;
            return FALSE;
        }
        i++;
    }
    return TRUE;
}


void handle_operation(char* operation){
    char command[MAX_LINE];
    int number_of_operators;
    char first_op[20],second_op[20];

    copy_token(operation,command); //get command
    operation = next_token(operation); //operation -> first operator

    number_of_operands = number_of_operands(find_operation(command));

    if(number_of_operands){
        copy_token(operation,first_op);
        number_of_operands--;
    }else if(!end_of_line(operation)){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }

    operation = next_token(operation); //operation -> comma

    if(*operation != ',' && !end_of_line(operation)){
        error = MISSING_COMMA_OPERATION;
        return;
    }

    operation = next_token(operation); //operation -> second operator

    if(number_of_operands){
        copy_token(operation, second_op);
        number_of_operands--;
    }else if(!end_of_line(operation)){
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(!operand_valid_method(find_operation(command),find_method(first_op),find_method(second_op))){
        error = ADDRESS_METHOD_ERROR;
        return;
    }

    ic += calculate_additional_words(find_operation(command), find_method(first_op), find_method(second_op));
}


int operand_valid_method(operations type, methods source_method, methods dest_method){
    switch(type){
    /*
     * mov, add and sub:
     * src: 0,1,3
     * dest: 1,3
     */
        case MOV:
        case ADD:
        case SUB:
            return (source_method == METHOD_IMMEDIATE || first_method == METHOD_DIRECT
                || first_method == METHOD_REGISTER)
                && (second_method == METHOD_DIRECT || second_method == METHOD_REGISTER);
    /*
     * cmp:
     * src: 0,1,3
     * dest: 0,1,3
     */
        case CMP:
            return (source_method == METHOD_IMMEDIATE || first_method == METHOD_DIRECT
                    || first_method == METHOD_REGISTER)
                    && (source_method == METHOD_IMMEDIATE || second_method == METHOD_DIRECT
                    || second_method == METHOD_REGISTER);
    /*
     * lea:
     * src: 1
     * dest: 1,3
     */
        case LEA:
            return (source_method == METHOD_DIRECT) && (dest_method == METHOD_DIRECT || dest_method == METHOD_REGISTER);
    /*
     * clr, not, inc, dec, red:
     * no src operand
     * dest: 1,3
     */
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
            return dest_method == METHOD_DIRECT || dest_method == METHOD_REGISTER;

    /*
     * prn:
     * no src operand
     * dest: 0,1,3
     */
        case PRN:
            return dest_method == METHOD_IMMEDIATE || dest_method == METHOD_DIRECT || dest_method == METHOD_REGISTER;
    /*
     * jmp, bne, jsr:
     * no src operand
     * dest: 1,2
     */
        case JMP:
        case BNE:
        case JSR:
            return dest_method == METHOD_IMMEDIATE || dest_method == METHOD_RELATIVE;
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
    return TRUE;
}