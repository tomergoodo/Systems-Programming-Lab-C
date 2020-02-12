#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "set.h"
#include "info.h"
#include "input.h"

#define NUMBER_OF_SETS_PRINT 1 /*number of sets that are needed for print_set operand*/
#define NUMBER_OF_SETS_OTHER 1 /*number of sets that are needed for other operands*/

extern set SETA;
extern set SETB;
extern set SETC;
extern set SETD;
extern set SETE;
extern set SETF;
extern set SETF;

/*This is a dictionary that maches String to its corresponding operand*/
struct operand_dictionary{
	operands val;
	const char* str;
} conversionOP[] = {
	{read_set, "read_set"},
	{print_set, "print_set"},
	{union_set, "union_set"},
	{intersect_set, "intersect_set"},
	{sub_set, "sub_set"},
	{symdiff_set, "symdiff_set"},
	{stop,"stop"}
};

/*This is a dictionary that maches String to its corresponding set*/
struct set_dictionary{
	sets val;
	const char* str;
} conversionSET[] = {
	{SETA_e, "SETA"},
	{SETB_e, "SETB"},
	{SETC_e, "SETC"},
	{SETD_e, "SETD"},
	{SETE_e, "SETE"},
	{SETF_e, "SETF"}
};

/*returns an enum value using the operands dictionary*/
operands strTOenumOP(const char* str) {
	int i;
	for (i = 0; i < sizeof(conversionOP) / sizeof(conversionOP[0]); i++)
		if (str!=NULL&&!strcmp(str, conversionOP[i].str))
			return conversionOP[i].val;
	return error;
}

/*returns an enum value using the sets dictionary*/
sets strTOenumSET(const char* str) {
	int i;
	for (i = 0; i < sizeof(conversionSET) / sizeof(conversionSET[0]); i++){
		if(str==NULL){
			return NONE;
		}
		if (str!=NULL&&!strcmp(str, conversionSET[i].str))
			return conversionSET[i].val;
	}
	return undefined;
}

/*Handles the input from the user and calls to the appropriate function to perform the wanted operand.*/
void command_input(char* operand) {
	char* token;
	int* set;
	char* copy=(char*)malloc(strlen(operand));
	printf("\nEnter an operand:\n");
	operand = fgets(operand, MAX_LINE, stdin);
	if(!check_input(operand))
		return;
	strcpy(copy, operand);
	printf("%s\n",operand);
	token = strtok(copy, "\t\n, ");
	switch (strTOenumOP(token))
	{
	case read_set:
		set= (int*)set_input(operand, 0);
		read_set_f(set, read_set_numbers(operand));
		break;
	case print_set:
		print_set_f((int*)set_input(operand,0));
		break;
	case union_set:
		union_set_f((int*)set_input(operand,0), (int*)set_input(operand,1), (int*)set_input(operand,2));
		break;
	case intersect_set:
		intersect_set_f((int*)set_input(operand, 0), (int*)set_input(operand, 1), (int*)set_input(operand, 2));
		break;
	case sub_set:
		sub_set_f((int*)set_input(operand, 0), (int*)set_input(operand, 1), (int*)set_input(operand, 2));
		break;
	case symdiff_set:
		symdiff_set_f((int*)set_input(operand, 0), (int*)set_input(operand, 1), (int*)set_input(operand, 2));
		break;
	case stop:
		exit(0);
		break;
	default:
		printf("Undefined operand name\n");
	}
	free(copy);
}

/*Gets the user input and returns the sets that the operand should be performed on.*/
set* set_input(const char* operand, int setCounter) {
	char* token;
	int counter = 0;
	char* copy = (char*)malloc(strlen(operand));
	strcpy(copy, operand);
	token = strtok(copy, "\t\n, ");
	while (token!=NULL && (strTOenumSET(token)==NONE || setCounter >= counter)){
		token = strtok(NULL, "\t\n, ");
		if (strTOenumSET(token) != NONE) {
			counter++;
		}
	}
	switch (strTOenumSET(token))
	{
	case NONE:
		free(copy);
		printf("Missing set\n");
		return NULL;
	case SETA_e:
		free(copy);
		return &SETA;
	case SETB_e:
		free(copy);
		return &SETB;
	case SETC_e:
		free(copy);
		return &SETC;
	case SETD_e:	
		free(copy);
		return &SETD;
	case SETE_e:
		free(copy);
		return &SETE;
	case SETF_e:
		free(copy);
		return &SETF;
	default:
		free(copy);
		printf("Undefined set name\n");
		return NULL;
	}
	return NULL;
}

/*Gets the input from the user and returns a pointer to an arra of numbers for read_set_f to use*/
int* read_set_numbers(const char* operand) {
	int* p_array = (int*)malloc(strlen(operand));
	char* copy = (char*)malloc(strlen(operand));
	char* token;
	int i = 0;
	int count = 0;
	strcpy(copy,operand);
	token = strtok(copy, "\t\n, ");
	while (token != NULL) {
		if (count >= 2) {
			if (is_valid_number(token)) {
				*(p_array + i)= strtoul(token, NULL, 10);
				i++;
			}
			else
				return NULL;
		}
		token = strtok(NULL, "\t\n, ");
		if(p_array[i-1]==-1&&token!=NULL){
			printf("additional parameters after -1\n");
			return NULL;
		}
		count++;
	}
	if (*(p_array + i - 1) != -1) {
		printf("List of set members is not terminated correctly\n");
		return NULL;
	}
	free(copy);
	return p_array;
}

/*Returns TRUE(1) if the char @p points to is a valid integer (also between 0-127 or -1), FALSE(0) otherwise*/
int is_valid_number(char* p) {
	int i;
	int num = 0;
	int sign = 1;
	if (p[0] == '-') {
		p[0] = '0';
		sign = -1;
	}
	for (i = 0; i < strlen(p) && p[i] != '\0'; i++) {
		num *= 10;
		if (isdigit(p[i]))
			num += p[i] - '0';
		else {
			printf("Invalid set member - not an integer\n");
			return FALSE;
		}
	}
	if (sign == -1)
		p[0] = '-';
	if ((sign * num >= 0 && sign * num <= MAX_NUMBER) || sign * num == -1)
		return TRUE;
	printf("Invalid set member - value out of range\n");
	return FALSE;
}

/*Returns TRUE(1) if the input is correctly written (only checking commas and extraneous text), FALSE(0) otherwise*/
int check_input(const char* operand) {
	int i;
	int j;
	char* token;
	char* copy = (char*)malloc(strlen(operand));
	int commaflag=FALSE;
	int operandflag=FALSE;
	int spaceflag=FALSE;
	int flag=FALSE;
	strcpy(copy,operand);
	token=strtok(copy,"\t\n, ");
	switch (strTOenumOP(token)){
	case read_set:
		break;
	case print_set:
		if(!check_extraneous_text(token,NUMBER_OF_SETS_PRINT))
			return 0;
		break;
	case union_set:
		if(!check_extraneous_text(token,NUMBER_OF_SETS_OTHER))
			return 0;
		break;		
	case intersect_set:
		if(!check_extraneous_text(token,NUMBER_OF_SETS_OTHER))
			return 0;
		break;							
	case sub_set:
		if(!check_extraneous_text(token,NUMBER_OF_SETS_OTHER))
			return 0;
		break;			
	case symdiff_set:
		if(!check_extraneous_text(token,NUMBER_OF_SETS_OTHER))
			return 0;
		break;		
	case stop:
		free(copy);
		return TRUE;
	default:
		break;
	}
	free(copy);
	for(i=0;i<strlen(operand)&&operand[i]!=EOF&&operand[i]!='\n';i++) {
		if(isalnum(operand[i])&&flag==FALSE){
			flag=TRUE;
			operandflag=TRUE;
		}
		if(operandflag==TRUE){
			/*checks for comma after the operand:*/
			/*stop if operand isn't alphanumeric (or '_') and there was no space or it isn't alphanumeric (or '_') and we are going through spaces*/
			for(j=i;j<(strlen(operand))&&((isalnum(operand[j])&&spaceflag==FALSE)||(spaceflag==TRUE&&(operand[j]==' '||operand[j]=='\t'))||(operand[j]=='_')||(operand[j]==','));j++) {
				if(operand[j+1]==' '||operand[j+1]=='\t'){
					spaceflag=TRUE;
				}
				if(operand[j]==','){
					printf("Illegal comma\n");
					return FALSE;
				}
			}
			operandflag=FALSE;
		}
		/*checks for Multiple consecutive commas*/
		if(operand[i]==','){
			if(commaflag==TRUE){
				printf("Multiple consecutive commas\n");
				return FALSE;
			}
			commaflag=TRUE;
		}
		else if(operand[i]!=' '&&operand[i]!='\t')
			commaflag=FALSE;
	}
	/*is the command ends with a comma?*/
	if(commaflag==TRUE){
		printf("Illegal comma\n");
		return FALSE;
	}
	return TRUE;
}

/*returns TRUE(1) if there is no extraneous text after the command, FALSE(0) otherwise.*/
int check_extraneous_text(char* token, int parameters){
	int setcount=0;
	while(token!=NULL){
		token=strtok(NULL,"\t\n, ");
		if(setcount >= parameters && token!=NONE){
			printf("Extraneous text after end of command\n");
			return FALSE;
		}
		if(strTOenumSET(token)!=undefined&&strTOenumSET(token)!=NONE){				
			setcount++;			
		}
	}
	return TRUE;		
}