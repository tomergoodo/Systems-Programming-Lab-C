#include <stdio.h>
#include <string.h>


#define MAX_LINE 120


char input(char String[],char c);
int suffix(char str[], char c);

int main()
{
	char String[MAX_LINE];
	char c=' ';
	printf("please enter Character and a String with a space between => \n");
	suffix(String,input(String, c));
	printf("\n");
	return 0;
}

/*The function scans a String and a char from the user and returns the char*/
char input(char String[],char c)
{
	c = getchar();
	fgets(String, MAX_LINE, stdin);
	if (String[strlen(String) - 1] == '\n') {
		String[strlen(String) - 1] = '\0';
	}
	return c;
}

/*The function gets a String and a Character, returns the number of suffixes of the char in the String
and prints those suffixes.*/
int suffix(char str[], char c) {
	int i, j, count = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == c) {
			count++;
			for (j = i; str[j] != '\0'; j++) {
				printf("%c",str[j]);
			}
			printf("\n");
		}
	}
	printf("number of suffixes is: %d",count);
	return count;
}
