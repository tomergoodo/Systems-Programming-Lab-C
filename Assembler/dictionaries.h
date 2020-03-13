//
// Created by Tomer Goodovitch on 01/03/2020.
//

#ifndef ASSEMBLER_DICTIONARIES_H
#define ASSEMBLER_DICTIONARIES_H

typedef enum operations {MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, RED,
                        PRN, JSR, RTS, STOP, UNKNOWN_COMMAND = -1 } operations;

typedef enum directives {DATA, STRING, EXTERN, ENTRY, UNKNOWN_DIRECTIVE = -1} directives;

typedef enum errors {FILE_NOT_FOUND, SYNTAX_ERR, LABEL_SYNTAX, LABEL_LENGTH, EMPTY_LABEL_LINE,
                     LABEL_DOUBLE_DEFINITION, LABEL_CONFLICTING_NAME, LABEL_SYNTAX_COLON, COMMAND_NOT_FOUND,
                     DIRECTIVE_NOT_FOUND, DATA_SYNTAX_ERROR, MISSING_COMMA_DATA, STRING_SYNTAX_ERROR,
                     MISSING_COMMA_OPERATION, ADDRESS_METHOD_ERROR, NUMBER_OF_OPERANDS_ERROR,
                     METHOD_UNKNOWN_ERROR, NO_ERR = -1} errors;

typedef enum methods {METHOD_IMMEDIATE, METHOD_DIRECT, METHOD_RELATIVE, METHOD_REGISTER, NONE, METHOD_UNKNOWN = -1} methods;

typedef enum fields {EXTERNAL, RELOCATABLE, ABSOLUTE} fields;

operations find_operation(const char* str);
directives find_directive(const char* str);
methods find_method(char* str);

#endif //ASSEMBLER_DICTIONARIES_H
