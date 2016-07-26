/*
	Name: Elichy Barak ID: 301013462
*/

#include <stdio.h>
#include <stdlib.h>
include "header.h"
void parser_file(char *);

/*
-----------------------------------------------------
Remove older files:
DESCRIPTION- will remove *.ent *.ext and *.ob files from the current loction
-----------------------------------------------------
*/
void removeTempFiles()
{

	char temp[FILENAME_MAX];
	strcpy(temp, fileName1);
	strcat(temp, ".ex");
	remove(temp);
	strcpy(temp, fileName1);
	strcat(temp, ".en");
	remove(temp);
}


/*
------------------------------------------------------
	MAIN- will control on the flow of this project
------------------------------------------------------	
*/
int main(int argc, char *argv[])
{
	/*
		step1: check if we give an argument when we run this (need to give location of file)
	*/
	if (argc < 2) {
	 	fprintf(stderr, "Must the location of the asembler file you ant to parse!\n");
	  	exit(0);
	}
	/*
		step2: send the file into the parser_file function
		(this will start to parse the file line after line)
	*/
	parser_file(argv[1]);
	return (0);
}

/*
----------------------------------------
GET- char pointer to start of file.
DESCRIPTION- will parse the file line by line.
RETURN- nothing
----------------------------------------
*/
void parser_file(char *ptr_file)
{
	FILE *file_asmbl;
  	char buff[100];
	file_asmbl = fopen(ptr_file, "r");
	if (!file_asmbl)
	{
		printf("Can't read file.\n");
		exit(1);
	}
	while(fgets(buff,100,file_asmbl))
	{
		/*print line on the screen*/
		puts(buff);
	}
	fclose(file_asmbl);
}