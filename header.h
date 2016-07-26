/*
	Name: Elichy Barak ID: 301013462
*/
#ifndef HEADER_H
#define HEADER_H

/* BOOLEANS handler  */
typedef int bool;
#define true 1
#define false 0

/*maximum size of symbol is 30*/
#define MAX_SYMBOL 31

/*  symbol table*/
struct symbol_table
{
	char symbol_name[MAX_SYMBOL];
	int address;
	bool is_ext;
	bool is_action_or_dirction;
};
#endif
