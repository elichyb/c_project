/* Name: Elichy Barak ID: 301013462*/

#include <stdio.h>
#include <stdlib.h>
#include "assmbler.h"

void parseFile(char *fileName)
{
 	FILE *file = fopen(fileName, "r");
	lineInfo linesArr[MAX_LINES_NUM];
	int /*memoryArr[MAX_DATA_NUM] = { 0 },*/ IC = 0, DC = 0, linesFound = 0;

	/* Open File */
	if (file == NULL)
	{
		printf("ERR:\tCan't open the file \"%s\".\n", fileName);
		exit(0);
	}
	printf("INFO:\tSuccessfully opened the file \"%s\".\n", fileName);

	/* First Read */
	firstFileRead(file, linesArr, &linesFound, &IC, &DC);
	
	/* Second Read */
	/*secondFileRead(memoryArr, linesArr, linesFound, IC, DC);*/

	/* Create Output Files */
	/*if (numOfErrors == 0)
	{*/
		/* Create all the output files */
	/*	createObjectFile(fileName, IC, DC, memoryArr);
		createExternFile(fileName, linesArr, linesFound); 
		createEntriesFile(fileName);
		printf("[Info] Created output files for the file \"%s.as\".\n", fileName);
	}
	else
	{*/
		/* print the number of errors. */
	/*	printf("[Info] A total of %d error%s found throughout \"%s.as\".\n", numOfErrors, (numOfErrors > 1) ? "s were" : " was", fileName);
	}*/

	/* Free all malloc pointers, and reset the globals. */
	/*clearData(linesArr, linesFound, IC + DC);*/

	/* Close File */
	/*fclose(file);*/
	return;
}

/*
------------------------------------------------------
	MAIN- will control on the flow of this project
------------------------------------------------------	
*/
int main(int argc, char *argv[])
{
	/*
		step2: check if we give an argument when we run this (need to give location of file)
	*/
	if (argc < 2) 
	{
	 	fprintf(stderr, "Must the location of the asembler file that you want to parse!\n");
	  	exit(0);
	}
	/*
		step3: send the file into the parser_file function
		(this will start to parse the file line after line)
	*/
	parseFile(argv[1]);
	return (0);
}