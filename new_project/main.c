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

/* Puts in the given buffer a base 32 representation of num. */
int intToBaseSpecial8(int num, char *buf, int index)
{
	const int base = 8;
	const char digits[] = "!@#$%^&*";

	if (num)
	{		
		/*index = intToBase32(num / base, buf, index);*/
		buf[index] = digits[num % base];
		return ++index;
	}

	return 0;
}

void printStrWithZeros(char *str, int strMinWidth)
{
	int i, numOfZeros = strMinWidth - strlen(str);

	for (i = 0; i < numOfZeros; i++)
	{
		printf("0");
	}
	printf("%s", str);
}

/* Prints a number in base 32 in the file. */
void fprintfBase32(FILE *file, int num, int strMinWidth)
{
	int numOfZeros, i;
	/* 2^15 = 32 ^ 3, So 3 chars are enough to represent 15 bits in base 32, and the last char is \0. */
	char buf[4] = { 0 }; 

	/*intToBase32(num, buf, 0);*/

	/* Add zeros first, to make the length at least strMinWidth */
	numOfZeros = strMinWidth - strlen(buf);
	for (i = 0; i < numOfZeros; i++)
	{
		fprintf(file, "0");
	}
	fprintf(file, "%s", buf);
}

/* Creates the .obj file, which contains the assembled lines in base "specail 8". */
void createObjectFile(char *name, int IC, int DC, int *memoryArr)
{
	int i;
	FILE *file;
	file = fopen("prog.ob", "w");

	/* Print IC and DC */
	/*fprintfBase32(file, IC, 1);*/
	fprintf(file, "\t\t");
	/*fprintfBase32(file, DC, 1);*/

	/* Print all of memoryArr */
	for (i = 0; i < IC + DC; i++)
	{
		fprintf(file, "\n");
		/*fprintfBase32(file, FIRST_ADDRESS + i, 3);*/
		fprintf(file, "\t\t");
		/*fprintfBase32(file, memoryArr[i], 3);*/
	}

	fclose(file);
}

/* Creates the .ent file, which contains the addresses for the .entry labels in base 32. */
void createEntriesFile(char *name)
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
		fprintf(file, "%s\t\t", g_entryLines[i]->lineStr);
		/*fprintfBase32(file, getLabel(g_entryLines[i]->lineStr)->address, 1);*/

		if (i != g_entryLabelsNum - 1)
		{
			fprintf(file, "\n");
		}
	}

	fclose(file);
}

/* Creates the .ext file, which contains the addresses for the extern labels operands in base 32. */
void createExternFile(char *name, lineInfo *linesArr, int linesFound)
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

				fprintf(file, "%s\t\t", label->name);
				/*fprintfBase32(file, linesArr[i].op1.address, 1);*/
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

				fprintf(file, "%s\t\t", label->name);
				/*fprintfBase32(file, linesArr[i].op2.address, 1);*/
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

void parseFile(char *fileName)
{
 	FILE *file = fopen(fileName, "r");
	lineInfo linesArr[MAX_LINES_NUM];
	int memoryArr[MAX_DATA_NUM] = { 0 }, IC = 0, DC = 0, linesFound = 0;

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
	secondFileRead(memoryArr, linesArr, linesFound, IC, DC);

	printf("%d\n", memoryArr[0]);
	/* Create Output Files */
	/*if (numOfErrors == 0)
	{*/
		/* Create all the output files */
		/*createObjectFile(fileName, IC, DC, memoryArr);
		createExternFile(fileName, linesArr, linesFound); 
		createEntriesFile(fileName);
		printf("[Info] Created output files for the file \"%s.as\".\n", fileName);
	}*/

	/* Free all malloc pointers, and reset the globals. */
	clearData(linesArr, linesFound, IC + DC);

	/* Close File */
	fclose(file);
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