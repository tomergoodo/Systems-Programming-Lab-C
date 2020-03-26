/*
 * Created by Tomer Goodovitch on 28/02/2020.
 *
 * This is a project to simulate an Assembler with an imaginary computer and assembly language.
 * This is the main file directing the command line parameters to first and second pass processing.
 */


#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "info.h"
#include "dictionaries.h"
#include "first_pass.h"
#include "second_pass.h"
#include "label_table.h"

/**----------------------------------------------------prototypes-----------------------------------------------------*/

void handle_file(char * filename);
void set_parameters();

/**-------------------------------------------------global variables--------------------------------------------------*/

int error; /**error is the global error variable used by many functions to indicate an error was found.*/
int error_flag = FALSE; /**This is a boolean flag to indicate an error was found.*/
int extern_flag = FALSE; /**This is a boolean flag to indicate an external label was declared.*/
int entry_flag = FALSE; /**This is a boolean flag to indicate an entry label was declared.*/
int ic; /**ic is the instruction counter.*/
int dc; /**dc is the data counter.*/
label_table *table_head; /**table_head is the head pointer of the label table linked list.*/
label_table *extern_table_head; /**extern_table_head is the head pointer of the extern label table linked list.*/
unsigned int data [MACHINE_RAM]; /**this is the array in which data words are coded into and then printed to the .ob output file.*/
unsigned int code [MACHINE_RAM];/**this is the array in which code words are coded into and then printed to the .ob output file.*/

/**-------------------------------------------------------------------------------------------------------------------*/


int main(int argc, char* argv[]){
    while(--argc){
        set_parameters();
        handle_file(*(++argv));
    }
    return 0;
}

/**This function handles a file.
 * opens the file and with success sends it to first pass and second pass processing.
 * @param filename: the name of the file to be processed (with no extension).*/
void handle_file(char * filename){
    FILE *fp;
    char * extended_filename;
    extended_filename = add_extension(filename,".as");
    fp = fopen(extended_filename,"r");
    free(extended_filename);
    if(fp == NULL) {
        error = FILE_NOT_FOUND;
        fprintf(stderr, "File %s was not found\n", filename);
        return;
    }
    printf("--------------------------------------------------------------------------------------\n");
    printf("\nStarting %s processing\n",filename);
    first_pass(fp);
    if(!error_flag){
        printf("Finished first-pass processing successfully\n");
        rewind(fp);
        second_pass(fp, filename);
        if(!error_flag){
            printf("\nFinished %s second-pass processing successfully\n\n",filename);
        } else
            printf("\n\nFinished %s second-pass processing. Errors have been detected.\n\n",filename);
    }
    else
        printf("\n\nFinished %s first-pass processing. Errors have been detected.\n\n",filename);
    free_list(&table_head);
    free_list(&extern_table_head);
    fclose(fp);
}

/**This function sets to default the global parameters for a new file processing.*/
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