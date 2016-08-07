/* Name: Elichy Barak ID: 301013462*/
/*  
	If you want to see project progress you can clone it from:
	https://github.com/elichyb/c_project
*/
#include <stdio.h>
#include <stdlib.h>
#include "assmbler.h"

/* ====== Global Data Structures ====== */
/* Labels */
labelInfo g_labelArr[MAX_LABELS_NUM]; 
int g_labelNum = 0;
/* Entry Lines */
lineInfo *g_entryLines[MAX_LABELS_NUM]; /**/
int g_entryLabelsNum = 0;
/* Data */
int g_dataArr[MAX_DATA_NUM];

bool ERROR = FALSE;/*  this will check if there is any errors  */
/* 
	Description- Puts in the given buffer a base special 8 representation of num.
	GET-
		num: int number to convert.
		buf: char array to put the converted number in the new special 8 base!.
*/
void intToBaseSpecial8(int num, char *buf)
{
	const int base = 8; /*  */
	int index = 0;
	while(num != 0)
	{
		if(index > 6)
		{
			printf("There is somthing wrong here\n");
			break;
		}
		switch(num%8)
		{
			case(0):
				buf[index] = '!';
				break;
			case(1):
				buf[index] = '@';
				break;
			case(2):
				buf[index] = '#';
				break;
			case(3):
				buf[index] = '$';
				break;
			case(4):
				buf[index] = '%';
				break;
			case(5):
				buf[index] = '^';
				break;
			case(6):
				buf[index] = '&';
				break;
			case(7):
				buf[index] = '*';
				break;
			default:
				printf("no such char need to check whats wrong here.\n");
				ERROR = TRUE;

		}
		num = num / base;
		index++;
	}
	return ;
}

/* 
	Description-prints a number in base special 8 in the file. 
	GET-
		file- the output file in the end.
		num- the number that i want to convert to special 8.
*/
void fprintfBaseSpecail8(FILE *file, int num)
{
	/* 2^15 = 8 ^ 5, So 3 chars are enough to represent 15 bits in base specail 8, and the last char is \0. */
	char buf[6] = { 0 }; 
	intToBaseSpecial8(num, buf);
	fprintf(file, "%s", buf);
}

/* 
	Description- Creates the .obj file, which contains the assembled lines in base "specail 8". 
	GET- 
		memoryArr- array of integers that parsed form the input file.
		(IC, DC).
*/
void createObjectFile(int IC, int DC, int *memoryArr)
{
	int i;
	FILE *file;
	file = fopen("prog.ob", "w");

	/* Print IC and DC */
	fprintfBaseSpecail8(file, IC);
	fprintf(file, "\t");
	fprintfBaseSpecail8(file, DC);

	/* Print all of memoryArr */
	for (i = 0; i < IC + DC; i++)
	{
		fprintf(file, "\n");
		fprintfBaseSpecail8(file, FIRST_ADDRESS + i);
		fprintf(file, "\t");
		fprintfBaseSpecail8(file, memoryArr[i]);
	}
	fprintf(file,"\n");
	fclose(file);
}

/* 
	Description- Creates the .ent file, which contains the addresses for the .entry labels in base special 8. 
*/
void createEntriesFile()
{
	int i;
	FILE *file;

	/* Don't create the entries file if there aren't entry lines */
	if (!g_entryLabelsNum)
	{
		return;
	}

	file = fopen("prog.ent", "w");
	if(!file)
	{
		printf("couldn't create .ent file.\n");
	}

	for (i = 0; i < g_entryLabelsNum; i++)
	{
		fprintf(file, "%s\t", g_entryLines[i]->lineStr); /*print the label name into the .ent file*/
		fprintfBaseSpecail8(file, getLabel(g_entryLines[i]->lineStr)->address); /* print address next to it */

		if (i != g_entryLabelsNum - 1)
		{
			fprintf(file, "\n");
		}
	}
	fprintf(file,"\n");
	fclose(file);
}

/* 
	Description- Creates the .ext file, which contains the addresses for the extern labels operands in base special 8. 
	GET-
		lineArr: parsed line array that in the input file.
		lineFound: number of lines that there is in the input file.
*/
void createExternFile(lineInfo *linesArr, int linesFound)
{
	int i;
	labelInfo *label;
	bool firstPrint = TRUE; /* This bool meant to prevent the creation of the file if there aren't any externs */
	FILE *file = NULL;

	file = fopen("prog.ext", "w");
	if(!file)
		printf("Unable to create a file.\n");

	for (i = 0; i < linesFound; i++)
	{
		if (!strcmp(linesArr[i].commandStr, "extern"))
		{
			label = getLabel(linesArr[i].lineStr);
			if(label->name)
			{
				fprintf(file, "%s\t", label->name);
				fprintfBaseSpecail8(file, linesArr[i].address);
			}
			else
			{
				printf("There isn't any ext label?\n");
			}
		}
	}

	if (file)
	{
		fprintf(file,"\n");
		fclose(file);
	}

	return;
}

/* 
	Description- Resets all the globals and free all the malloc blocks.
	GET- 
		lineArss- array that inculdes lines information.
		lineFound- the number of lines in the file.
		dataCount- how many vars allocatino are holds.
*/
void clearData(lineInfo *linesArr, int linesFound, int dataCount)
{
	int i;
	/* Reset global labels */
	for (i = 0; i < g_labelNum; i++)
	{
		g_labelArr[i].address = 0;
		g_labelArr[i].isData = 0;
		g_labelArr[i].isExtern = 0;
	}
	g_labelNum = 0;

	/* Reset global entry lines */
	for (i = 0; i < g_entryLabelsNum; i++)
	{
		g_entryLines[i] = NULL;
	}
	g_entryLabelsNum = 0;

	/* Reset global data */
	for (i = 0; i < dataCount; i++)
	{
		g_dataArr[i] = 0;
	}

	/* Free malloc blocks */
	for (i = 0; i < linesFound; i++)
	{
		free(linesArr[i].originalString);
	}
}

/*
------------------------------------------------------
	MAIN- will control on the flow of this project
------------------------------------------------------	
*/
int main(int argc, char *argv[])
{
	/*  Declaration vars  */
	lineInfo linesArr[MAX_LINES_NUM];
	int memoryArr[MAX_DATA_NUM] = { 0 }, IC = 0, DC = 0, linesFound = 0;
	/*
		step1: check if we give an argument when we run this (need to give location of file)
	*/
	if (argc < 2) 
	{
	 	fprintf(stderr, "Must the location of the asembler file that you want to parse!\n");
	  	exit(0);
	}
	/*  step 2: open the file if we can  */
	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("ERR:\tCan't open the file \"%s\".\n", argv[1]);
		exit(0);
	}
	printf("INFO:\tSuccessfully opened the file \"%s\".\n", argv[1]);

	/*  step 3: First read!  */
	firstFileRead(file, linesArr, &linesFound, &IC, &DC);
	
	/*  step 4: seconde read!  */
	secondFileRead(memoryArr, linesArr, linesFound, IC, DC);

	if(ERROR == FALSE)
	{
		/*  step 5: Create Output Files  */
		createObjectFile(IC, DC, memoryArr);
		createExternFile(linesArr, linesFound); 
		createEntriesFile();
		printf("[Info] Created output files for the file \"%s\".\n", argv[1]);
	}
	else
	{
		printf("ERR:\t Some line are found with errors.\n");
	}
	/*  step 6: Free all malloc pointers, and reset the globals.  */
	clearData(linesArr, linesFound, IC + DC);

	/*  step 7: Close File  */
	fclose(file);
	return (0);
}