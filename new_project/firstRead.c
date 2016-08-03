/*Name: Elichy Barak ID: 301013462*/

/*
This file parses a specific assembly language.
It saves the data from an assembly file in data structures, and finds the errors. 
*/

#include "assmbler.h"
#include <ctype.h>
#include <stdlib.h>

/* ====== Directives List ====== */
void parseDataDirc(lineInfo *line, int *IC, int *DC);
void parseStringDirc(lineInfo *line, int *IC, int *DC);
void parseExternDirc(lineInfo *line);
void parseEntryDirc(lineInfo *line);

const directive g_dircArr[] = 
{	/* Name | Parseing Function */
	{ "data", parseDataDirc } ,
	{ "string", parseStringDirc } ,
	{ "extern", parseExternDirc },
	{ "entry", parseEntryDirc },
	{ NULL } /* represent the end of the array */
};	

/* ====== Commands List ====== */
const command g_cmdArr[] =	
{	/* Name | Opcode | NumOfParams */
	{ "mov", 0, 2 } , 
	{ "cmp", 1, 2 } ,
	{ "add", 2, 2 } ,
	{ "sub", 3, 2 } ,
	{ "not", 4, 1 } ,
	{ "clr", 5, 1 } ,
	{ "lea", 6, 2 } ,
	{ "inc", 7, 1 } ,
	{ "dec", 8, 1 } ,
	{ "jmp", 9, 1 } ,
	{ "bne", 10, 1 } ,
	{ "red", 11, 1 } ,
	{ "prn", 12, 1 } ,
	{ "jsr", 13, 1 } ,
	{ "rst", 14, 0 } ,
	{ "stop", 15, 0 } ,
	{ NULL } /* represent the end of the array */
}; 

/* ====== Externs ====== */
extern labelInfo g_labelArr[MAX_LABELS_NUM];
extern int g_labelNum;
lineInfo *g_entryLines[MAX_LABELS_NUM];
extern int g_entryLabelsNum;
extern int g_dataArr[MAX_DATA_NUM];

/* ====== Methods ====== */

/* Adds the label to the labelArr and increases labelNum. Returns a pointer to the label in the array. */
labelInfo *addLabelToArr(labelInfo label, lineInfo *line)
{
	/* Check if label is legal */
	if (!isLegalLabel(line->lineStr, line->lineNum, TRUE))
	{
		/* Illegal label name */
		printf("ERR:\tNot legal label %d",line->lineNum);
		exit(0);
	}

	/* Check if label is legal */
	if (isExistingLabel(line->lineStr))
	{
		printf("ERR:\tLabel already exists line: %d",line->lineNum);
		exit(0);
	}

	/* Add the name to the label */
	strcpy(label.name, line->lineStr);

	/* Add the label to g_labelArr and to the lineInfo */
	if (g_labelNum < MAX_LABELS_NUM)
	{
		g_labelArr[g_labelNum] = label;
		return &g_labelArr[g_labelNum++];
	}

	/* Too many labels */
	printf("ERR:\tToo many labels - max is %d.", MAX_LABELS_NUM);
	exit(0);
}

/* Adds the number to the g_dataArr and increases DC. Returns if it succeeded. */
bool addNumberToData(int num, int *IC, int *DC, int lineNum)
{
	/* Check if there is enough space in g_dataArr for the data */
	if (*DC + *IC < MAX_DATA_NUM)
	{
		g_dataArr[(*DC)++] = num;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

/* Adds the str to the g_dataArr and increases DC. Returns if it succeeded. */
bool addStringToData(char *str, int *IC, int *DC, int lineNum)
{
	do
	{
		if (!addNumberToData((int)*str, IC, DC, lineNum))
		{
			return FALSE;
		}
	} while (*str++);

	return TRUE;
}

/*
	Description- Finds the label in line->lineStr and add it to the label list. 
				 Returns a pointer to the next char after the label, 
				 or NULL is there isn't a legal label. 
*/
char *findLabel(lineInfo *line, int IC)
{
	char *labelEnd = strchr(line->lineStr, ':');
	labelInfo label = { 0 };
	label.address = FIRST_ADDRESS + IC;

	/* Find the label (or return NULL if there isn't) */
	if (!labelEnd)
	{
		return NULL;
	}
	*labelEnd = '\0';

	/* Check if the ':' came after the first word */
	if (!isOneWord(line->lineStr))
	{
		*labelEnd = ':'; /* Fix the change in line->lineStr */
		return NULL;
	}

	/* Check of the label is legal and add it to the labelList */
	line->label = addLabelToArr(label, line);
	printf("%s\n", line->label);
	return labelEnd + 1; /* +1 to make it point at the next char after the \0 */
}

/* 
	Description- Omits the last label in labelArr by updating g_labelNum.
				 Used to remove the label from a "entry/extern" line.
	GET- lineNum: line number. 
*/
void removeLastLabel(int lineNum)
{
	g_labelNum--;
	printf("WARN: At line %d: The assembler ignored the label before the directive.\n", lineNum);
}

/* Parses a .data directive. */
void parseDataDirc(lineInfo *line, int *IC, int *DC)
{
	char *operandTok = line->lineStr, *endOfOp = line->lineStr;
	int operandValue;
	bool foundComma;

	/* Make the label a data label (is there is one) */
	if (line->label)
	{
		line->label->isData = TRUE;
		line->label->address = FIRST_ADDRESS + *DC;
	}

	/* Check if there are params */
	if (isWhiteSpaces(line->lineStr))
	{
		/* No parameters */
		printf("ERR:\tNo parameter. line: %d",line->lineNum);
		exit(0);
	}

	/* Find all the params and add them to g_dataArr */
	FOREVER
	{
		/* Get next param or break if there isn't */
		if (isWhiteSpaces(line->lineStr))
		{
			break;
		}
		operandTok = getFirstOperand(line->lineStr, &endOfOp, &foundComma);

		/* Add the param to g_dataArr */
		if (isLegalNum(operandTok, MEMORY_WORD_LENGTH, line->lineNum, &operandValue))
		{
			if (!addNumberToData(operandValue, IC, DC, line->lineNum))
			{
				/* Not enough memory */
				printf("ERR:\t Not enough memory\n");
				exit(0);
			}
		}
		else
		{
			/* Illegal number */
			printf("ERR:\t Illegal number. line: %d\n",line->lineNum);
			exit(0);
		}

		/* Change the line to start after the parameter */
		line->lineStr = endOfOp;
	}

	if (foundComma)
	{
		/* Comma after the last param */
		printf("ERR:\tDo not write a comma after the last parameter. line: %d",line->lineNum);
		exit(0);
	}
}

/* Parses a .string directive. */
void parseStringDirc(lineInfo *line, int *IC, int *DC)
{
	/* Make the label a data label (is there is one) */
	if (line->label)
	{
		line->label->isData = TRUE;
		line->label->address = FIRST_ADDRESS + *DC;
	}

	trimStr(&line->lineStr);

	if (isLegalStringParam(&line->lineStr, line->lineNum))
	{
		if (!addStringToData(line->lineStr, IC, DC, line->lineNum))
		{
			/* Not enough memory */
			printf("ERR:\t Not enough memory.\n");
			exit(0);
		}
	}
	else
	{
		/* Illegal string */
		printf("ERR:\tIllegal string. line: %d\n",line->lineNum);
		exit(0);
	}
}

/* 
	Description- Parses a .extern directive. 
*/
void parseExternDirc(lineInfo *line)
{
	/*  check if we use in labelInfo if not i can delete it.*/
	labelInfo label = { 0 }, *labelPointer;

	/* If there is a label in the line, remove the it from labelArr */
	if (line->label)
	{
		removeLastLabel(line->lineNum);
	}

	trimStr(&line->lineStr);
	labelPointer = addLabelToArr(label, line);
}

/* 
	Description- Parses a ".entry" directive. 
	GET- line structure.
*/
void parseEntryDirc(lineInfo *line)
{
	/* If there is a label in the line, remove the it from labelArr */
	if (line->label)
	{
		removeLastLabel(line->lineNum);
	}

	/* Add the label to the entry labels list */
	trimStr(&line->lineStr);

	if (isLegalLabel(line->lineStr, line->lineNum, TRUE))
	{
		if (isExistingEntryLabel(line->lineStr))
		{
			printf("ERR:\tLabel already defined as an entry label. line: %d",line->lineNum);
			exit(0);
		}
		else if (g_entryLabelsNum < MAX_LABELS_NUM)
		{
			g_entryLines[g_entryLabelsNum++] = line;
			printf(" Detect a dirctiv .entry%s\n", line);
		}
	}
	return;
}

/* 
	Description- Parses the directive and in a directive line. 
	GET- line- structure of info about the line.
	(IC, DC)
*/
void parseDirective(lineInfo *line, int *IC, int *DC)
{
	int i = 0;
	while (g_dircArr[i].name)
	{
		if (!strcmp(line->commandStr, g_dircArr[i].name))
		{
			/* Call the parse function for this type of directive */
			printf("%s\n", line);
			g_dircArr[i].parseFunc(line, IC, DC);
			return;
		}
		i++;
	}
	
	/* line->commandStr isn't a real directive */
	printf("ERR:\tNo such directive in line: %d.", line->lineNum);
	exit(0);
}

/* 
	Description- Returns if the operands' types are legal (depending on the command). 
*/
bool areLegalOpTypes(const command *cmd, operandInfo op1, operandInfo op2, int lineNum)
{
	/* --- Check First Operand --- */
	/* "lea" command (opcode is 6) can only get a label as the 1st op */
	if (cmd->opcode == 6 && op1.type != LABEL)
	{
		printf("ERR:\tSource operand for \"%s\" command must be a label. line:%d", cmd->name, lineNum);
		return FALSE;
	}

	/* 2nd operand can be a number only if the command is "cmp" (opcode is 1) or "prn" (opcode is 12).*/
	if (op2.type == NUMBER && cmd->opcode != 1 && cmd->opcode != 12)
	{
		printf("ERR:\tDestination operand for \"%s\" command can't be a number.line: %d", cmd->name, lineNum);
		return FALSE;
	}

	return TRUE;
}

/* 
	Description- Updates the type and value of operand. 
*/
void parseOpInfo(operandInfo *operand, int lineNum)
{
	int value = 0;

	if (isWhiteSpaces(operand->str))
	{
		printf("ERR:\tEmpty parameter. line: %d",lineNum);
		operand->type = INVALID;
		return;
	}

	/* Check if the type is NUMBER */
	if (*operand->str == '#')
	{
		operand->str++; /* Remove the '#' */

		/* Check if the number is legal */
		if (isspace(*operand->str)) 
		{
			printf("ERR:\tThere is a white space afetr the '#'.line: %d",lineNum);
			operand->type = INVALID;
		}
		else
		{
			operand->type = isLegalNum(operand->str, MEMORY_WORD_LENGTH - 2, lineNum, &value) ? NUMBER : INVALID;
		}
	}
	/* Check if the type is REGISTER */
	else if (isRegister(operand->str, &value))
	{
		operand->type = REGISTER;
	}
	/* Check if the type is LABEL */
	else if (isLegalLabel(operand->str, lineNum, FALSE))
	{
		operand->type = LABEL;
	}
	/* Check id op type is DYNM  */
	else if((strchr(*operand->str,'[') != NULL) && (strchr(*operand->str, ']') != NULL))
	{
		operand->type = DYNM;
	}
	/* The type is INVALID */
	else
	{
		printf("ERR:\t\"%s\" is an invalid parameter.line: %d", operand->str,lineNum);
		exit(0);
	}

	operand->value = value;
}

/* 
	Description- parses the operands in a command line.
	DET- line- line info.
	(IC, DC)
*/
void parseCmdOperands(lineInfo *line, int *IC, int *DC)
{
	char *startOfNextPart = line->lineStr;
	bool foundComma = FALSE;
	int numOfOpsFound = 0;

	/* Reset the op types */
	line->op1.type = INVALID;
	line->op2.type = INVALID;

	/* Get the parameters */
	FOREVER
	{
		/* If both of the operands are registers, they will only take 1 memory word (instead of 2) */
		if (!(line->op1.type == REGISTER && line->op2.type == REGISTER))
		{
			/* Check if there is enough memory */
			if (*IC + *DC < MAX_DATA_NUM)
			{
				++*IC; /* Count the last command word or operand. */
			}
			else
			{
				printf("ERR:\tNot enough memory.\n");
				exit(0);
			}
		}

		/* Check if there are still more operands to read */
		if (isWhiteSpaces(line->lineStr) || numOfOpsFound > 2)
		{
			/* If there are more than 2 operands it's already illegal */
			break;
		}

		/* If there are 2 ops, make the destination become the source op */
		if (numOfOpsFound == 1)
		{
			line->op1 = line->op2;
			/* Reset op2 */
			line->op2.type = INVALID;
		}

		/* Parse the opernad*/
		line->op2.str = getFirstOperand(line->lineStr, &startOfNextPart, &foundComma);
		parseOpInfo(&line->op2, line->lineNum);

		if (line->op2.type == INVALID)
		{
			printf("ERR:\t Operand 2 isn't valid. line: %d",line->lineNum);
			exit(0);
		}

		numOfOpsFound++;
		line->lineStr = startOfNextPart;
	} /* End of while */

	/* Check if there are enough operands */
	if (numOfOpsFound != line->cmd->numOfParams) 
	{
		/* There are more/less operands than needed */
		if (numOfOpsFound <  line->cmd->numOfParams)
		{
			printf("ERR:\tNot enough operands. line: %d", line->lineNum);
			exit(0);
		}
		else
		{
			printf("ERR:\tToo many operands line: %d", line->lineNum);
			exit(0);
		}
	}

	/* Check if there is a comma after the last param */
	if (foundComma)
	{
		printf("ERR:\tDon't write a comma after the last parameter.line: %d",line->lineNum);
		exit(0);
	}
	/* Check if the operands' types are legal */
	if (!areLegalOpTypes(line->cmd, line->op1, line->op2, line->lineNum))
	{
		printf("ERR:\tNot legal operands. line:%d\n", line->lineNum);
		exit(0);
	}
}

/* 
	Description- parses the command in a command line. 
	GET- line: line info.
	(IC, DC)
*/
void parseCommand(lineInfo *line, int *IC, int *DC)
{
	int cmdId = getCmdId(line->commandStr);

	if (cmdId == -1)
	{
		line->cmd = NULL;
		if (*line->commandStr == '\0')
		{
			/* The command is empty, but the line isn't empty so it's only a label. */
			printf("ERR:\tCan't write a label to an empty line: %d.", line->lineNum);
			exit(0);
		}
		else
		{
			/* Illegal command. */
			printf("ERR:\tNo such command as. line: %d", line->lineNum);
			exit(0);
		}
		return;
	}

	line->cmd = &g_cmdArr[cmdId];
	parseCmdOperands(line, IC, DC);
}

/* 
	Description-Returns the same string in a different part of the memory by using malloc. 
	GET- str: will allocate the chars of the line in str.
	RETURN: pointer to where to start read this line.
*/
char *allocString(const char *str) 
{
	char *newString = (char *)malloc(strlen(str) + 1);
	if (newString) 
	{
		strcpy(newString, str); 
	}

	return newString;
}

/* 
	Description- parses a line, and exit when setect error.
	GET-
		line: struct that whlie hold info about this line.
		lineStr: the line that we parse.
		lineNum: line number in the file.
		(IC, DC). 
*/
void parseLine(lineInfo *line, char *lineStr, int lineNum, int *IC, int *DC)
{
	char *startOfNextPart = lineStr;

	line->lineNum = lineNum;
	line->address = FIRST_ADDRESS + *IC;
	line->originalString = allocString(lineStr);
	line->lineStr = line->originalString;
	line->label = NULL;
	line->commandStr = NULL;
	line->cmd = NULL;

	if (!line->originalString)
	{
		printf("ERR:\tNot enough memory - malloc falied.");
		return;
	}

	/* Check if the line is a comment */
	if (isCommentOrEmpty(line))
	{	
		return;
	}

	/* Find label and add it to the label list */
	startOfNextPart = findLabel(line, *IC);

	/* Update the line if startOfNextPart isn't NULL */
	if (startOfNextPart)
	{
		line->lineStr = startOfNextPart;
	}

	/* Find the command token */
	line->commandStr = getFirstTok(line->lineStr, &startOfNextPart);
	line->lineStr = startOfNextPart;
	/* Parse the command / directive */
	if (isDirective(line->commandStr))
	{
		line->commandStr++; /* Remove the '.' from the command */
		parseDirective(line, IC, DC);
	}
	else
	{
		parseCommand(line, IC, DC);
	}

}

/* 
	Description- Puts a line from 'file' in 'buf'. Returns if the line is shorter than maxLength. 
	GET-
		file: the input file
		buf: array that holds 1 line frim a file.
		maxLength: limit of char in the file.
	RETURN- 
		True: if the buf contain all the line.
		False: else.
*/
bool readLine(FILE *file, char *buf, size_t maxLength)
{
	char *endOfLine;

	if (!fgets(buf, maxLength, file))
	{
		return FALSE;
	}

	/* Check if the line is too long (no '\n' was present). */
	endOfLine = strchr(buf, '\n');
	if (endOfLine)
	{
		*endOfLine = '\0';
	}
	else
	{
		char c;
		bool ret = (feof(file)) ? TRUE : FALSE; /* Return FALSE, unless it's the end of the file */

		/* Keep reading chars until you reach the end of the line ('\n') or EOF */
		do
		{
			c = fgetc(file);
		} while (c != '\n' && c != EOF);

		return ret;
	}

	return TRUE;
}

/*
	Description- Reading the file for the first time, line by line, and parsing it. 
	Returns how many errors were found. 

	GET-
		file: file to parse
		linesArr: array of maximum lines in the file.
		linesFound: how many lines are found.
		IC and DC.(no need to explain them)
*/
void firstFileRead(FILE *file, lineInfo *linesArr, int *linesFound, int *IC, int *DC)
{
	char lineStr[MAX_LINE_LENGTH + 2]; /* +2 for the \n and \0 at the end */

	*linesFound = 0;

	/* Read lines and parse them */
	while (!feof(file))
	{
		if (readLine(file, lineStr, MAX_LINE_LENGTH + 2)) 
		{
			/* Check if the file is too lone */
			if (*linesFound >= MAX_LINES_NUM)
			{
				printf("ERR:\tFile is too long. Max lines number in file is %d.\n", MAX_LINES_NUM);
				exit(0);
			}

			/* Parse a line */
			printf("%s\n", lineStr);			
			parseLine(&linesArr[*linesFound], lineStr, *linesFound + 1, IC, DC);

			/* Check if the number of memory words needed is small enough */
			if (*IC + *DC >= MAX_DATA_NUM)
			{
				/* dataArr is full. Stop reading the file. */
				printf("ERR:\tToo much data and code. Max memory words is %d.", MAX_DATA_NUM);
				printf("INFO:\tMemory is full. Stoping to read the file.\n");
				exit(0);
			}
			++*linesFound;
		}
		else if (!feof(file))
		{
			/* Line is too long */
			printf("ERR:\tLine is too long. Max line length is %d.line: %d", MAX_LINE_LENGTH,*linesFound + 1);
			exit(0);
			++*linesFound;
		}
	}

	return;
}