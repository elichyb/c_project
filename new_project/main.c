/* Name: Elichy Barak ID: 301013462*/

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

/* enum to help convert to spacial 8*/
/* 
	Description- Puts in the given buffer a base special 8 representation of num. 

*/
int intToBaseSpecial8(int num, char *buf)
{
	const int base = 8; /*  */
	int index = 0;
	while(base != 0)
	{
		switch(base%8)
		{
			case(one)
		}
		index++;
	}
	return 0;
}

/* 
	Description-prints a number in base special 8 in the file. 
	GET-
		file- the output file in the end.
		num- the number that i want to convert to special 8.
*/
void fprintfBaseSpecail8(FILE *file, int num)
{
	int numOfZeros, i;
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
		fprintfBaseSpecail8(file, FIRST_ADDRESS + i, 3);
		fprintf(file, "\t");
		fprintfBaseSpecail8(file, memoryArr[i], 3);
	}
	fclose(file);
}

/* Creates the .ent file, which contains the addresses for the .entry labels in base 32. */
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

	for (i = 0; i < g_entryLabelsNum; i++)
	{
		fprintf(file, "%s\t", g_entryLines[i]->lineStr);
		/*fprintfBaseSpecail8(file, getLabel(g_entryLines[i]->lineStr)->address, 1);*/

		if (i != g_entryLabelsNum - 1)
		{
			fprintf(file, "\n");
		}
	}

	fclose(file);
}

/* Creates the .ext file, which contains the addresses for the extern labels operands in base 32. */
void createExternFile(lineInfo *linesArr, int linesFound)
{
	int i;
	labelInfo *label;
	bool firstPrint = TRUE; /* This bool meant to prevent the creation of the file if there aren't any externs */
	FILE *file = NULL;

	for (i = 0; i < linesFound; i++)
	{
		/* Check if the 1st operand is extern label, and print it. */
		if (linesArr[i].cmd && linesArr[i].cmd->numOfParams >= 2 && linesArr[i].op1.type == LABEL)
		{
			label = getLabel(linesArr[i].op1.str);
			if (label && label->isExtern)
			{
				if (firstPrint)
				{
					/* Create the file only if there is at least 1 extern */
					file = fopen("ext.ext", "w");
				}
				else
				{
					fprintf(file, "\n");
				}

				fprintf(file, "%s\t", label->name);
				/*fprintfBaseSpecail8(file, linesArr[i].op1.address, 1);*/
				firstPrint = FALSE;
			}
		}

		/* Check if the 2nd operand is extern label, and print it. */
		if (linesArr[i].cmd && linesArr[i].cmd->numOfParams >= 1 && linesArr[i].op2.type == LABEL)
		{
			label = getLabel(linesArr[i].op2.str);
			if (label && label->isExtern)
			{
				if (firstPrint)
				{
					/* Create the file only if there is at least 1 extern */
					file = fopen("prog.ext", "w");
				}
				else
				{
					fprintf(file, "\n");
				}

				fprintf(file, "%s\t", label->name);
				/*fprintfBaseSpecail8(file, linesArr[i].op2.address, 1);*/
				firstPrint = FALSE;
			}
		}
	}

	if (file)
	{
		fclose(file);
	}
}

/* Resets all the globals and free all the malloc blocks. */
void clearData(lineInfo *linesArr, int linesFound, int dataCount)
{
	int i;

	/* --- Reset Globals --- */

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
	FILE *file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("ERR:\tCan't open the file \"%s\".\n", fileName);
		exit(0);
	}
	printf("INFO:\tSuccessfully opened the file \"%s\".\n", fileName);

	/*  step 3: First read!  */
	firstFileRead(file, linesArr, &linesFound, &IC, &DC);
	
	/*  step 4: seconde read!  */
	econdFileRead(memoryArr, linesArr, linesFound, IC, DC);

	/*  step 5: Create Output Files  */
	createObjectFile(IC, DC, memoryArr);
	createExternFile(linesArr, linesFound); 
	createEntriesFile();
	printf("[Info] Created output files for the file \"%s.as\".\n", fileName);

	/*  step 6: Free all malloc pointers, and reset the globals.  */
	clearData(linesArr, linesFound, IC + DC);

	/*  step 7: Close File  */
	fclose(file);
	return (0);
}