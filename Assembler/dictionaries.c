//
// Created by Tomer Goodovitch on 01/03/2020.
//

#include "dictionaries.h"
operations str_to_operation_enum(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_operations) / sizeof(conversion_operations[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_operations[i].str))
            return conversion_operations[i].val;
    return UNKNOWN_COMMAND;
}

operations str_to_directive_enum(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversion_directives) / sizeof(conversion_directives[0]); i++)
        if (str!=NULL && !strcmp(str, conversion_directives[i].str))
            return conversion_directives[i].val;
    return UNKNOWN_TYPE;
}