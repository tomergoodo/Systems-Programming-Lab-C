//
// Created by Tomer Goodovitch on 06/03/2020.
//

#ifndef ASSEMBLER_LABEL_TABLE_H
#define ASSEMBLER_LABEL_TABLE_H
#define MAX_LINE 120
#define TRUE 1
#define FALSE 0

extern int ic;
extern int error;

typedef struct label_table label_table;

void add_label(const char * name, const unsigned int address, const int is_extern, ...);
label_table * find_label(char * label);
void remove_last_label();
void update_label_table();
void handle_entry(char * label);

int get_address(label_table *p);
int get_extern(label_table *p);

label_table * table_head;
#endif //ASSEMBLER_LABEL_TABLE_H
