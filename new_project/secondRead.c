/* Name: Elichy Barak ID: 301013462 */

/*
This file gets the data structures from the first file read, and convert them into bits.
*/

/* ======== Includes ======== */
#include "assmbler.h"

/* ====== Externs ====== */
/* Use the commands list from firstRead.c */
extern const command g_cmdArr[];

/* Use the data from firstRead.c */
extern labelInfo g_labelArr[MAX_LABELS_NUM];
extern int g_labelNum;
lineInfo *g_entryLines[MAX_LABELS_NUM];
extern int g_entryLabelsNum;
extern int g_dataArr[MAX_DATA_NUM];

/* ====== Methods ====== */

/* 
	GET- int IC.
	Updates the addresses of all the data labels in g_labelArr.
*/
void updateDataLabelsAddress(int IC)
{
	int i;

	/* Search in the array for label with isData flag */
	for (i = 0; i < g_labelNum; i++)
	{
		if (g_labelArr[i].isData)
		{
			/* Increase the adress */
			g_labelArr[i].address += IC;
		}
	}
}

/* 
	Description- Returns if there is an illegal entry line in g_entryLines. 
*/
void countIllegalEntries()
{
	int i;
	labelInfo *label;

	for (i = 0; i < g_entryLabelsNum; i++)
	{
		label = getLabel(g_entryLines[i]->lineStr);
		if (label)
		{
			if (label->isExtern)
			{
				printf("ERR:\tThe parameter for .entry can't be an external label.line: %d",g_entryLines[i]->lineNum);
				exit(0);
			}
		}
		else
		{
			printf("ERR:\tNo such label as \"%s\".line: %d", g_entryLines[i]->lineStr,g_entryLines[i]->lineNum);
			exit(0);
		}
	}

	/*  NO ERRORS  */
	return;
}

/* Returns a poinetr to a random not-external label from g_labelArr. */
labelInfo *getRandomLabel()
{
	int i, numOfNotExternal = 0, randomLabelNum;

	/* Count how many labes aren't external in labelArr */
	for (i = 0; i < g_labelNum; i++)
	{
		if (!g_labelArr[i].isExtern)
		{
			numOfNotExternal++;
		}
	}

	/* All the labels are external */
	if (numOfNotExternal == 0)
	{
		return NULL;
	}

	/* Get the randomInRange-th not external label in */
	for (i = 0; i < g_labelNum; i++)
	{
		if (!g_labelArr[i].isExtern && --randomLabelNum == 0)
		{
			return &g_labelArr[i];
		}
	}

	return NULL;
}

/* If the op is a label, this method is updating the value of it to be the address of the label. */
void updateLableOpAddress(operandInfo *op, int lineNum)
{
	if (op->type == LABEL)
	{
		labelInfo *label = getLabel(op->str);
		/* Check if op.str is a real label name */
		if (label == NULL)
		{
			/* Print errors (legal name is illegal or not exists yet) */
			printf("ERR:\tNo such label as \"%s\".line: %d", op->str, lineNum);
			exit(0);	
		}
		op->value = label->address;
	}
	/*  return mean label is legal  */
	return;
}

/* Returns the int value of a memory word. */
int getNumFromMemoryWord(memoryWord memory)
{
	/* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
	unsigned int mask = ~0;
	mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

	/* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
	return mask & ((memory.valueBits.value << 2) + memory.era);
}

/* Returns the id of the addressing method of the operand */
int getOpTypeId(operandInfo op)
{
	/* Check if the operand have legal type */
	if (op.type != INVALID)
	{
		/* NUMBER = 0, VAR = 1, DYMN = 2,REGISTER = 3 */
		return (int)op.type;
	}

	return 0;
}

/* Returns a memory word which represents the command in a line. */
memoryWord getCmdMemoryWord(lineInfo line)
{
	memoryWord memory = { 0 };

	/* Update all the bits in the command word */
	memory.era = (eraType)ABSOLUTE; /* All commands are absolute!!! */
	memory.valueBits.cmdBits.dest = getOpTypeId(line.op2);
	memory.valueBits.cmdBits.src = getOpTypeId(line.op1);
	memory.valueBits.cmdBits.opcode = line.cmd->opcode;
	memory.valueBits.cmdBits.group = line.cmd->numOfParams;
	memory.valueBits.cmdBits.stat = 5; /* always will be with value 101 (which is 5)  */
	return memory;
}

/* Returns a memory word which represents the operand (assuming it's a valid operand). */
memoryWord getOpMemoryWord(operandInfo op, bool isDest)
{
	memoryWord memory = { 0 };

	/* Check if it's a register or not */
	if (op.type == REGISTER)
	{
		memory.era = (eraType)ABSOLUTE; /* Registers are absolute */

		/* Check if it's the dest or src */
		if (isDest)
		{
			memory.valueBits.regBits.destBits = op.value;
		}
		else
		{
			memory.valueBits.regBits.srcBits = op.value;
		}
	}
	else
	{
		labelInfo *label = getLabel(op.str);

		/* Set era */	
		if (op.type == LABEL && label && label->isExtern)
		{
			memory.era = EXTENAL;
		}
		else
		{
			memory.era = (op.type == NUMBER) ? (eraType)ABSOLUTE : (eraType)RELOCATABLE;
		}

		memory.valueBits.value = op.value;
	}

	return memory;
}

/* Adds the value of memory word to the memoryArr, and increase the memory counter. */
void addWordToMemory(int *memoryArr, int *memoryCounter, memoryWord memory)
{
	/* Check if memoryArr isn't full yet */
	if (*memoryCounter < MAX_DATA_NUM)
	{
		/* Add the memory word and increase memoryCounter */
		memoryArr[(*memoryCounter)++] = getNumFromMemoryWord(memory);
	}
}

/* Adds a whole line into the memoryArr, and increase the memory counter. */
void addLineToMemory(int *memoryArr, int *memoryCounter, lineInfo *line)
{
	/* Don't do anything if the line is not a command line */
	if (line->cmd != NULL)
	{
		/* Update the label operands value */
		updateLableOpAddress(&line->op1, line->lineNum);
		updateLableOpAddress(&line->op2, line->lineNum);

		/* Add the command word to the memory */
		addWordToMemory(memoryArr, memoryCounter, getCmdMemoryWord(*line));

		if (line->op1.type == REGISTER && line->op2.type == REGISTER)
		{
			/* Create the memory word */
			memoryWord memory = { 0 };
			memory.era = (eraType)ABSOLUTE; /* Registers are absolute */
			memory.valueBits.regBits.destBits = line->op2.value;
			memory.valueBits.regBits.srcBits = line->op1.value;
			memory.valueBits.regBits.stat = 0; /*  will always be  0 */

			/* Add the memory to the memoryArr array */
			addWordToMemory(memoryArr, memoryCounter, memory);
		}
		else
		{
			/* Check if there is a source operand in this line */
			if (line->op1.type != INVALID)
			{
				/* Add the op1 word to the memory */
				line->op1.address = FIRST_ADDRESS + *memoryCounter;
				addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op1, FALSE));
				/* ^^ The FALSE param means it's not the 2nd op */
			}

			/*Check if there is a destination operand in this line */
			if (line->op2.type != INVALID)
			{
				/* Add the op2 word to the memory */
				line->op2.address = FIRST_ADDRESS + *memoryCounter;
				addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op2, TRUE));
				/* ^^ The TRUE param means it's the 2nd op */
			}
		}
	}
	return;
}

/* Adds the data from g_dataArr to the end of memoryArr. */
void addDataToMemory(int *memoryArr, int *memoryCounter, int DC)
{
	int i;
	/* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
	unsigned int mask = ~0;
	mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

	/* Add each int from g_dataArr to the end of memoryArr */
	for (i = 0; i < DC; i++)
	{
		if (*memoryCounter < MAX_DATA_NUM)
		{
			/* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
			memoryArr[(*memoryCounter)++] = mask & g_dataArr[i];
		}
		else
		{
			/* No more space in memoryArr */
			return;
		}
	}
}

/* Reads the data from the first read for the second time. */
/* It converts all the lines into the memory. */
void secondFileRead(int *memoryArr, lineInfo *linesArr, int lineNum, int IC, int DC)
{
	int memoryCounter = 0, i;

	/* Update the data labels */
	updateDataLabelsAddress(IC);

	/* Check if there are illegal entries */
	countIllegalEntries();

	/* Add each line in linesArr to the memoryArr */
	for (i = 0; i < lineNum; i++)
	{
		addLineToMemory(memoryArr, &memoryCounter, &linesArr[i]);
	}

	/* Add the data from g_dataArr to the end of memoryArr */
	addDataToMemory(memoryArr, &memoryCounter, DC);

	return ;
}