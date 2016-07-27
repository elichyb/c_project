/*
	Name: Elichy Barak ID: 301013462
*/
#ifndef HEADER_H
#define HEADER_H

/* BOOLEANS handler  */
typedef int bool;
#define true 1
#define false 0

/*  enum that list the posiible commands  */
typedef enum {mov = 0, cmp = 1, add = 2, sub = 3, not = 4, clr = 5
				lea = 6, inc = 7, dec = 8, jmp = 9, bne = 10, red = 11, 
				prn = 12, jsr = 13, rts = 14, stop = 15} command;

/*  maximum size of symbol is 30  */
#define MAX_SYMBOL 31

/*  defint the start of the code  */
#define IC 100

/*  define dirction maximum size*/
#define DIR_SIZE 6

/*  
	symbol table:
	Symbol table struct presents a line in the table
*/

typedef struct symbol_table
{
	char symbol_name[MAX_SYMBOL];
	int address;
	bool is_ext;
	
	/*
	if not action sentence it will be a dirction.
	*/
	bool is_action;
} *symbol_line;

/*  define data structure of data  */
typedef struct data
{
	int address;
}*data;

/*  define data structure of code line  */
typedef struct code_line
{
	int address;
}*line;

/*
-----------------------------------------------------
				functions defined
-----------------------------------------------------
*/
void parser_file(char *);
void removeOldFiles();
void line_parser(char *);
#endif
