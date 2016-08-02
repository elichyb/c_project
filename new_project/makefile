assembler: main.o parser1.o parser2.o utility.o assmbler.h 
	gcc -g -ansi main.o parser1.o parser2.o utility.o -Wall -pedantic -o assembler

main.o: main.c assmbler.h
	gcc -c -g -ansi main.c -Wall -pedantic -o main.o	

parser1.o: firstRead.c assmbler.h
	gcc -c -g -ansi firstRead.c -Wall -pedantic -o parser1.o 

parser2.o: secondRead.c assmbler.h
	gcc -c -g -ansi secondRead.c -Wall -pedantic -o parser2.o


utility.o: utility.c assmbler.h
	gcc -c -g -ansi utility.c -Wall -pedantic -o utility.o