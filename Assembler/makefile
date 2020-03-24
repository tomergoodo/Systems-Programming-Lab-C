assembler: main.o dictionaries.o first_pass.o label_table.o second_pass.o utility.o
	gcc -ansi -Wall -pedantic -g main.o dictionaries.o first_pass.o label_table.o second_pass.o utility.o -o assembler
dictionaries.o: dictionaries.c
	gcc -ansi -Wall -pedantic -c -g dictionaries.c -o dictionaries.o
first_pass.o: first_pass.c
	gcc -ansi -Wall -pedantic -c -g first_pass.c -o first_pass.o
label_table.o: label_table.c
	gcc -ansi -Wall -pedantic -c -g label_table.c -o label_table.o
second_pass.o: second_pass.c
	gcc -ansi -Wall -pedantic -c -g second_pass.c -o second_pass.o
utility.o: utility.c
	gcc -ansi -Wall -pedantic -c -g utility.c -o utility.o
main.o: main.c
	gcc -ansi -Wall -pedantic -c -g main.c -o main.o