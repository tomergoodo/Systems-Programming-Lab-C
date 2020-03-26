/*
 * Created by Tomer Goodovitch on 01/03/2020.
 *
 * This file typedefs enums.
 *
 */

#ifndef ASSEMBLER_DICTIONARIES_H
#define ASSEMBLER_DICTIONARIES_H

#include "info.h"

extern int error;

/**------------------------------------------------------enums-------------------------------------------------------**/

/**This is an enum for all operations**/
typedef enum operations {MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, RED,
                        PRN, JSR, RTS, STOP, UNKNOWN_COMMAND = NOT_FOUND } operations;

/**This is an enum for all directives**/
typedef enum directives {DATA, STRING, EXTERN, ENTRY, UNKNOWN_DIRECTIVE = NOT_FOUND} directives;

/**This is an enum for all errors**/
typedef enum errors {FILE_NOT_FOUND, SYNTAX_ERR, LABEL_SYNTAX, LABEL_LENGTH, EMPTY_LABEL_LINE, LABEL_DOUBLE_DEFINITION,
                     LABEL_CONFLICTING_NAME, LABEL_SYNTAX_COLON, COMMAND_NOT_FOUND, DIRECTIVE_NOT_FOUND,
                     ILLEGAL_COMMA_DATA, DATA_SYNTAX_ERROR, DATA_INT_OVERFLOW, MISSING_COMMA_DATA, STRING_SYNTAX_ERROR,
                     NUMBER_OF_STRINGS_ERROR, MISSING_COMMA_OPERATION, ADDRESS_METHOD_ERROR, NUMBER_OF_OPERANDS_ERROR,
                     IMMEDIATE_INT_OVERFLOW, METHOD_UNKNOWN_ERROR, ENTRY_NOT_FOUND, LABEL_NOT_FOUND, OVERFLOW,
                     NO_ERR = NOT_FOUND} errors;

/**This is an enum for all addressing methods**/
typedef enum methods {METHOD_IMMEDIATE = BIT_ZERO, METHOD_DIRECT = BIT_ONE, METHOD_INDIRECT_REGISTER = BIT_TWO,
                      METHOD_REGISTER = BIT_THREE, NONE, METHOD_UNKNOWN = NOT_FOUND} methods;

/**This is an enum for all fields (external, relocatable and absolute)**/
typedef enum fields {EXTERNAL = BIT_ZERO, RELOCATABLE = BIT_ONE, ABSOLUTE = BIT_TWO} fields;


/**-----------------------------------------------------prototypes---------------------------------------------------**/

operations find_operation(const char* str);
directives find_directive(const char* str);
methods find_method(char* str);

#endif /*ASSEMBLER_DICTIONARIES_H*/
