//
// Created by Tomer Goodovitch on 06/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "label_table.h"

struct label_table{
    char label[MAX_LINE];
    unsigned int address;
    int is_directive;
    int is_extern;
    struct label_table * next;
};

int find_label(char * label){
    label_table *p = table_head;
    if(table_head == NULL)
        return FALSE;
    while(p != NULL){
        if(p->label == label)
            return TRUE;
        p = p->next;
    }
    return FALSE;
}

void add_label(const char * name, const unsigned int address, const int is_extern, ...){
    label_table * p = table_head;
    va_list list;
    va_start(list, is_extern);
    if(table_head == NULL){
        table_head =(label_table*)malloc(sizeof(label_table));
        table_head->next = NULL;
        strcpy(table_head->label, name);
        table_head->address = address;
        table_head->is_directive = va_arg(list, int);
        return;
    }
    while(p->next != NULL)
        p = p->next;
    p =(label_table*)malloc(sizeof(label_table));
    p->next = NULL;
    strcpy(table_head->label, name);
    p->address = address;
    p->is_extern = is_extern;
    p->is_directive = va_arg(list, int);

    va_end(list);
}

void remove_last_label(){
    label_table * p = table_head, *q = NULL;
    if(table_head == NULL)
        return;
    while(p->next != NULL) {
        q = p;
        p = p->next;
    }
    q -> next = NULL;
    free(p);
}