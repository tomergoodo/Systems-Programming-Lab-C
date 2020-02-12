#ifndef __LinkedList_H
#define __LinkedList_H

/*This is a struct of a node in the LinkedList.*/
typedef struct node{
    char* file_name;
    int data; /*number of appearances*/
    struct node *next_node;
}node;

void insert_file(char* file, struct node **head);
void increment_value(char *file, struct node *head);

#endif