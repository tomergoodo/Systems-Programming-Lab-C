//
// Created by Tomer Goodovitch on 06/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "label_table.h"
#include "dictionaries.h"

struct label_table{
    char label[MAX_LINE];
    unsigned int address;
    int is_directive;
    int is_extern;
    int entry;
    struct label_table * next;
};

label_table * find_label(char * label){
    label_table *p = table_head;
    if(table_head == NULL)
        return NULL;
    while(p != NULL){
        if(strcmp(p->label, label) == 0)
            return p;
        p = p->next;
    }
    return NULL;
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
        table_head->entry = FALSE;
        return;
    }
    while(p->next != NULL)
        p = p->next;
    p->next =(label_table*)malloc(sizeof(label_table));
    p = p->next;
    p->next = NULL;
    strcpy(p->label, name);
    p->address = address;
    p->is_extern = is_extern;
    p->is_directive = va_arg(list, int);
    p->entry = FALSE;
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

void update_label_table(){
    label_table * p = table_head;
    if(table_head == NULL)
        return;
    while(p != NULL) {
        if(p->is_directive)
            p->address += 100;
        if(!p->is_directive)
            p->address += ic+100;
        p = p->next;
    }
}

void handle_entry(char * label){
    label_table * p = table_head;
    int flag = FALSE;
    if(table_head == NULL){
        error = ENTRY_NOT_FOUND;
        return;
    }
    while(p != NULL) {
        if(strcmp(p->label, label) == 0){
            p->entry = TRUE;
            flag = TRUE;
        }
        p = p->next;
    }
    if(!flag)
        error = ENTRY_NOT_FOUND;
}

int get_address(label_table *p){
    return p->address;
}

int get_extern(label_table *p){
    return p->is_extern;
}