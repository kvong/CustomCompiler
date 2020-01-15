#include <stdio.h>
#include <scanner.h>
#include <token.h>
#include <stdlib.h>
#include <dstring.h>
#include <string.h>
#include <parser.h>
#include <testScanner.h>
#include <tqueue.h>
#include <parsenode.h>

/******************************************************************************
* File:             parser.c
*
* Author:           Khanh Vong  
* Created:          11/16/19 
*                   Parse tree generator through recursion call of functions.
*                   Functions are set up as nonterminators. Tokens are stored
*                   in a queue and dequeued accordingly.
*****************************************************************************/

// Parse tree creation
parsenode_t* parser(char* filename){
	int i, t_count = 0;
	tokens = testScan(filename);

	t_node = detqueue(tokens);
	
	root = S();

	if (tokens->size < 1) {
		if (t_node->token.tokenID == 4) {
			return root;
		}
		else{
			printf("Parser: EOF not reached\n");
			free_parsetree(root);
			free_queue();
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser: More than 1 token remaining\n");
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
}

// S -> program <V> <B>
parsenode_t* S(){
	parsenode_t* node = init_parsenode(2, 1);
	node->label = "S";
	if (strcmp(t_node->token.token, "program") != 0) {
		printf("Parser: Error: S(): Keyword 'program' expected on line %d.\n" , t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	else{
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[0] = V();
		node->child[1] = B();
	}
	return node;
}

// B -> begin <V> <Q> end
parsenode_t* B(){
	parsenode_t* node = init_parsenode(2, 2);
	node->label = "B";
	if (strcmp(t_node->token.token, "begin") != 0){
		printf("Parser: Error: B(): Keyword 'begin' expected on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;
	t_node = detqueue(tokens);
	node->child[0] = V();
	node->child[1] = Q();
	if (strcmp(t_node->token.token, "end") != 0) {
		printf("Parser: Error: B(): Keywork 'end' expected on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	else{
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
	}
	return node;
}

// V -> [empty] | var [identifier] . <V>
parsenode_t* V(){
	if (strcmp(t_node->token.token, "var") != 0) 
		return NULL;
	
	parsenode_t* node = init_parsenode(1, 3);
	node->label = "V";
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;
	
	t_node = detqueue(tokens);
	if (t_node->token.tokenID == 0){
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
	}
	else{
		printf("Parser: Error: V(): Identifier expected after 'var' on line %d.\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	if (strcmp(t_node->token.token, ".") != 0) {
		printf("Parser: Error: V(): '.' expected after identifier on line %d.\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	else{
		strcpy(node->token[2], t_node->token.token);
		node->tokenID[2] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[0] = V();
	}
	return node;
}

// M -> <H> + <M> | <H> - <M> | <H> / <M> | <H> * <M> | <H>
parsenode_t* M(){
	parsenode_t* node;
	node = init_parsenode(2, 1);
	node->label = "M";
	node->child[0] = H();

	if (strcmp(t_node->token.token, "+") == 0 || 
		strcmp(t_node->token.token, "-") == 0 || 
		strcmp(t_node->token.token, "/") == 0 || 
		strcmp(t_node->token.token, "*") == 0 ) 
	{
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[1] = M();
	}
	else{
		node->child[1] = NULL;
	}
	return node;
}

// H -> & <R> | <R>
parsenode_t* H(){
	if (strcmp(t_node->token.token, "&") == 0){
		parsenode_t* node = init_parsenode(1, 1);
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[0] = R();
		node->label = "H";
		return node;
	}
	else{
		parsenode_t* node = init_parsenode(1, 0);
		node->child[0] = R();
		node->label = "H";
		return node;
	}
}

// R -> [identifier] | [number]
parsenode_t* R(){
	if (t_node->token.tokenID > 1){
		printf("Parser: Error: R(): Expected Identifier/Number on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	else{
		parsenode_t* node = init_parsenode(0, 1);
		node->label = "R";
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		return node;
	}
}

// Q -> <T> # <Q> | [empty]
parsenode_t* Q(){
	if (strcmp(t_node->token.token, "scan") == 0 ||
	strcmp(t_node->token.token, "write") == 0 ||
	strcmp(t_node->token.token, "begin") == 0 ||
	strcmp(t_node->token.token, "if") == 0 ||
	strcmp(t_node->token.token, "repeat") == 0 ||
	strcmp(t_node->token.token, "let") == 0){
		parsenode_t* node = init_parsenode(2, 1);
		node->child[0] = T();
		if (strcmp(t_node->token.token, "#") == 0) {
			strcpy(node->token[0], t_node->token.token);
			node->tokenID[0] = t_node->token.tokenID;
			t_node = detqueue(tokens);
			node->child[1] = Q();
			node->label = "Q";
			return node;
		}
		else{
			printf("Parser: Error: Q(): Expected '#' on line %d\n", t_node->token.line);
			display_token(t_node->token);
			free_parsetree(root);
			free_queue();
			exit(EXIT_FAILURE);
		}
	}
	else{
		return NULL;
	}
}

// T -> <A> ,| <W> ,| <B> | <I> ,| <G> ,| <E> ,
parsenode_t* T(){
	parsenode_t* node = init_parsenode(1, 1);
	node->label = "T";
	if (strcmp(t_node->token.token, "scan") == 0) {
		node->child[0] = A();
	}
	else if (strcmp(t_node->token.token, "write") == 0) {
		node->child[0] = W();
	}
	else if (strcmp(t_node->token.token, "begin") == 0) {
		node->child[0] = B();
		return node;
	}
	else if (strcmp(t_node->token.token, "if") == 0) {
		node->child[0] = I();
	}
	else if (strcmp(t_node->token.token, "repeat") == 0) {
		node->child[0] = G();
	}
	else if (strcmp(t_node->token.token, "let") == 0) {
		node->child[0] = E();
	}
	else{
		printf("Parser: Error: T(): Expected {scan, write, begin, if, repeat, let} on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}

	if (strcmp(t_node->token.token , ",") == 0) {
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		return node;
	}
	else{
		printf("Parser: Error: T(): Expected ',' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
}

// A -> scan [identifier] | scan [number]
parsenode_t* A(){
	if (strcmp(t_node->token.token, "scan") != 0) {
		printf("Parser: Error A(): Expected 'scan' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	parsenode_t* node= init_parsenode(0, 2);
	node->label = "A";
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;
	t_node = detqueue(tokens);
	if (t_node->token.tokenID < 2) {
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		return node;
	}
	else{
		printf("Parser: Error: A(): Expected Identifier/Number on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
}

// W -> write <M>
parsenode_t* W(){
	parsenode_t* node = init_parsenode(1, 1);
	node->label = "W";
	if (strcmp(t_node->token.token, "write") != 0) {
		printf("Parser: Error: W(): Expected 'write' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;
	t_node = detqueue(tokens);
	node->child[0] = M();
	return node;
}

// I -> if [ <M> <Z> <M> ] <T>
parsenode_t* I(){
	if (strcmp(t_node->token.token, "if") != 0) {
		printf("Parser: Error: I(): Expected 'if' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}

	parsenode_t* node= init_parsenode(4, 3);
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;

	node->label = "I";

	t_node = detqueue(tokens);
	if (strcmp(t_node->token.token, "[") == 0) {
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[0] = M();
		node->child[1] = Z();
		node->child[2] = M();
		
		if (strcmp(t_node->token.token, "]") == 0) {
			strcpy(node->token[2], t_node->token.token);
			node->tokenID[2] = t_node->token.tokenID;
			t_node = detqueue(tokens);
			node->child[3] = T();
			return node;
		}
		else{
			printf("Parser: Error: I(): Expected ']' on line %d\n", t_node->token.line);
			display_token(t_node->token);
			free_parsetree(root);
			free_queue();
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser: Error: I(): Expected '[' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
}

// G -> repeat [ <M> <Z> <M> ] <T>
parsenode_t* G(){
	if (strcmp(t_node->token.token, "repeat") != 0) {
		printf("Parser: Error: G(): Expected 'repeat' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}

	parsenode_t* node= init_parsenode(4, 3);
	node->label = "G";
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;

	t_node = detqueue(tokens);
	if (strcmp(t_node->token.token, "[") == 0) {
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		node->child[0] = M();
		node->child[1] = Z();
		node->child[2] = M();
		
		if (strcmp(t_node->token.token, "]") == 0) {
			strcpy(node->token[2], t_node->token.token);
			node->tokenID[2] = t_node->token.tokenID;
			t_node = detqueue(tokens);
			node->child[3] = T();
			return node;
		}
		else{
			printf("Parser: Error: G(): Expected ']' on line %d\n", t_node->token.line);
			display_token(t_node->token);
			free_parsetree(root);
			free_queue();
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser: Error: G(): Expected '[' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	
}

// E -> let [identifier] : <M>
parsenode_t* E(){
	if (strcmp(t_node->token.token, "let") != 0) {
		printf("Parser: Error: E(): Expected 'let' on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}

	parsenode_t* node= init_parsenode(1, 3);
	node->label = "E";
	strcpy(node->token[0], t_node->token.token);
	node->tokenID[0] = t_node->token.tokenID;
	
	t_node = detqueue(tokens);
	if (t_node->token.tokenID == 0) {
		strcpy(node->token[1], t_node->token.token);
		node->tokenID[1] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		if (strcmp(t_node->token.token, ":") == 0) {
			strcpy(node->token[2], t_node->token.token);
			node->tokenID[2] = t_node->token.tokenID;
			t_node = detqueue(tokens);
			node->child[0] = M();
			return node;
		}
		else{
			printf("Parser: Error: E(): Expected ':' on line %d\n", t_node->token.line);
			display_token(t_node->token);
			free_parsetree(root);
			free_queue();
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Parser: Error: E(): Expected Identifier on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
}

// Z -> < | > | : | = | ==
parsenode_t* Z(){
	if (strcmp(t_node->token.token, "<") != 0 &&
		strcmp(t_node->token.token, ">") != 0 &&
		strcmp(t_node->token.token, ":") != 0 &&
		strcmp(t_node->token.token, "=") != 0 &&
		strcmp(t_node->token.token, "==") != 0 ) {
		printf("Parser: Error: Z(): Expected {<, >, :, =, ==} on line %d\n", t_node->token.line);
		display_token(t_node->token);
		free_parsetree(root);
		free_queue();
		exit(EXIT_FAILURE);
	}
	else{
		parsenode_t* node= init_parsenode(0, 1);
		node->label = "Z";
		strcpy(node->token[0], t_node->token.token);
		node->tokenID[0] = t_node->token.tokenID;
		t_node = detqueue(tokens);
		return node;
	}
}

