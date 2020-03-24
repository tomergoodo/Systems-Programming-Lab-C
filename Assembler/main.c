//
// Created by Tomer Goodovitch on 28/02/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "info.h"
#include "dictionaries.h"
#include "first_pass.h"
#include "second_pass.h"
#include "label_table.h"

void handle_file(char * filename);
void set_parameters();
int error;
int error_flag = FALSE;
int extern_flag = FALSE;
int entry_flag = FALSE;
int ic;
int dc;
label_table *table_head;
label_table *extern_table_head;
unsigned int data [MACHINE_RAM];
unsigned int code [MACHINE_RAM];


int main(int argc, char* argv[]){
    while(--argc){
        set_parameters();
        handle_file(*(++argv));
    }
    return 0;
}

void handle_file(char * filename){
    FILE *fp;
    char * extended_filename;
    extended_filename = add_extension(filename,".as",EXTENSION_LENGTH3);
    fp = fopen(extended_filename,"r");
    free(extended_filename);
    if(fp == NULL) {
        error = FILE_NOT_FOUND;
        fprintf(stderr, "File %s was not found\n", filename);
        return;
    }
    printf("\nStarting %s processing\n",filename);
    first_pass(fp);
    if(!error_flag){
        printf("Finished first-pass processing successfully\n");
        rewind(fp);
        second_pass(fp, filename);
        if(!error_flag){
            printf("Finished %s second-pass processing successfully\n",filename);
        } else
            printf("Finished %s second-pass processing. Errors have been detected.\n",filename);
    }
    else
        printf("Finished %s first-pass processing. Errors have been detected.\n",filename);
    free_list(&table_head);
    free_list(&extern_table_head);
    fclose(fp);
}


void set_parameters(){
    error_flag = FALSE;
    ic = 0;
    dc = 0;
    error = NO_ERR;
    extern_flag = FALSE;
    entry_flag = FALSE;
    data [MACHINE_RAM] = 0;
    code [MACHINE_RAM] = 0;
    table_head = NULL;
    extern_table_head = NULL;
}