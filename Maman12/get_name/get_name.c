/*The program gets a list of 30 names from an input and prints 10 random names from the list*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define NAMES 30 /*size of the name list*/
#define MAX_LINE 21 /*max size of each name in the list*/
#define RANDOM_LOOP 10 /*number of random names to be chosen*/
#define FALSE 0
#define TRUE 1


char* get_name();
void scan_names();
int check_duplicate(char* name,int j);
int check_same(char* name1, char* name2);
void free_name();

char* names[NAMES];

int main() {
	char *name;
	int i;
	scan_names();
	srand(time(0)); /*determines the seed for the rand function*/
	printf("These are the 10 chosen names by random:\n");
	for(i=0;i<RANDOM_LOOP;i++){
		name=get_name();
		printf("%s\n",name);
	}
	free_name(); /*deallocate the memory*/
	return 0;
}


/*returns a random name from the list*/
char* get_name() {
	char *str;
	int random;
	random=rand();
	random %= NAMES;
	str=names[random];
	return str;
}

/*scans 30 names from stdin, checks that there are no duplicates and prints the list*/
void scan_names() {
	int i = 0;

	while (i < NAMES) {
		names[i] =(char *) malloc(MAX_LINE);
		printf("Enter the #%d name\n",i);
		fgets(names[i], MAX_LINE, stdin);
		if(names[i][0]=='\n')
			fgets(names[i], MAX_LINE, stdin);
		if ((names[i][strlen(names[i]) - 1]) == '\n') {
			names[i][strlen(names[i]) - 1] = '\0';
		}
		if (!check_duplicate(names[i],i)) {
			printf("ERROR: The name %s was scanned more than once.\n", names[i]);
			exit(0);
		}
		i++;
	}
	i = 0;
	printf("\nThis is the list of names:\n");
	while (i < NAMES) {
		printf("%s\n", names[i]);
		i++;
	}
	printf("\n\n");
}

/*returns 0 if there is a duplicate of @param name, 1 otherwise*/
int check_duplicate(char* name,int j) {
	int i;
	for (i = 0; i < j; i++) {
		if (strlen(name) == strlen(names[i])) {
			if (check_same(name, names[i]))
				return FALSE;
		}
	}
	return TRUE;
}

/*compares name1 and name2 (does not recognize uppercase and lowercase letters)*/
int check_same(char* name1, char* name2) {
	int j, flag = 1;
	for (j = 0; j < strlen(name1) && flag; j++) {
		if (tolower(name1[j]) != tolower(name2[j])) {
			flag = 0;
		}
	}
	if (j == strlen(name1))
		return TRUE;
	return FALSE;
}

/*deallocates the memory*/
void free_name(){
	int i;
	for(i=0;i<NAMES;i++){
		free(names[i]);	
	}
}
