/*  Name: Elichy Barak ID: 301013462*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

typedef enum {semicolon = ';', dote = '.', label} Parse;


void line_parser(char *line)
{	
	int end_line = strlen(line); /*  the end of the line  */
	int start_line = 0; /*  the begininng of the line  */
	Parse parse;
	char dirction_word[DIR_SIZE];
	int word_runner = 0;
	/*  step 1: check that line isn't empty  */
	if (end_line == 1)
	{
		printf("Line is empty\n");
		return;
	}


	/*  step 2: run over the line  */
	for(start_line; start_line<=end_line;start_line++)
	{
		parse = *line;
		switch(parse)
		{
			case semicolon :
			/*If comment skip line*/
				printf("commented line\n");
				return;
				break;
			case dote :
			/*  check if i am a dirction word  */
				word_runner = 0;
				/* after we checked that we have a . i am increasing the line index plus 1 --> for continue investigation  */
				*line++;
				while(word_runner<6 && *line!=' ' && *line!='\t' && *line!='\n' && *line!=EOF)
				{
					dirction_word[word_runner] = *line;
				}
				/*Checks if entry*/
	            if (strcmp("entry ", dirction_word)==0)
	            {

	            }
	            /*Checks if extern*/
	            else if(strcmp("extern",dirction_word) == 0)
	            {

	            }
	            /*Checks if string*/
	            else if (strcmp("string", dirction_word) == 0)
	            {
	            	
	            }
	            /*Checks if data*/
	            else if (strcmp("data", dirction_word) == 0)
	            {

	            }
	            /*print an error*/
	            else
	            {
	            	printf("Unable to get this symbol: %s\n", dirction_word);
	            }
				break;


		}
		printf("%c", line[start_line]);
	}
}