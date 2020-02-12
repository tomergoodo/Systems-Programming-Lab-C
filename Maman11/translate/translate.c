#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_LINE 120

void input(char String[]);
unsigned long int translate_dec_hex(char String[]);
void DecToBinary(unsigned long int  num);

int main()
{

	char String[MAX_LINE];
	printf("please enter a positive number => \n");
	input(String);
	printf("the number is: ");
	DecToBinary(translate_dec_hex(String));
	printf("\n");
	return 0;
}

/*The function translates the String into int using strtoul*/
unsigned long int translate_dec_hex(char String[])
{
	char** ptr = NULL;
	unsigned long int number = strtoul(String,ptr,10);

	return number;
}

/*The function scans a String from the user*/
void input(char String[])
{
	fgets(String, MAX_LINE, stdin);
	if (String[strlen(String) - 1]=='\n') {
		String[strlen(String) - 1] = '\0';
	}
}

/*The function prints a Decimal number in Binary*/
void DecToBinary(unsigned long int num) {

	if (num == 0) {
		printf("0");
		return;
	}
	DecToBinary(num / 2);
	printf("%lu",num % 2);
}
