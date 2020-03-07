//
// Created by Tomer Goodovitch on 06/03/2020.
//
#include <stdarg.h>
#include "label_table.h"

int find_label(char * label){
    label_table *p = table_head;
    if(table_head == NULL)
        return FALSE;
    while(p != NULL){
        if(p->name == label)
            return TRUE;
    }
    return FALSE;
}

void add_label(const char * name, const unsigned int address, const int is_extern, ...){
    label_table * p = table_head;
    va_list list;
    va_start(list, is_entry);
    if(table_head == NULL){
        table_head =(label_table*)malloc(sizeof(label_table));
        table_head->next = NULL;
        table_head->name = name;
        table_head->address = address;
        table_head->is_entry = is_entry;
        table_head->is_directive = va_arg(list, int);
        return;
    }
    while(p->next != NULL)
        p = p->next;
    p =(label_table*)malloc(sizeof(label_table));
    p->next = NULL;
    p->name = name;
    p->address = address;
    p->is_extern = is_extern;
    p->is_directive = va_arg(list, int);

    va_end(list);
}
