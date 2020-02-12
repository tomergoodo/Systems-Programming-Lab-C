#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"


/*inserting a new node (file) to the end of the LinkedList.
head is a pointer to the head pointer of the LinkedList.*/
void insert_file(char* file, struct node **head)
{
	struct node *ptr;
	
	if (*head == NULL)
	{
		*head = (struct node *)malloc(sizeof(struct node));
		(*head)->file_name = file;
		(*head)->next_node = NULL;
	}
	else
	{
		ptr = *head;
		if(!strcmp(ptr->file_name,file)){
				return;
		}
		while (ptr->next_node != NULL){
			if(!strcmp(ptr->file_name,file)){
				return;
			}
			ptr = ptr->next_node;
		}
		ptr->next_node = (struct node *)malloc(sizeof(struct node));
		ptr = ptr->next_node;
		ptr->file_name = file;
		ptr->next_node = NULL;
	}
}

/*increments the value of the node of the corresponding file to the file parameter.*/
void increment_value(char *file, struct node *head){
    struct node *ptr = head;
    while( ptr != NULL && strcmp(ptr->file_name,file) != 0)
        ptr = ptr->next_node;
	
    if(ptr == NULL){
		return;
    }
    if(strcmp(ptr->file_name,file)==0){
        ptr->data++;
    }
}

void deallocList(struct node *head)
{
	struct node *p = head, *q = head;
	while(p != NULL){
		p=p->next_node;
		free(q);
		q=p;
	}
}