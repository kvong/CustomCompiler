#include <scanner.h>
#include <testScanner.h>
#include <token.h>
#include <stdio.h>
#include <stdlib.h>
#include <dstring.h>
#include <string.h>
#include <tqueue.h>
#include <tnode.h>

/******************************************************************************
* File:             testScanner.c
*
* Author:           Khanh Vong  
* Created:          10/26/19 
* Description:      Test scanner: Open a file and reading one line at a time
*					splitting out tokens accordingly. Seeker for scanner is
*					controlled by offsets.
*
*****************************************************************************/


tqueue_t* testScan(char* filename){
	FILE* file_ptr = NULL;
	char name[80];
	int total_lines;
	int i, len, c_state, c_class, offset = 0;
	int buf_size = 10;
	
	tqueue_t* token_queue = init_tqueue();
	token_t token;

	strcpy(name, "");
	strcat(name, ".filter1");

	file_ptr = fopen(filename, "r");
	if (!file_ptr){
		perror("testScan: Failed to open file for reading");
		exit(EXIT_FAILURE);
	}

	// Remove comment from file
	char* filter_1 = filter_one(file_ptr, '%');
	filter_1 = tab_remove(filter_1);

	if (file_ptr != NULL){
		fclose(file_ptr);
	}
	file_ptr = NULL;

	// Write to new temp file
	file_ptr = fopen(name, "w+");
	if (!file_ptr){
		perror("testScan: Failed to open file for writing");
		exit(EXIT_FAILURE);
	}

	// Write to new temp file (no comments)
	fputs(filter_1, file_ptr);
	
	// Put reader/writer to begining of file
	if (fseek(file_ptr, 0, SEEK_SET) == -1){
		perror("testScanner.c: fseek()");
		exit(EXIT_FAILURE);
	}

	total_lines = get_fline(file_ptr);

	// Reset seeker to begining of file
	if (fseek(file_ptr, 0, SEEK_SET) == -1){
		perror("testScanner.c: fseek()");
		exit(EXIT_FAILURE);
	}

	token.token = (char*)malloc(sizeof(char)*buf_size);
	for (i = 0; i < total_lines; i++) {
		offset = 0;
		filter_1 = d_readline(file_ptr, i);
		len = strlen(filter_1) + 1;

		while (offset < len - 1) {
			c_state = 0;

			// Get token from scanner
			token = scanner(filter_1, &offset, i + 1);
			if (strlen(token.token) != 0){
				entqueue(token_queue, token);
			}
		}
		// Set EOFtk at the end
		if ((offset == len -1) && (i == total_lines -1)){
			token.token = "EOF";
			token.line = total_lines;
			token.tokenID = EOF_ID;
			entqueue(token_queue, token);
		}
		// If not the end reset seeker
		else if (fseek(file_ptr, 0, SEEK_SET) == -1){
			perror("testScanner.c: fseek()");
			exit(EXIT_FAILURE);
		}
	}

	if (file_ptr != NULL){
		fclose(file_ptr);
	}
	free(filter_1);
	return token_queue;
}

// Remove text surrounded by comment_tk
char* filter_one(FILE* file_ptr, char comment_tk){
	int ch, count = 0, comment = 0;
	int buf_size = 100;
	char* str = (char*)malloc(buf_size*sizeof(char));
	if (!str){
		perror("testScanner.c: remove_comment(): malloc()");
		exit(EXIT_FAILURE);
	}

	while(1){
		ch = fgetc(file_ptr);
		if (ch == EOF){
			if (comment == 1){
				printf("Error: Open-ended commenting detected while in filter 1.\n");
				free(str);
				exit(EXIT_FAILURE);
			}
			str[count] = '\0';
			break;
		}
		// Turn on comment blocker
		if (ch == comment_tk && comment == 0){
			comment = 1;
		}
		// Turn off comment blocker
		else if (ch == comment_tk && comment == 1) {
			comment = 0;
			ch = ' ';
		}
		if (comment == 1 && ch != '\n' && ch != '\0'){
			ch = ' ';
		}
		str[count++] = ch;
		if (count >= buf_size){
			str = (char*)realloc(str, sizeof(char)*(buf_size += buf_size));
			if (!str){
				perror("testScanner.c: remove_comment(): realloc()");
				exit(EXIT_FAILURE);
			}
		}
	}
	return str;
}

char* tab_remove(char* str){
	int len = strlen(str) + 1;
	int i;
	for (i = 0; i < len; i++) {
		if (str[i] == '\t') {
			str[i] = ' ';
		}
	}
	return str;

}

