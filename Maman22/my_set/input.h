#ifndef __INPUT_H
#define __INPUT_H
#include "info.h"

operands strTOenumOP(const char* str);
sets strTOenumSET(const char* str);

void command_input(char* operand);
set* set_input(const char* operand, int setCounter);

int check_input(const char* operand);

int check_extraneous_text(char* token, int parameters);


#endif