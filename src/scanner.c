#include <token.h>
#include <stdio.h>
#include <stdlib.h>
#include <scanner.h>
#include <string.h>

/******************************************************************************
* File:             scanner.c
*
* Author:           Khanh Vong  
* Created:          10/26/19 
* Description:      Scanner a string and return first token starting from given
*                   offset.
*
*****************************************************************************/

/* Scan starting at offset and return one token */
token_t scanner(char* str, int* offset, int line_num){
	token_t token;
	int state = 0;
	int class;
	int i, j;
	int count = 0;
	int buf_size = 8;

	token.line = line_num;
	token.token = (char*)malloc(sizeof(char)*buf_size);
	for (i = *offset; i < strlen(str) + 1; i++) {
		class = get_class(str[i]);
		if (class == -1){
			printf("Character '%c' is not in the alphabet.\n", str[i]);	
			exit(EXIT_FAILURE);
		}
		
		state = get_next_state(state, class);

		// If state not final state
		if (state >= 0 && state <= 6) {
			if (state > 0) {
				// Put character into buffer
				token.token[count++] = str[i];
				if (count >= buf_size){
					token.token = realloc(token.token, sizeof(char)*(buf_size+=buf_size));
					if (!token.token){
						perror("testScanner.c: token.token realloc()");
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		// At final state
		else{
			token.token[count++] = '\0';
			if (count >= buf_size){
				token.token = realloc(token.token, sizeof(char)*(buf_size+=buf_size));
				if (!token.token){
					perror("scanner.c: token.token realloc()");
					exit(EXIT_FAILURE);
				}
			}
			// Finish up token
			if ((token.tokenID = fstoid(state)) == -1) {
				printf("Error state ocurred on offset %d line %d\n", i, line_num);
				exit(EXIT_FAILURE);
			}
			
			// Check if identifier is a keyword (Filter 2)
			if (token.tokenID == 0) {
				for (j = 0; j < KWTABLE_SIZE; j++) {
					if (strcmp(token.token, KW_TABLE[j]) == 0){
						token.tokenID = KEYW_ID;
						break;
					}
				}
			}
			// Update offset (by pointer)
			*offset = i;
			break;
		}
	}
	return token;
}

// Find a character's class
int get_class(char var){
	if (var >= 97 && var <= 122)
		return 0;
	else if (var >= 65 && var <= 90)
		return 1;
	else if (var  == 61 )
		return 2;
	else if (var == 60 || var == 62 || var == 58 
			|| var == 38 || (var >= 42 && var <= 47) 
			|| var == 35 || var == 91 || var == 93)
		return 3;	
	else if (var >= 48 && var <= 57)
		return 4;
	else if (var == 0)
		return 5;
	else if (var == 32)
		return 6;
	else
		return -1;
}

// Table look up to get next state
int get_next_state(int state, int class){
	int new_state = FSA_TABLE[state][class];
	return new_state;
}
