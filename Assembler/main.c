//
// Created by Tomer Goodovitch on 28/02/2020.
//

#include <stdio.h>
typedef enum Instructions{mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, red, prn, jsr, rts, stop, error = -1 } instructions;

struct instruction_dictionary{
    instructions val;
    const char* str;
} conversionOP[] = {
        {mov, "mov"},
        {cmp, "cmp"},
        {add, "add"},
        {sub, "sub"},
        {lea, "lea"},
        {clr, "clr"},
        {not, "not"},
        {inc, "inc"},
        {dec, "dec"},
        {jmp, "jmp"},
        {bne, "bne"},
        {red, "red"},
        {prn, "prn"},
        {jsr, "jsr"},
        {rts, "rts"},
        {stop,"stop"}
};

instructions str_to_enum_instructions(const char* str) {
    int i;
    for (i = 0; i < sizeof(conversionOP) / sizeof(conversionOP[0]); i++)
        if (str!=NULL&&!strcmp(str, conversionOP[i].str))
            return conversionOP[i].val;
    return error;
}


int main(int argc, char* argv[]){
    while(argc){
        handle_file(*argv++);
        argc--;
    }
    return 0;
}

void handle_file(char * filename){
    FILE *fp=fopen(filename,"r");
    if(fp == NULL){
        fprintf(stderr, "File name %s was not found\n",filename);
        exit(1);
    }
    while()

}