/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This file implements the first-pass functions.
 */
#include "first_pass.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "info.h"
#include "utility.h"

/**This function reads a line from the file and manages all the first pass processing.
 * @param fp: FILE pointer to the file to be processed.**/
void first_pass(FILE* fp){
    char instruction[MAX_LINE] = "";
    int line_number = 1;
    while(fgets(instruction, MAX_LINE, fp) != NULL){
        error = NO_ERR;
        if(!ignore_line(instruction))
            process_line(instruction);
        if(is_error())
            write_error(line_number);

        if(overflow()){
            error = OVERFLOW;
            error_flag = TRUE;
            fprintf(stderr, "Size too big\n");
            return;
        }

        line_number++;
    }
    update_label_table();
}

/**This function processes a line of the file, identifies its content and sends to sub processing functions.
 * @param instruction: char pointer to the start of the line.**/
void process_line(char* instruction){
    char token[MAX_LINE] = "";

    instruction = skip_spaces(instruction);

    if(end_of_line(instruction)) return; /*blank or comment line*/
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

/**This function identifies all directives(.data, .string, .extern, .entry)
 * and sends to the appropriate sub-functions to handle.
 * @param directive: char pointer to the directive.*/
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

/**This function handles the .data directive.
 * @param data: char pointer to the first variable declared in data.*/
void handle_data(char *data){
    char token[MAX_LINE] = "";
    copy_token(data,token);

    while(!end_of_line(data)){
        if(*token == ','){
            error = ILLEGAL_COMMA_DATA;
            return;
        }
        if(!check_number_validity(token)){
            error = DATA_SYNTAX_ERROR;
            return;
        }
        else if(atoi(token)>=SHRT_MIN&&atoi(token)<=SHRT_MAX){
            write_to_data(atoi(token));
        } else{
            error = DATA_INT_OVERFLOW;
            return;
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
    if(*token == ','){
        error = ILLEGAL_COMMA_DATA;
        return;
    }
}

/**This function checks for the validity of a string that should represent an integer.
 * @param num: char pointer to the string.
 * @return: TRUE If the the number is valid , FALSE otherwise.*/
int check_number_validity(char *num){
    if(*num == '-' || *num == '+')
        num++;
    if(!isdigit(*num))
        return FALSE;
    while(*num != '\0' && *num != ',' && !isspace(*num)){
        if(!isdigit(*num))
            return FALSE;
        num++;
    }
    return TRUE;
}

/**This function converts the .data integers and .string strings into unsigned int and
 * writes them into the data array to be outputted in the .ob file.
 * @param num: the integer to be written to data[].*/
void write_to_data(int num){
    data[dc++] = (unsigned int) num;
}

/**This function handles the .string directive.
 * @param string: is a char pointer to a string declared in .string directive.*/
void handle_string(char *string){
    if(*string != '"'){
        error = STRING_SYNTAX_ERROR;
        return;
    }else{
        string++;
    }
    while(*string != '"'){
        if(end_of_line(string)){
            error = STRING_SYNTAX_ERROR;
            return;
        }
        write_to_data((int) *string);
        string++;
    }
    if(!end_of_line(++string)||!end_of_line(next_token(string))){
        error = NUMBER_OF_STRINGS_ERROR;
    }
    write_to_data(0);
}

/**This function handles the .extern directive. Adds it to the label_table Linked-list.
 * @param label: is the label (char pointer) declared as extern.*/
void handle_extern(char *label){
    extern_flag = TRUE;
    if(is_label(label,FALSE)){
        if(isspace(label[strlen(label)-1]))
            label[strlen(label)-1] = '\0';
        add_label(&table_head, label,0,TRUE, FALSE);
    }
}

/**This function handles all labels and ads them to the label table.
 * @param instruction: is a char pointer to the start of the label.*/
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
        add_label(&table_head, label, dc, FALSE, TRUE);
    }
    else if(find_operation(token) != UNKNOWN_COMMAND){
        if(label[strlen(label)-1] == ':' || isspace(label[strlen(label)-1])){
            label[strlen(label)-1] = '\0';
        }
        add_label(&table_head, label, ic+STARTING_OFFSET, FALSE, FALSE);
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

/**This functions checks whether the given string is a label and for its validity.
 * @param colon: is a boolean to indicate if the label should end with a colon.*/
int is_label(char * token, int colon){
    char tmp[MAX_LINE] = ".";
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

    if(find_label(token) != NULL){
        error = LABEL_DOUBLE_DEFINITION;
        return TRUE;
    }
    if(find_operation(token) != UNKNOWN_COMMAND){
        error = LABEL_CONFLICTING_NAME;
        return FALSE;
    }
    strcat(tmp,token); /*append a '.' at the begging of the string to check if its a directive.*/
    if(find_directive(tmp) != UNKNOWN_DIRECTIVE){
        error = LABEL_CONFLICTING_NAME;
        return FALSE;
    }

    while(!end_of_line(token)){
        if(!isalnum(*token)){
            error = LABEL_SYNTAX;
            return FALSE;
        }
        token++;
    }
    return TRUE;
}

/**This function handles all operations (mov, add, etc.)
 * @param operation: the operation string excluding a label.*/
void handle_operation(char* operation){
    char command[MAX_LINE] = "";
    int num_of_operands;
    char first_op[20] = "",second_op[20] = "";

    copy_token(operation,command); /**get command*/
    operation = next_token(operation); /**operation -> first operator*/

    num_of_operands = number_of_operands(find_operation(command));

    if(!num_of_operands && end_of_line(next_token(operation))){ /**if expects no operands (like stop operation).*/

        code[ic] = encode_first_word(find_operation(command), FALSE, FALSE, 0, 0);
        ic += calculate_additional_words(find_operation(command), NONE, NONE);
        return;
    }
    if(num_of_operands && end_of_line(operation)){ /**too little operands*/
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }

    if(num_of_operands){
        copy_token(operation,first_op); /**first_op <- first operand.*/
        num_of_operands--;
    }else if(!end_of_line(operation)){ /**too many operands*/
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(!num_of_operands && end_of_line(next_token(operation))){ /**if expects one operand (like jmp operation).*/
        if(!operand_valid_method(find_operation(command), NONE, find_method(first_op))){
            error = ADDRESS_METHOD_ERROR;
            return;
        }
        code[ic] = encode_first_word(find_operation(command), FALSE, TRUE, 0, find_method(first_op));
        ic += calculate_additional_words(find_operation(command), NONE, find_method(first_op));
        return;
    }

    operation = next_token(operation); /**operation -> comma*/
    if(operation == NULL && num_of_operands){ /**too little operands*/
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }
    if(*operation != ',' && !end_of_line(operation)){ /**missing comma*/
        error = MISSING_COMMA_OPERATION;
        return;
    }

    operation = next_token(operation); /**operation -> second operator*/

    if(num_of_operands){
        copy_token(operation, second_op); /**second_op <- second operand.*/
        num_of_operands--;
    }else if(!end_of_line(operation)){ /**too many operands*/
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }

    if(!num_of_operands && !end_of_line(next_token(operation))){ /**too many operands*/
        error = NUMBER_OF_OPERANDS_ERROR;
        return;
    }

    if(!operand_valid_method(find_operation(command), find_method(first_op), find_method(second_op))){
        error = ADDRESS_METHOD_ERROR;
        return;
    }
    /**encodes the first word into code[]*/
    code[ic] = encode_first_word(find_operation(command), TRUE, TRUE, find_method(first_op),find_method(second_op));
    /**increments ic by calculate_additional_words()*/
    ic += calculate_additional_words(find_operation(command), find_method(first_op), find_method(second_op));
}

/**This function encodes into the code array (to be outputted in the .ob file)
 * an unsigned int that is the code of the first words of an operation (mov, add, etc.).
 * @param opcode: the operation opcode.
 * @param src: boolean to indicate source operand.
 * @param dest: boolean to indicate destination operand.
 * @param src_method: source-operand addressing method(methods enum is casted to int).
 * @param dest_method: dest-operand addressing method(methods enum is casted to int).
 * @return: encoded word(unsigned int).*/
unsigned int encode_first_word(int opcode, int src, int dest, int src_method, int dest_method){
    unsigned int word = 0;
    word = opcode;
    word <<= METHOD_BITS;
    if(src && dest)
        word |= src_method;
    word <<= METHOD_BITS;
    if(dest)
        word |= dest_method;
    word = insert_field(word, ABSOLUTE);
    return word;
}

/**This function checks for the validity of the operands in the operation.
 * (the addressing methods: immediate, direct, indirect register amd register).
 * @param src_method: the source-operand addressing method.
 * @param dest_method: the destination-operand addressing method.
 * @param type: the operation.
 * @return: TRUE if valid, FALSE otherwise.*/
int operand_valid_method(operations type, methods source_method, methods dest_method){
    /*
     * 0: IMMEDIATE
     * 1: DIRECT
     * 2: INDIRECT REGISTER
     * 3: REGISTER
     */
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
            return dest_method == METHOD_DIRECT || dest_method == METHOD_INDIRECT_REGISTER;
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
    error = COMMAND_NOT_FOUND;
    return TRUE;
}

/**This function returns the number of operands an operation accepts.
 * @param type: the operation type.*/
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

/**This function calculate additional words the operation needs in the RAM to increment ic.
 * @param type: the operation type (mov, add, etc.).
 * @param src_method: the source-operand addressing method.
 * @param dest_method: the destination-operand addressing method.*/
int calculate_additional_words(operations type, methods src_method, methods dest_method){
    int words = 1, register_flag = FALSE;
    if(src_method == METHOD_IMMEDIATE || src_method == METHOD_DIRECT)
        words++;
    else if(src_method == METHOD_REGISTER || src_method == METHOD_INDIRECT_REGISTER){
        register_flag = TRUE;
        words++;
    }
    if(dest_method == METHOD_IMMEDIATE || dest_method == METHOD_DIRECT)
        words++;
    else if((dest_method == METHOD_REGISTER || dest_method == METHOD_INDIRECT_REGISTER) && !register_flag)
        words++;
    return words;
}

/**This function checks if the total words that have been encoded so far exceeds the RAM size (MACHINE_RAM: 4096).
 * @return: TRUE if it does, FALSE otherwise.*/
int overflow(){
    return ic+dc>MACHINE_RAM;
}

/**This function checks if an error has been detected in the last line that was processed.
 * @return: TRUE if it has, FALSE otherwise.*/
int is_error(){
    if(error == NO_ERR)
        return FALSE;
    if(!error_flag){
        fprintf(stderr,"\n\n");
        fprintf(stderr,"--------------------------------------------------------------------------------------\n");
        fprintf(stderr,"\n\n");
    }
    error_flag = TRUE;
    return TRUE;
}