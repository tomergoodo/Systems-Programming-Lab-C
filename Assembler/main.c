//
// Created by Tomer Goodovitch on 28/02/2020.
//

#include <stdio.h>
#include "utility.h"
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
unsigned int data [2000];
unsigned int code [2000];


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
    extended_filename = add_extension(filename,".as",3);
    fp = fopen(extended_filename,"r");
    if(fp == NULL) {
        error = FILE_NOT_FOUND;
        fprintf(stderr, "File %s was not found\n", filename);
        return;
    }
    process_file(fp);
    if(!error_flag){
        printf("Finished first-pass processing successfully\n");
        rewind(fp);
        second_pass(fp, filename);
        if(!error_flag){
            printf("Finished second-pass processing successfully\n");
        }
    }
    else
        printf("Finished %s first-pass processing. Errors have been detected.\n",filename);
    fclose(fp);
}


void set_parameters(){
    ic = 0;
    dc = 0;
    error = NO_ERR;
}