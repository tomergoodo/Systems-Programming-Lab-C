//
// Created by Tomer Goodovitch on 28/02/2020.
//

#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){
    //printf("%d", sizeof(unsigned int));
    while(argc){
        handle_file(*argv++);
        argc--;
    }
    return 0;
}

void handle_file(char * filename){
    FILE *fp;
    filename = add_extention(filename);
    fp = fopen(filename,"r");
    if(fp == NULL) {
        error = FILE_NOT_FOUND;
        fprintf(stderr, "File %s was not found\n", filename);
        return;
    }
    process_file(fp);
    fclose(fp);
}