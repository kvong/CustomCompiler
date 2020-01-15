#include <codeGen.h>
#include <stdio.h>
#include <parsenode.h>
#include <staticSem.h>
#include <stdio.h>
#include <squeue.h>
#include <stack.h>
#include <cqueue.h>

/******************************************************************************
* File:             codeGen.c
*
* Author:           Khanh Vong  
* Created:          12/07/19 
* Description:      Code Generator for custom grammar.
*****************************************************************************/

// Run code generation from root
void run_codeGen(char* input, parsenode_t* root){
	int i;
	int table_size;
	label_counter = 0;
	temp_counter = 0;
	first_bit = 1;
	first_lock = 0;
	label_stack = NULL;
	breaker_stack = NULL;
	op_queue = init_cqueue();

	squeue_node* node;
	file_ptr = fopen(input, "w");

	codeGeneration( root, 0);

	table_size = sym_table->size;
	fprintf(file_ptr, "\nSTOP\n");

	// Generate variable allocation
	for (i = 0; i < table_size; i++) {
		node = desqueue(sym_table);
		fprintf(file_ptr, "%s 0\n", node->string);
	}
	for (i = 0; i < temp_counter; i++) {
		fprintf(file_ptr, "T%d 0\n", i);
	}

	for (i = 0; i < op_queue->size; i++) {
		decqueue(op_queue);
	}
	while(!isEmpty(label_stack))
		pop(&label_stack);

	while(!isEmpty(breaker_stack))
		pop(&breaker_stack);

	free(sym_table);
	fclose(file_ptr);
}

// Recursively generate code with respect to node's label
void codeGeneration(parsenode_t* root, int depth){
	sym_table;
	int i;
	int this_temp_counter;
	int this_loop_counter;
	int this_label_counter;
	int this_first_bit;
	int start_temp_counter;
	if (root != NULL) {
		if (isGenerator(root->label[0]) == 1) {
			// Generate code
			switch (root->label[0]) {
				case 'M':
					// Set first bit (only true in begining of line)
					if (first_bit == 1){
						this_first_bit = 1;
						first_bit = 0;
						start_temp_counter = temp_counter;
					}

					// Go down one child (always exist)
					codeGeneration(root->child[0], depth + 1);
					fprintf(file_ptr, "STORE T%d\n", temp_counter++);

					// Check for <H> <Z> <M>
					if (root->token_size > 0) {
						for (i = 0; i < root->token_size; i++) {
							// Put operator in a a queue to add together at the end
							if (strcmp(root->token[i], "") != 0) {
								switch (root->token[i][0]) {
									case '+':
										encqueue(op_queue, '+');
										break;
									case '-':
										encqueue(op_queue, '-');
										break;
									case '/':
										encqueue(op_queue, '/');
										break;
									case '*':
										encqueue(op_queue, '*');
										break;
								}
							}
						}
					}
					
					// Go through children (if exist)
					if (root->child_size > 1) {
						codeGeneration( root->child[1], depth + 1);
					}

					// Perform all operations on temp variables
					if (this_first_bit == 1){
						fprintf(file_ptr, "LOAD T%d\n", start_temp_counter);
						for (i = start_temp_counter + 1; i < temp_counter; i++) {
							switch (op_queue->front->key) {
								case '+':
									fprintf(file_ptr, "ADD ");
									break;
								case '-':
									fprintf(file_ptr, "SUB ");
									break;
								case '/':
									fprintf(file_ptr, "DIV ");
									break;
								case '*':
									fprintf(file_ptr, "MULT ");
									break;
							}
							fprintf(file_ptr, "T%d\n", i);
							decqueue(op_queue);
						}
						first_bit = 1;
					}
					break;
				case 'H':
					// Go down child (only exists one child)
					codeGeneration(root->child[0], depth + 1);

					// Check for absolute value
					if (root->token_size > 0) {
						label_counter++;
						push(&label_stack, label_counter);
						fprintf(file_ptr, "BRZPOS L%d\n", peek(label_stack));
						fprintf(file_ptr, "MULT -1\n");
						fprintf(file_ptr, "L%d: NOOP\n", pop(&label_stack));
						label_counter++;
					}
					break;
				case 'R':
					// End of node
					fprintf(file_ptr, "LOAD %s\n", root->token[0]);
					return;
					break;
				case 'A':
					// scan identifier
					if (isalpha(root->token[1][0])) {
						fprintf(file_ptr, "READ %s\n", root->token[1]);
					}
					break;
				case 'W':
					// write identifier/number
					this_temp_counter = temp_counter;
					for (i = 0; i < root->child_size; i++) {
						codeGeneration( root->child[i], depth + 1);
					}
					fprintf(file_ptr, "STORE T%d\n", this_temp_counter);
					fprintf(file_ptr, "WRITE T%d\n", this_temp_counter);
					first_lock = 0;
					temp_counter++;
					break;
				case 'I':
					// If statement
					fprintf(file_ptr, "\n");
					push(&label_stack, label_counter);
					this_label_counter = label_counter;
					this_temp_counter = temp_counter;

					// Traverse into children and reset first-locks after
					codeGeneration(root->child[2], depth + 1);
					fprintf(file_ptr, "STORE T%d\n", this_temp_counter);
					first_lock = 0;

					codeGeneration(root->child[0], depth + 1);
					fprintf(file_ptr, "SUB T%d\n", this_temp_counter);
					first_lock = 0;
					temp_counter++;

					codeGeneration(root->child[1], depth + 1);
					first_lock = 0;

					codeGeneration(root->child[3], depth + 1);
					first_lock = 0;

					fprintf(file_ptr, "L%d: NOOP\n", pop(&breaker_stack));
					fprintf(file_ptr, "\n");
					break;
				case 'G':
					// Loop
					fprintf(file_ptr, "\n");
					push(&label_stack, label_counter);
					fprintf(file_ptr, "L%d: NOOP\n", peek(label_stack));
					this_label_counter = peek(label_stack);

					this_temp_counter = temp_counter;

					// M2
					codeGeneration(root->child[2], depth + 1);
					first_lock = 0;
					fprintf(file_ptr, "STORE T%d\n", this_temp_counter);

					// M1
					codeGeneration(root->child[0], depth + 1);
					first_lock = 0;
					fprintf(file_ptr, "SUB T%d\n", this_temp_counter);
					temp_counter++;

					// Z
					codeGeneration(root->child[1], depth + 1);
					first_lock = 0;

					codeGeneration(root->child[3], depth + 1);
					first_lock = 0;

					// Loop to top
					fprintf(file_ptr, "BR L%d\n", this_label_counter);
					fprintf(file_ptr, "L%d: NOOP\n", pop(&breaker_stack));
					fprintf(file_ptr, "\n");
					break;
				case 'E':
					// Variable assignment
					first_lock = 0;
					codeGeneration( root->child[0], depth + 1);
					fprintf(file_ptr, "STORE %s\n", root->token[1]);
					break;
				case 'Z':
					// Generate conditional asm code
					label_counter++;
					switch (root->token[0][0]) {
						case '=':
							fprintf(file_ptr, "BRZERO L%d\n", label_counter);
							break;
						case '>':
							fprintf(file_ptr, "BRZNEG L%d\n", label_counter);
							break;
						case '<':
							fprintf(file_ptr, "BRZPOS L%d\n", label_counter);
							break;
						case ':':
							//fprintf(file_ptr, "BR L%d\n", label_counter);
							break;
					}
					push(&breaker_stack, label_counter);
					label_counter++;
					return;
					break;
			}
		}
		else{
			for (i = 0; i < root->child_size; i++) {
				codeGeneration( root->child[i], depth + 1);
			}
		}
	}
	else{
		return;
	}
}

// Check if a label is will generate asm code
int isGenerator(char label){
	switch (label) {
		case 'M':
		case 'H':
		case 'R':
		case 'A':
		case 'W':
		case 'I':
		case 'G':
		case 'E':
		case 'Z':
			return 1;
	}
	return -1;
}

