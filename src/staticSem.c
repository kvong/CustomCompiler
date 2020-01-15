#include <stdio.h>
#include <stdlib.h>
#include <parsenode.h>
#include <parser.h>
#include <staticSem.h>
#include <tqueue.h>
#include <squeue.h>

void test_semantic(parsenode_t* proot){
	int i, table_size;
	squeue_node* node;
	sym_table = init_squeue();
	static_semantic(proot);

	/* For freeing up symbol table
	 *
	printf("Symbol Table:\n");
	table_size = sym_table->size;
	for (i = 0; i < table_size; i++) {
		node = desqueue(sym_table);
		printf(" %s |", node->string);
	}
	printf("\n");
	free(sym_table);
	*/
}

void static_semantic(parsenode_t* proot){
	token_t tk;
	squeue_node* node;
	if (proot != NULL) {
		int i, j, token_size, table_size, declared;
		if (strcmp(proot->label, "V") == 0){

			declared = 0;
			// Check List: Declared?
			
			table_size = sym_table->size;
			for (i = 0; i < table_size; i++) {
				node = desqueue(sym_table);

				if (strcmp(node->string, proot->token[1]) == 0) {
					declared = 1;
					ensqueue(sym_table, node->string);
					break;
				}
				ensqueue(sym_table, node->string);
			}
			if (declared == 0) {
				// No ADD 
				ensqueue(sym_table, proot->token[1]);
			}
			else{
				// Yes ERROR
				printf("StaticSem: Error: Double declaration detected on '%s'\n", node->string);
				free_parsetree(root);
				free_queue();
				free(sym_table);
				exit(EXIT_FAILURE);
			}
		}
		else{
			token_size = proot->token_size;
			for (i = 0; i < token_size; i++) {
				if (proot->tokenID[i] == 0 && strcmp(proot->token[i], "")) {

					table_size = sym_table->size;
					declared = 0;
					// Check List: Is identifier declared?
					for (j = 0; j < table_size; j++) {
						node = desqueue(sym_table);

						if (strcmp(node->string, proot->token[i]) == 0) {
							declared = 1;
							ensqueue(sym_table, node->string);
							break;
						}
						ensqueue(sym_table, node->string);
					}
					if (declared == 0) {
					// No Error
						printf("StaticSem: Error: Using undeclared '%s'\n", proot->token[i]);
						free_parsetree(root);
						free_queue();
						free(sym_table);
						exit(EXIT_FAILURE);
					}
					break;
					// Yes do nothing
				}
			}
		}
		for (i = 0; i < proot->child_size; i++) {
			static_semantic(proot->child[i]);
		}
	}
	else
		return;
}

int verify_semtree(char* identifier){
	int i;
	squeue_node* node = desqueue(sym_table);
	for (i = 0; i < sym_table->size; i++) {
		if ((node = desqueue(sym_table))) {
			ensqueue(sym_table, node->string);
			return 1;
		}
		ensqueue(sym_table, node->string);
	}
	return 0;
}

void free_table(){
	int i, table_size;
	squeue_node* node;
	if (sym_table->size > 0) {
		table_size = sym_table->size;
		for (i = 0; i < table_size; i++) {
			node = desqueue(sym_table);
			free(node->string);
		}
		free(sym_table);
	}
}
