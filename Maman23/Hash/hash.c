#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

#define MAX_LINE 255
#define TRUE 1
#define FALSE 0
#define range 29

/*hsah_table is built of an array of 29 pointers to heads of LinkedLists.
Each node represents a file.
Each cell in the array represents the correspondin number.*/
typedef struct hash_table{
    struct node *keys[range];
}hash_table;

struct hash_table *HashTable;


void open_file(char* filename);
void update_hash(char* filename,int number);
void display();
void dealloc();

int main(int argc, char*argv[]){
    HashTable = (struct hash_table *)malloc(sizeof(struct hash_table));
    while(--argc > 0){
        open_file(*++argv);
    }
    display();
    dealloc();
    return 0;
}


/*opens file and calls update_hash with the information in the file*/
void open_file(char* filename){
    FILE *file;
    char* file_content;
    char buff[MAX_LINE];
    file = fopen(filename,"r");
    int i=0;
    int number;

    if(file == NULL){
        fprintf(stderr, "File name %s was not found\n",filename);
        exit(0);
    }
    while(fscanf(file,"%d", &number) == TRUE)
        update_hash(filename,number);
    fclose(file);
}

/*updates the hash table*/
void update_hash(char* filename,int number){
    node ** ptr=&(HashTable->keys[number]);
    insert_file(filename,ptr);
    increment_value(filename,HashTable->keys[number]);
}

/*displays the hsah table*/
void display(){
    int i;
    int flag=TRUE;
    struct node *ptr;
    for(i=0;i<range;i++){
        ptr=HashTable->keys[i];
        while(ptr!=NULL){
            if(ptr->data > 0 && flag){
                printf("%d appers in file %s - %d time",i,ptr->file_name,ptr->data);
                flag=FALSE;
            }
            else if(ptr->data > 0)
                printf(", file %s - %d time",ptr->file_name,ptr->data);
            ptr = ptr->next_node;
        }
        if(!flag)
            printf("\n");
        flag=TRUE;
    }
}
void dealloc(){
    int i;
    for(i=0;i<range;i++){
        deallocList(HashTable->keys[i]);
    }
}