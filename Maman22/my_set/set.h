#ifndef __SET_H
#define __SET_H

#define SETSIZE 4

typedef int set[SETSIZE]; /*Size=32*4=128 - one bit for each number in the closed range [0-127]*/


int* read_set_numbers(const char* operand);
int is_valid_number(cstrTOenumOPhar* p);
void read_set_f(set SET, int* numbers);

void print_set_f(set SET);

void union_set_f(set SET1, set SET2, set SET3);

void intersect_set_f(set SET1, set SET2, set SET3);

void sub_set_f(set SET1, set SET2, set SET3);

void symdiff_set_f(set SET1, set SET2, set SET3);

void empty_set(set SET);

#endif