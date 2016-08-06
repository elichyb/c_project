/* Name: Elichy Barak ID: 301013462 */

/*This file contains utility parsing functions, mainly for the first read.*/
/* ======== Includes ======== */
#include "assmbler.h"

#include <ctype.h>
#include <stdlib.h>

/* ====== Methods ====== */
extern const command g_cmdArr[];
extern labelInfo g_labelArr[];
extern int g_labelNum;
lineInfo *g_entryLines[MAX_LABELS_NUM];
extern int g_entryLabelsNum;

/* 
	Description- returns a pointer to the label with 'labelName' name in 
				 g_labelArr or NULL if there isn't such label. 
	GET-
		labelName- char array that might contain the label name.
	RETURN- labelInfo or NULL. 
*/
labelInfo *getLabel(char *labelName)
{
	int i = 0;

	if (labelName)
	{
		for (i = 0; i < g_labelNum; i++)
		{
			if (strcmp(labelName, g_labelArr[i].name) == 0)
			{
				return &g_labelArr[i];
			}
		}
	}
	return NULL;
}

/* 
	Description- returns the ID of the command with 'cmdName' name in g_cmdArr or -1 if there isn't such command. 
	GET-
		cmdName.
	RETURN- 
		int.
*/
int getCmdId(char *cmdName)
{
	int i = 0;

	while (g_cmdArr[i].name)
	{
		if (strcmp(cmdName, g_cmdArr[i].name) == 0)
		{
			return i;
		}
		i++;
	}
	return -1;
}

/* 
	Description- removes spaces from start 
	GET- 
		**ptStr.
*/
void trimLeftStr(char **ptStr)
{
	/* Return if it's NULL */
	if (!ptStr)
	{
		return;
	}

	/* Get ptStr to the start of the actual text */
	while (isspace(**ptStr))
	{
		++*ptStr;
	}
}

/* 
	Description- removes all the spaces from the edges of the string ptStr is pointing to. 
	GET- 
		**ptStr
*/
void trimStr(char **ptStr)
{
	char *eos;

	/* Return if it's NULL or empty string */
	if (!ptStr || **ptStr == '\0')
	{
		return;
	}

	trimLeftStr(ptStr);

	/* oes is pointing to the last char in str, before '\0' */
	eos = *ptStr + strlen(*ptStr) - 1;

	/* Remove spces from the end */
	while (isspace(*eos) && eos != *ptStr)
	{
		*eos-- = '\0';
	}
}

/* 
	Description- returns a pointer to the start of first token. 
	Also makes *endOfTok (if it's not NULL) to point at the last char after the token. 
	GET- 
		*str.
		*enoOfTok.
	RETURN- char.
*/
char *getFirstTok(char *str, char **endOfTok)
{
	char *tokStart = str;
	char *tokEnd = NULL;

	/* Trim the start */
	trimLeftStr(&tokStart);

	/* Find the end of the first word */
	tokEnd = tokStart;
	while (*tokEnd != '\0' && !isspace(*tokEnd))
	{
		tokEnd++;
	}

	/* Add \0 at the end if needed */
	if (*tokEnd != '\0')
	{
		*tokEnd = '\0';
		tokEnd++;
	}

	/* Make *endOfTok (if it's not NULL) to point at the last char after the token */
	if (endOfTok)
	{
		*endOfTok = tokEnd;
	}
	return tokStart;
}

/* 
	Description- returns if str contains only one word. 
	GET- 
		*str.
	RETURN- bool.
*/
bool isOneWord(char *str)
{
	trimLeftStr(&str);							/* Skip the spaces at the start */
	while (!isspace(*str) && *str) { str++; }	/* Skip the text at the middle */					

	/* Return if it's the end of the text or not. */
	return isWhiteSpaces(str);
}

/* 
	Description- returns if str contains only white spaces. 
	GET- 
		*str.
	RETURN- bool.
*/
bool isWhiteSpaces(char *str)
{
	while (*str)
	{
		if (!isspace(*str++))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/* 
	Description- returns if labelStr is a legal label name. 
	GET- 
		labelStr.
		lineNum.
		printErrors- will be treu as long as the label is legal.
	RETURN- bool.
*/
bool isLegalLabel(char *labelStr, int lineNum, bool printErrors)
{
	int labelLength = strlen(labelStr), i;

	/* Check if the label is short enough */
	if (strlen(labelStr) > MAX_LABEL_LENGTH)
	{
		if (printErrors) 
			printf("ERR:\tLabel is too long. Max label name length is %d.line: %d", MAX_LABEL_LENGTH, lineNum);
		return FALSE;
	}

	/* Check if the label isn't an empty string */
	if (*labelStr == '\0')
	{
		if (printErrors) 
			printf("ERR:\tLabel name is empty. line: %d",lineNum);
		return FALSE;
	}

	/* Check if the 1st char is a letter. */
	if (isspace(*labelStr))
	{
		if (printErrors) 
			printf("ERR:\tLabel must start at the start of the line. line: %d",lineNum);
		return FALSE;
	}

	/* Check if it's chars only. */
	for (i = 1; i < labelLength; i++)
	{
		if (!isalnum(labelStr[i]))
		{
			if (printErrors) 
				printf("ERR:\t\"%s\" is illegal label - use letters and numbers only.line: %d", labelStr, lineNum);
			return FALSE;
		}
	}

	/* Check if the 1st char is a letter. */
	if (!isalpha(*labelStr))
	{
		if (printErrors) 
			printf("ERR:\t\"%s\" is illegal label - first char must be a letter.line: %d", labelStr,lineNum);
		return FALSE;
	}

	/* Check if it's not a name of a register */
	if (isRegister(labelStr, NULL)) /* NULL since we don't have to save the register number */
	{
		if (printErrors) 
			printf("ERR:\t\"%s\" is illegal label - don't use a name of a register.line: %d", labelStr,lineNum);
		return FALSE;
	}

	/* Check if it's not a name of a command */
	if (getCmdId(labelStr) != -1)
	{
		if (printErrors) 
			printf("ERR:\t\"%s\" is illegal label - don't use a name of a command.line: %d", labelStr,lineNum);
		return FALSE;
	}

	return TRUE;
}

/* 
	Description- returns if the label exists. 
	GET- 
		label.
	RETURN- bool.
*/
bool isExistingLabel(char *label)
{
	if (getLabel(label))
	{
		return TRUE;
	}

	return FALSE;
}

/* 
	Description- returns if the label is already in the entry lines array. 
	GET- 
		labelName.
	RETURN- bool.
*/
bool isExistingEntryLabel(char *labelName)
{
	int i = 0;

	if (labelName)
	{
		for (i = 0; i < g_entryLabelsNum; i++)
		{
			if (strcmp(labelName, g_entryLines[i]->lineStr) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

/* 
	Description- returns if str is a register name, and update value to be the register value. 
	GET- 
		str (char *).
		value (int).
	RETURN- bool.
*/
bool isRegister(char *str, int *value)
{
	if (str[0] == 'r'  && str[1] >= '0' && str[1] - '0' <= MAX_REGISTER_DIGIT && str[2] == '\0') 
	{
		/* Update value if it's not NULL */
		if (value)
		{
			*value = str[1] - '0'; /* -'0' To get the actual number the char represents */
		}
		return TRUE;
	}
	/*Not register*/
	return FALSE;
}

/* 
	Description- return a bool, represent whether 'line' is a comment or not.
	If the first char is ';' but it's not at the start of the line exit. 
	GET- 
		line- type (lineInfo).
	RETURN- bool.
*/
bool isCommentOrEmpty(lineInfo *line)
{
	char *startOfText = line->lineStr; /* We don't want to change line->lineStr */

	if (*line->lineStr == ';')
	{
		/* Comment */
		return TRUE;
	}

	trimLeftStr(&startOfText);
	if (*startOfText == '\0')
	{
		/* Empty line */
		return TRUE;
	}
	if (*startOfText == ';')
	{
		/* Illegal comment - ';' isn't at the start of the line */
		printf("ERR:\tComments must start with ';' at the start of the line.line: %d", line->lineNum);
		exit(0);
	}
	/* Not empty or comment */
	return FALSE;
}

/* 
	Description-returns a pointer to the start of the first operand in 'line' and change the end of it to '\0'. 
	Also makes *endOfOp (if it's not NULL) point at the next char after the operand. 
	GET- 
		line (char *).
		endOfOp (char **).
		foundComma (bool *).
	RETURN- char *.
*/
char *getFirstOperand(char *line, char **endOfOp, bool *foundComma)
{
	if (!isWhiteSpaces(line))
	{
		/* Find the first comma */
		char *end = strchr(line, ',');
		char *sqrBrackStart = strchr(line, '[');
		char *sqrBrackEnd = strchr(line, ']');

		if ((end) && (sqrBrackStart == NULL))
		{
			*foundComma = TRUE;
			*end = '\0';
			end++;
		}
		/*  check that is isn't this case [,]*/
		else if((sqrBrackStart != NULL) &&(sqrBrackEnd != NULL))
		{
			printf("ERR:\t Not supported in DYNM. \n");
			exit(0);
			/*
			end = strchr(sqrBrackEnd, ',');
			if ((end) && (sqrBrackStart == NULL))
			{
				*foundComma = TRUE;
				*end = '\0';
				end++;
			}*/
		}
		else
		{
			*foundComma = FALSE;
		}

		/* Set endOfOp (if it's not NULL) to point at the next char after the operand
		(Or at the end of it if it's the end of the line) */
		if (endOfOp)
		{
			if (end)
			{
				*endOfOp = end;
			}
			else
			{
				*endOfOp = strchr(line, '\0');
			}
		}
	}
	trimStr(&line);
	return line;
}

/* 
	Description- returns if the cmd is a directive. 
	GET- 
		cmd (char).
	RETURN- bool.
*/
bool isDirective(char *cmd)
{
	return (*cmd == '.') ? TRUE : FALSE;
}

/* Returns if the strParam is a legal string param (enclosed in quotes), and remove the quotes. */
bool isLegalStringParam(char **strParam, int lineNum)
{
	/* check if the string param is enclosed in quotes */
	if ((*strParam)[0] == '"' && (*strParam)[strlen(*strParam) - 1] == '"')
	{
		/* remove the quotes */
		(*strParam)[strlen(*strParam) - 1] = '\0';
		++*strParam;
		return TRUE;
	}

	if (**strParam == '\0')
	{
		printf("ERR:\tNo parameter? line: %d",lineNum);
	}
	else
	{
		printf("ERR:\tThe parameter for .string must be enclosed in quotes.line: %d",lineNum);
	}
	return FALSE;
}

/* 
	Description- returns if the num is a legal number param, and save it's value in *value. 
	GET- 
		numStr (char *).
		numOfBits (int).
		lineNum (int).
		value (int *).
	RETURN- bool.
*/
bool isLegalNum(char *numStr, int numOfBits, int lineNum, int *value)
{
	char *endOfNum;
	/* maxNum is the max number you can represent with (MAX_LABEL_LENGTH - 1) bits 
	 (-1 for the negative/positive bit) */
	int maxNum = (1 << numOfBits) - 1;

	if (isWhiteSpaces(numStr))
	{
		printf("ERR:\tEmpty parameter.line: %d",lineNum);
		return FALSE;
	}

	*value = strtol(numStr, &endOfNum, 0);

	/* Check if endOfNum is at the end of the string */
	if (*endOfNum)
	{
		printf("ERR:\t\"%s\" isn't a valid number.line: %d. \n", numStr,lineNum);
		return FALSE;
	}

	/* Check if the number is small enough to fit into 1 memory word 
	(if the absolute value of number is smaller than 'maxNum' */
	if (*value > maxNum || *value < -maxNum)
	{
		printf("ERR:\t\"%s\" is too %s, must be between %d and %d. line: %d.\n", numStr, (*value > 0) ? "big" : "small", -maxNum, maxNum, lineNum);
		return FALSE;
	}

	return TRUE;
}