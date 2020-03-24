/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This file implements the "find" functions matching keys(strings) to values(enums declared in the header).
 *
*/

#include "dictionaries.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utility.h"

/**This is a dictionary struct for the operations matching a key (string)
 * with the corresponding value (operations enum).**/
struct operations_dictionary{
    operations val;
    const char* key;
} conversion_operations[] = {
        {MOV, "mov"},
        {CMP, "cmp"},
        {ADD, "add"},
        {SUB, "sub"},
        {LEA, "lea"},
        {CLR, "clr"},
        {NOT, "not"},
        {INC, "inc"},
        {DEC, "dec"},
        {JMP, "jmp"},
        {BNE, "bne"},
        {RED, "red"},
        {PRN, "prn"},
        {JSR, "jsr"},
        {RTS, "rts"},
        {STOP,"stop"}
};

/**This is a dictionary struct for the directives matching a key (string)
 * with the corresponding value (directives enum).**/
struct directives_dictionary{
    directives val;
    const char* key;
} conversion_directives[] = {
        {DATA, ".data"},
        {STRING, ".string"},
        {ENTRY, ".entry"},
        {EXTERN, ".extern"}
};

/**This function returns the corresponding value to the given key.
 * @param str: string to be checked (key).
 * @return: operations enum (value) corresponding to the string. If unrecognized returns UNKNOWN_COMMAND**/
operations find_operation(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_operations) / sizeof(conversion_operations[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_operations[i].key))
            return conversion_operations[i].val;
    return UNKNOWN_COMMAND;
}

/**This function returns the corresponding value to the given key.
 * @param str: string to be checked (key).
 * @return: directives enum (value) corresponding to the string. If unrecognized returns UNKNOWN_DIRECTIVE**/
directives find_directive(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_directives) / sizeof(conversion_directives[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_directives[i].key))
            return conversion_directives[i].val;
    return UNKNOWN_DIRECTIVE;
}

/**This function returns the corresponding value to the given string.
 * @param str: string to be checked (key).
 * @return: methods enum (value) corresponding to the string. If none is given returns NONE.
 * If unrecognized returns METHOD_UNKNOWN**/
methods find_method(char* str) {
    if(end_of_line(str))
        return NONE;
    if (*str == 'r') {
        str++;
        if (atoi(str) >= MIN_REGISTER && atoi(str) <= MAX_REGISTER) {
            return METHOD_REGISTER;
        }
        str--;
    }
    if(isalpha(*str)){
        return METHOD_DIRECT;
    }
    switch(*str){
        case '#':
            return METHOD_IMMEDIATE;
        case '*':
            if(find_method(++str) == METHOD_REGISTER)
                return METHOD_INDIRECT_REGISTER;
    }
    error = METHOD_UNKNOWN_ERROR;
    return METHOD_UNKNOWN;
}