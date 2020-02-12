#ifndef __INFO_H
#define __INFO_H

#define MAX_LINE 200
#define MAX_NUMBER 127
#define MAX_DISPLAY 16
#define TRUE 1
#define FALSE 0
#define EOI -1 /*end of input*/
#define READOPERANDSIZE 2

typedef enum SETS{ NONE, SETA_e, SETB_e, SETC_e, SETD_e, SETE_e, SETF_e, undefined = -1 } sets;
typedef enum OPERANDS{ read_set, print_set, union_set, intersect_set, sub_set, symdiff_set, stop, error = -1 } operands;


#endif