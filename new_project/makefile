assembler: main.o parser.o assmbler.h
	gcc -g -ansi main.o parser.o -Wall -pedantic -o assembler

main.o: main.c assmbler.h
	gcc -c -g -ansi main.c -Wall -pedantic -o main.o	

parser.o: firstRead.c assmbler.h
	gcc -c -g -ansi firstRead.c -Wall -pedantic -o parser.o 
