#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"

/******************************************************************************
* File:             dstring.c
*
* Author:           Khanh Vong  
* Created:          10/26/19 
* Description:      Dynamic string library. Include tokenizer, different file
*					readers.
*****************************************************************************/


// Tokenize based on deliminator
char** tokenize(char* str, char* delim){
	int buf_size = 100;
	char** tokens = malloc(buf_size * sizeof(char**));
	if (!tokens){
		printf("Error allocating tokens\n");
		exit(EXIT_FAILURE);
	}
	char* token;
	int index = 0;
	token = strtok(str, delim);

	// Dynamic tokenization
	while(1){
		if (token == NULL){
			break;
		}
		tokens[index++] = token;

		// Increase memory allocation when needed
		if (index >= buf_size){
			tokens = realloc(tokens, sizeof(char)*(buf_size+=40));
			if (!tokens){
				printf("Error reallocating while tokenizing.\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	// Add a terminating element
	tokens[index] = NULL;
	return tokens;
}

// Reading from a file
char* d_readfile(FILE* file_ptr){
	int ch, count = 0;
	int buf_size = 100;
	char* str = (char*)malloc(buf_size*sizeof(char));
	if (!str){
		printf("Error allocating string.\n");
		exit(EXIT_FAILURE);
	}

	// Dynamically write to string
	while(1){
		ch = fgetc(file_ptr);
		if (ch == EOF){
			str[count] = '\0';
			break;
		}
		str[count++] = ch;
		if (count >= buf_size){
			str = realloc(str, sizeof(char)*(buf_size+=40));
			if (!str){
				printf("Error reallocating string while reading.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return str;
}

// Get length of a char**
int get_dcplength(char** str){
	int index = 0;
	while(str[index] != NULL){
		index++;
	}
	return index;
}

// Get length of char*
int get_cplength(char* str){
	int index = 0;
	while(str[index] != '\0'){
		index++;
	}
	return index;
}

int get_fline(FILE* file_ptr){
	int count = 0;
	int ch;
	while(1){
		ch = fgetc(file_ptr);
		if (ch == EOF){
			break;
		}
		if (ch == '\n') {
			count++;
		}
	}
	return count;
}

// Read a specific line in file
char* d_readline(FILE* file_ptr, int line){
	int ch, count = 0;
	int buf_size = 100;
	char* str = (char*)malloc(buf_size*sizeof(char));
	if (!str){
		printf("Error allocating string.\n");
		exit(EXIT_FAILURE);
	}

	// Ignore n lines
	int i;
	char ignore[10000];
	for(i = 0; i < line; i++){
		fgets(ignore, sizeof(ignore), file_ptr);
	}
	
	while(1){
		ch = fgetc(file_ptr);
		if (ch == EOF || ch == '\n'){
			str[count] = '\0';
			break;
		}
		str[count++] = ch;
		if (count >= buf_size){
			str = realloc(str, sizeof(char)*(buf_size+=40));
			if (!str){
				printf("Error reallocating string while reading.\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	return str;
}

