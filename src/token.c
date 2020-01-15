#include <token.h>

/******************************************************************************
* File:             token.c
*
* Author:           Khanh Vong  
* Created:          10/26/19 
* Description:      Define tokens, names, keyword table and FSA table.
*****************************************************************************/


const char* TK_NAMES[] = {"Identifier", "Number", "Operator", "Keyword", "EOF"}; 

const int FSA_TABLE[7][7] = {
						//  0	1	2	3	4	5	6	
					    {	1,	-1,	2,	3,	4,	14,	0}, // S0
						{	5,	5,	-1,	-1,	5,	-1,	-1}, // S1
						{	12,	12,	6,	12,	12,	12,	12}, // S2
						{	12, 12, 12, 12, 12, 12, 12}, // S3
						{	11, 11, 11, 11,	4,	11, 11}, // S4
						{	5,	5,	10, 10,	5,	10, 10}, // S5
						{	12, 12, 12, 12, 12, 12, 12}};// S6

const char* KW_TABLE[] = {"begin", "end", "repeat", "if", "then", "let",
						"void", "var", "return", "scan", "write", "program"};

// Convert final state to token_id
int fstoid(int final_state){
	switch (final_state) {
		case ID_FS:
			return IDENT_ID;
			break;
		case DG_FS:
			return NUM_ID;
			break;
		case OP_FS:
			return OPER_ID;
			break;
		case KW_FS:
			return KEYW_ID;
			break;
		case EF_FS:
			return EOF_ID;
			break;
		default:
			return -1;
	}	
}

// Display token
void display_token(token_t token){
	if (strlen(token.token) != 0){
		printf("-> Token ID %d", token.tokenID);
		printf(" | Token type '%s'", TK_NAMES[token.tokenID]);
		printf(" | Token value '%s'", token.token);
		printf(" | Token on line %d\n", token.line);
	}
}
