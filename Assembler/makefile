assembler: main.o dictionaries.o first_pass.o label_table.o second_pass.o utility.o
	gcc -g main.o dictionaries.o first_pass.o label_table.o second_pass.o utility.o -o assembler
main.o: main.c
	gcc -c -g main.c -o main.o
dictionaries.o: dictionaries.c
	gcc -c -g dictionaries.c -o dictionaries.o
first_pass.o: first_pass.c
	gcc -c -g first_pass.c -o first_pass.o
label_table.o: label_table.c
	gcc -c -g label_table.c -o label_table.o
second_pass.o: second_pass.c
	gcc -c -g second_pass.c -o second_pass.o
utility.o: utility.c
	gcc -c -g utility.c -o utility.o