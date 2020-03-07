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
    char* token;
    copy_token(operation,token);
    switch(find_operation(token)){
        case MOV:
            break;
        case CMP:
            break;
        case ADD:
            break;
        case SUB:
            break;
        case LEA:
            break;
        case CLR:
            break;
        case NOT:
            break;
        case INC:
            break;
        case DEC:
            break;
        case JMP:
            break;
        case BNE:
            break;
        case RED:
            break;
        case PRN:
            break;
        case JSR:
            break;
        case RTS:
            break;
        case STOP:
            break;
        case UNKNOWN_COMMAND:
            error = COMMAND_NOT_FOUND;
            break;
    }
}


int is_error(){
    if(error == NO_ERR)
        return FALSE;
    return TRUE;
}