//
// Created by Tomer Goodovitch on 06/03/2020.
//

#ifndef ASSEMBLER_LABEL_TABLE_H
#define ASSEMBLER_LABEL_TABLE_H

void add_label(const char * name, const unsigned int address, const int is_extern, ...);
int find_label(char * label);

typedef struct label_table{
    char label[MAX_LINE];
    unsigned int address;
    int is_directive;
    int is_extern;
    int is_entry;
    label_table * next;

}label_table * table_head;


#endif //ASSEMBLER_LABEL_TABLE_H
