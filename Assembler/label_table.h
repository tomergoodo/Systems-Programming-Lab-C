/*
 * Created by Tomer Goodovitch on 06/03/2020.
 *
 * This is a header file for label_table.c
 * This file typedefs the label_table struct.
 */

#ifndef ASSEMBLER_LABEL_TABLE_H
#define ASSEMBLER_LABEL_TABLE_H

#include "info.h"

typedef struct label_table label_table;
/**This is a struct of a node in a linked list containing information about labels that are declared in the file.
 * For each label in the file we add a node to the linked list with information about the label.*/
struct label_table{
    char label[MAX_LINE];
    unsigned int address;
    int is_directive;
    int is_extern;
    int entry;
    struct label_table * next;
};

/**----------------------------------------------------prototypes-----------------------------------------------------*/

void add_label(label_table** head, const char * name, const unsigned int address, const int is_extern, ...);
label_table * find_label(const char * label);
void remove_last_label();
void update_label_table();
void set_entry(const char * label);

int get_address(const label_table *p);
int get_extern(const label_table *p);

void free_list(label_table** head);

/**------------------------------------------------external variables-------------------------------------------------*/

extern int ic;
extern int error;
extern label_table *extern_table_head;
extern label_table *table_head;

#endif /*ASSEMBLER_LABEL_TABLE_H*/