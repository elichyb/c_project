assembler: main.o parser.o
	gcc -g -ansi main.o parser.o -Wall -pedantic -o assembler

main.o: main.c header.h
	gcc -c -g -ansi main.c -Wall -pedantic -o main.o	

parser.o: line_handler.c header.h
	gcc -c -g -ansi line_handler.c -Wall -pedantic -o parser.o 