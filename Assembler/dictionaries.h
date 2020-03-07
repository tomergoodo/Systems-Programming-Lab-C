//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_DICTIONARIES_H
#define ASSEMBLER_DICTIONARIES_H

typedef enum Operations{MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP, UNKNOWN_COMMAND = -1 } operations;
typedef enum directives {DATA, STRING, ENTRY, EXTERN, UNKNOWN_DIRECTIVE = -1} directives;
typedef enum errors {SYNTAX_ERR, FILE_NOT_FOUND, LABEL_SYNTAX, LABEL_LENGTH, EMPTY_LABEL_LINE, LABEL_DOUBLE_DEFINITION, COMMAND_NOT_FOUND, DIRECTIVE_NOT_FOUND,  /*, ...*/ NO_ERR = -1};

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


operations find_operation(const char* str);
operations find_directive(const char* str);
#endif //ASSEMBLER_DICTIONARIES_H
