/*  Name: Elichy Barak ID: 301013462*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <ctype.h>

/*typedef enum {semicolon = ';', dote = '.', label} Parse;*/


void line_parser(char *line, int line_num)
{	
	int end_line = strlen(line); /*  the end of the line  */
	int start_line = 0; /*  the begininng of the line  */
	/*Parse parse;*/
	char dirction_word[DIR_SIZE]; /* will contain a word to help define it */
	int word_runner = 0; /* run over a word handler */
	char label[MAX_SYMBOL]; /*  will be use for label name  */
	/*  step 1: check that line isn't empty  */
	int counter = 0; /*  help counter  */
	int flag = 0; /* just notification to know if we are in the start of the line  */
	if (end_line == 1)
	{
		printf("Line is empty\n");
		return;
	}

	/*  step 2: run over the line  */
	for(start_line; start_line<=end_line;start_line++)
	{
		/* find comment in the line */	
		if ((*(line + start_line) == ';') && (flag == 0))
	        {
	        	return;
	    	}

	    /* if .entry or .extern  */
	    else if ((*(line + start_line) == '.') && (flag == 0))
	    {
	    	line++;
	    	
	    	while (word_runner < 6 && *line != ' ' && *line != '\t' && *line != '\n' && *line != EOF)
	    	{
	    		dirction_word[word_runner] = *line;
	    		word_runner++;
	    		line++;
	    	}
	    	
	    	if(strcmp("entry",dirction_word) == 0)
	    	{
	    		printf("1. entry\n");
	    		/* int the first6 parse we don't need to take care to .entry statments  */
	    		return;
	    	}
	    	
	    	else if((strcmp("extern",dirction_word) == 0))
	    	{
	    		/*  will take the outside labal name  */
	    		word_runner = 0;
	    		counter = 0;
	    		while((*line != '\n') && (*line != EOF))
	    		{
	    			counter++;
	    			line++;
	    			
	    			/*ignore spaces*/
	    			if((*line == ' ') && (*line == '\t'))
	    			{
	    				continue;
	    			}

	    			/*  start of the label name  */
	    			else
	    			{
	    				counter ++;
	    				if(counter < MAX_SYMBOL )
	    				{
	    					label[word_runner] = *line;
	    				}

	    				/*  check label size  */
	    				else
	    				{
	    					printf("Maximum label size (label can't be that big) \n");
	    					/*exit (0);*/
	    				}
	    			}
	    		}

	    		/* if not valid label exit */
	    		if(check_valid_label(label) != true)
	    		{
	    			printf("3. not vaild label\n");
	    			/*exit(0);*/
	    		}

	    		/*add_to_symbole_table();*/
	    		return;
	    	}
	    	
	    	else
	    	{
	    		printf("4. Not valid input line (line: %d)\n", line_num);
	    		/*exit(0);*/
	    	}
	    }
	    
	    /*
			Now we need to check if we are labal command or nigther.	
	    */
		if(isalpha(*line))
		{
			
		}

		else
		{
			printf("ERR: not valid line.(%d)\n",line_num);
			/*exit(0);*/
		}
    }
}

/*
	GET - any "label" and chcek validation
	RETURN- 
		* True if the label is valid.
		* False if not.
*/
bool check_valid_label(char *label_name)
{
		/*  check that the first char isn't a digit*/
	if(isdigit(*label_name))
	{
		printf("ERR: label can't be define if it start with digit.\n");
		return false;
	}
	
	else
	{
		label_name ++;
		while(*label_name != EOF)
		{
			if (isdigit(*label_name) || isalpha(*label_name))
			{
				continue;
			}
			
			else
			{
				printf("ERR: Unknown char?\n");
				return false;
			}
			label_name++;
		}
		return true;
	}
}
