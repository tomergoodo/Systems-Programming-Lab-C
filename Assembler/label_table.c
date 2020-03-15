//
// Created by Tomer Goodovitch on 06/03/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "label_table.h"
#include "dictionaries.h"

label_table * find_label(const char * label){
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

void add_label(label_table** head, const char * name, const unsigned int address, const int is_extern, ...){
    label_table * p = *head;
    va_list list;
    va_start(list, is_extern);
    if(*head == NULL){
        *head =(label_table*)malloc(sizeof(label_table));
        (*head)->next = NULL;
        strcpy((*head)->label, name);
        (*head)->address = address;
        (*head)->is_directive = va_arg(list, int);
        (*head)->is_extern = is_extern;
        (*head)->entry = FALSE;
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
            p->address += ic+100;
        p = p->next;
    }
}

void set_entry(const char * label){
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

int get_address(const label_table *p){
    return p->address;
}

int get_extern(const label_table *p){
    return p->is_extern;
}