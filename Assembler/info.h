/*
 * Created by Tomer Goodovitch on 17/03/2020.
 *
 * This file is a header file for all the constants in use.
 *
 */

#ifndef ASSEMBLER_INFO_H
#define ASSEMBLER_INFO_H

#define FALSE 0
#define TRUE 1


#define STARTING_OFFSET 100 /*Starting offset in the RAM*/

/**-----------------------------------bitwise operations-----------------------------------*/
/*Constants for working with encoding operations to a word*/

#define METHOD_BITS 4
#define FIELD_BITS 3
#define REGISTER_BITS 3

#define BIT_ZERO 1
#define BIT_ONE 2
#define BIT_TWO 4
#define BIT_THREE 8


/**-----------------------------------limitations-----------------------------------*/
#define MACHINE_RAM 4096

/*Max label length with and without colon*/
#define MAX_LABEL_LENGTH 31
#define MAX_LABEL_LENGTH_FULL 32

#define MAX_LINE 120 /*Max length of arrays to store strings that are lines or sub-lines*/
/*Registers go from 0-7*/
#define MIN_REGISTER 0
#define MAX_REGISTER 7
#define IMMEDIATE_MIN -4096
#define IMMEDIATE_MAX 4095
/**-----------------------------------misc-----------------------------------*/
#define CODE_SIZE 5
#define NOT_FOUND -1


#endif /*ASSEMBLER_INFO_H*/
