//
// Created by Tomer Goodovitch on 28/02/2020.
//

#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "dictionaries.h"
#include "first_pass.h"

void handle_file(char * filename);
void set_parameters();
extern int error;
extern int ic;
extern int dc;

int main(int argc, char* argv[]){
    while(argc){
        set_parameters();
        handle_file(*argv++);
        argc--;
    }
    return 0;
}

void handle_file(char * filename){
    FILE *fp;
    filename = add_extension(filename);
    fp = fopen(filename,"r");
    if(fp == NULL) {
        error = FILE_NOT_FOUND;
        fprintf(stderr, "File %s was not found\n", filename);
        return;
    }
    process_file(fp);
    fclose(fp);
}


void set_parameters(){
    ic = 0;
    dc = 0;
    error = NO_ERR;
}