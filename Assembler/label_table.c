/*
 * Created by Tomer Goodovitch on 06/03/2020.
 *
 * This file implements the label table (a linked list structure) functions.
 */

#include "label_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "dictionaries.h"

/** This function find a label in the table_head linked list.
 * @param label: the label to be found.
 * @return: pointer to label with success, NULL otherwise.
 * */
label_table * find_label(const char *label){
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

/** This function appends a node to the end of a linked list.
 * It allocates memory for each new node.
 * @param head: a pointer to the linked list head pointer.
 * @param name: the label name.
 * @param address: the label address (0 if external or entry).
 * @param is_extern: a boolean to indicate whether the label is declared as extern.
 * @param ...: further information (e.g. entry boolean, is_directive boolean).
 * */
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

/**This function removes the last label from the table_head linked list.*/
void remove_last_label(){
    label_table *p = table_head, *q = p;
    if(table_head == NULL)
        return;
    while(p->next != NULL) {
        q = p;
        p = p->next;
    }
    q -> next = NULL;
    free(p);
}

/**This function updates the addresses of labels in the table_head linked list.
 * If they are directive we add the final ic+100 to aline it for the linker and loader.*/
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

/**This function sets the entry boolean to true if a label was declared as entry
 * (used if the entry declaration is after the label is already declared.)
 * @param label: the name of the label.*/
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

/**This functions free all allocated memory in a given list.
 * @param head: a pointer to the linkedlist's head pointer.*/
void free_list(label_table** head){
    label_table * p = *head, *q =NULL;
    if(*head == NULL){
        return;
    }
    while(p != NULL){
        q=p;
        p = p->next;
        free(q);
    }
}

/**This is a get function for the address of a label.
 * @param p: a pointer to a node in the linked list.*/
int get_address(const label_table *p){
    return p->address;
}

/**This is a get function for the is_extern boolean of a label.
 * @param p: a pointer to a node in the linked list.*/
int get_extern(const label_table *p){
    return p->is_extern;
}