/*  Name: Elichy Barak ID: 301013462*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <ctype.h>

int counter_to_DC = 0; /*  counter to data  */
int counter_to_IC = 0; /*  counter to code area  */

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
	char number_holder[10]; /*  the maximum possiable number is 2^16  */ 

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
	    		/* int the first parse we don't need to take care to .entry statments  */
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
	    		add_to_symbole_table(label, IC, true, false);
	    		return;
	    	}
	    /*	else if((strcmp("data",dirction_word) == 0))
	    	{
	    		
	    		while(*line != EOF)
	    		{
	    			counter = 0;
	    			while((*line != ' ') && (counter < 10))
	    			{
	    				if(isdigit(*line)&& *line = '-')
	    					number_holder[counter] = *line;
	    				else
	    				{
	    					printf("ERR: Not a number! line(%d)\n", line_num);
	    				}
	    				counter++;
	    				line++;
	    			}
	    			counter_to_DC++;

	    			line++;
	    		}
	    	}
	    	else if((strcmp("string",dirction_word) == 0))
	    	{
	    	}*/
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
			/*  reset to index veriabls*/
			flag = 1;			
			/*  
				we start with the guess that every line start with label... 
				then we will check if its command or not
			*/
			while((*line != '\n') && (counter<= 100))
			{
				/*
					read a label/command/Unknow value.
				*/
				counter = 0;

				while(*line != ' ')
				{
					label[counter] =*line;
					line++;
					counter++;
				}
				
				if((label[counter] = ':') && (flag == 1))
				{
					/*
						save the label name and continue to check what comes next.
					*/
					flag = 2; /* have one label in this line  */
				}
				
				/*else if((check_if_valid_command(label) == true) && (flag == 1 || flag == 2))
				{
					flag = 3;
				}*/
				else
				{
					printf("Unknown value in line (%d)\n", line_num);
					/*exit(0);*/
				}
			}
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

/*
	check if command
*/
/*bool check_if_valid_command(char command[])
{
	if(strcmp('mov', command) == 0)
	{
		counter_to_IC +=3;
		return true;
	}
	if(strcmp('cmp', command) == 0)
	if(strcmp('add', command) == 0)
	if(strcmp('sub', command) == 0)
	if(strcmp('not', command) == 0)
	if(strcmp('clr', command) == 0)
	if(strcmp('lea', command) == 0)
	if(strcmp('inc', command) == 0)
	if(strcmp('dec', command) == 0)
	if(strcmp('jmp', command) == 0)
	if(strcmp('bne', command) == 0)
	if(strcmp('red', command) == 0)
	if(strcmp('prn', command) == 0)
	if(strcmp('jsr', command) == 0)
	if(strcmp('rts', command) == 0)
	if(strcmp('stop', command) == 0)

}*/

void add_to_symbole_table(char[] label_name, int address, bool is_ext, bool is_dirction)
{
	symbol_table *p;
	
}