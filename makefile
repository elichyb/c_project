assembler: main.c parser.o header.h 
	gcc -g -ansi main.c -Wall -pedantic -o assembler

main.o: main.c header.h
	gcc -c -g -ansi main.c -Wall -pedantic -o main.o	

parser.o: line_handler.c header.h
	gcc -c -g -ansi line_handler.c -Wall -pedantic -o parser.o 