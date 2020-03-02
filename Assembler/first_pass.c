//
// Created by Tomer Goodovitch on 01/03/2020.
//

#include "first_pass.h"
#include "dictionaries.h"
#include "utility.h"
#define MAX_LINE 120

extern int error;
extern int ic;
extern int dc;

void process_file(FILE* fp){
    char instruction[MAX_LINE];
    int line_number = 1;
    while(fgets(instruction, MAX_LINE, fp) != NULL){
        error = NO_ERR;
        if(!ignore_line(instruction))
            process_line(instruction);
        if(is_error())
            write_err(line_number);
        line_number++;
    }
}

void process_line(char* instruction){
    char* token;

    if(end_of_line(instruction)) return;
    if(!isalpha(*line) && *line != '.') {
        error = SYNTAX_ERR;
        return;
    }
    instruction = skip_spaces(instruction);

    if(str_to_enum_instructions(instruction)!=-1){
        handle_operation(instruction);
    }
    else if(*instruction=='.'){
        handle_directive(instruction);
    }
    else{
        handle_label(instruction);
    }
}

void handle_operation(char* operation){
    char* token;
    token = strtok(operation, "\t\n, ");
    switch(str_to_operation_enum(token)){
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
            break;
    }
}

void handle_operation(char* directive){
    directive = skip_spaces(directive);
    switch(str_to_directive_enum(directive)){
        case DATA:
            break;
        case STRING:
            break;
        case ENTRY:
            break;
        case EXTERN:
            break;
        case UNKNOWN_TYPE:
            break;
    }
}

void handle_label(char* label){
    
}

int is_error(){
    if(error == NO_ERR)
        return 0;
    return 1;
}