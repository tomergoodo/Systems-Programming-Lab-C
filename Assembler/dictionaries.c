//
// Created by Tomer Goodovitch on 01/03/2020.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utility.h"
#include "dictionaries.h"

struct operations_dictionary{
    operations val;
    const char* str;
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

struct directives_dictionary{
    directives val;
    const char* str;
} conversion_directives[] = {
        {DATA, ".data"},
        {STRING, ".string"},
        {ENTRY, ".entry"},
        {EXTERN, ".extern"}
};

operations find_operation(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_operations) / sizeof(conversion_operations[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_operations[i].str))
            return conversion_operations[i].val;
    return UNKNOWN_COMMAND;
}

directives find_directive(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_directives) / sizeof(conversion_directives[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_directives[i].str))
            return conversion_directives[i].val;
    return UNKNOWN_DIRECTIVE;
}

methods find_method(char* str) {
    if(end_of_line(str))
        return NONE;
    if (*str == 'r') {
        str++;
        if (atoi(str) >= 0 && atoi(str) <= 7) {
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