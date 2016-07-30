/*  Name: Elichy Barak ID: 301013462  */

#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include <ctype.h>

/*
---------------------------------------------
will add stuff to symbol table
---------------------------------------------
*/
/*add_to_symbol_table()
{
	symbol_table *p2symbol;
	p2symbol = (symbol_table *)realloc(symbol_table, sizeof(symbol_table)*1);

}*/


/*
	GET - any "label" and chcek validation
	RETURN- 
		* True if the label is valid.
		* False if not.
*/
bool check_valid_label(char *label_name)
{
	int label_index = 0; /*will run over the label  */
	/*  check that the first char isn't a digit*/
	if(isdigit(*label_name))
	{
		printf("ERR: label can't be define if it start with digit.\n");
		return false;
	}
	
	else
	{
		*label_name ++;
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
			*label_name++;
		}
		return true;
	}
}
