#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dstring.h>
#include <parser.h>
#include <parsenode.h>
#include <testTree.h>
#include <tqueue.h>
#include <staticSem.h>
#include <cqueue.h>


/******************************************************************************
* File:             main.c
*
* Author:           Khanh Vong  
* Created:          11/16/19 
*                   Drivers program to perform code generation for grammar.
*                   Input from stdin or file argument.
*****************************************************************************/

int validate(char*, int);
int main(int argc, char *argv[])
{
	FILE* file_ptr;
	char* str;
	int i;
	int length;

	// Set default file name
	char name[80];
	strcpy(name, "");
	strcat(name, ".temp");

	// If input is given
	if (argc == 2){
		strcpy(name, "");
		strcat(name, argv[1]);
		file_ptr = fopen(name, "r");
		if (file_ptr == NULL) {
			perror("Main: Error: fopen");
			exit(EXIT_FAILURE);
		}
		str = d_readfile(file_ptr);
		length = get_cplength(str);
		if (validate(str, length) == 0) {
			exit(EXIT_FAILURE);
		}
	}
	// Read from stdin
	else if (argc == 1){
		str = d_readfile(stdin);
		length = get_cplength(str);
		if (validate(str, length) == 0) {
			exit(EXIT_FAILURE);
		}
		file_ptr = fopen(name, "w");
		if (file_ptr == NULL){
			perror("Main: Error: fopen");
			free(str);
			exit(EXIT_FAILURE);
		}
		fprintf(file_ptr, "%s \n", str);
		fclose(file_ptr);
		free(str);
		printf("\n");
	}
	else{
		printf("Fatal: Improper usage\n");
		printf("Usage: frontEnd [filename]\n");
		return 1;
	}

	// Perform parsing
	root = parser(name);

	//treePrint(root, 0);
	
	test_semantic(root);

	if (argc == 1) {
		strcpy(name, "kb");
	}
	else{
		strcpy(name, "");
		strcat(name, argv[1]);
	}

	strcat(name, ".asm");

	run_codeGen(name, root);
	printf("ASM generated '%s'\n", name);

	//free_table();
	free_parsetree(root);
	free_queue();

	return 0;
}

// Validation step
int validate(char* str, int length){
	int index = 0;
	int letters = 0;
	if(length == 0){
		printf("Main: Validatate: Empty file detected\n");
		return 0;
	}
	while((islower(str[index]) || isupper(str[index]) || isspace(str[index])) && index < length){
		if(!isspace(str[index]))
			letters++;
		index++;
	}

	if (letters == 0){
		printf("Main: Validatate: No characters detected in file.\n");	
		return 0;
	}
	else if ( index == length ){
		return 1;
	}
	return 1;
}

