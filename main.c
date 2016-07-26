/*
	Name: Elichy Barak ID: 301013462
*/
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

/*
------------------------------------------------------
	MAIN- will control on the flow of this project
------------------------------------------------------	
*/
int main(int argc, char *argv[])
{
	/*
		step1: remove older files
	*/
	removeOldFiles();

	/*
		step2: check if we give an argument when we run this (need to give location of file)
	*/
	if (argc < 2) {
	 	fprintf(stderr, "Must the location of the asembler file that you want to parse!\n");
	  	exit(0);
	}
	/*
		step3: send the file into the parser_file function
		(this will start to parse the file line after line)
	*/
	parser_file(argv[1]);
	return (0);
}

/*
-----------------------------------------------------
Remove older files:
DESCRIPTION- will remove output.ent output.ext and output.ob files from the current loction
-----------------------------------------------------
*/
void removeOldFiles()
{
	char *file_name_ob = "output.ob";
	char *file_name_ext = "output.ext";
	char *file_name_ent = "output.ent";
	/*  delete output.ob  */
	if((remove(file_name_ob)) == 0)
	{
		printf("File deleted\n");
	}
	else
	{
		printf("Unable to delete file\n");
	}

	/*  delete output.ext  */
	if((remove(file_name_ext)) == 0)
	{
		printf("File deleted\n");
	}
	else
	{
		printf("Unable to delete file\n");
	}

	/*delete output.ent*/
	if((remove(file_name_ent)) == 0)
	{
		printf("File deleted\n");
	}
	else
	{
		printf("Unable to delete file\n");
	}
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
		/*  send to line_parser the line from the file to parse it  */
		line_parser(&buff);
	}
	fclose(file_asmbl);
}