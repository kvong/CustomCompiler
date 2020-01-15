#include <stdio.h>
#include <parsenode.h>

void treePrint(parsenode_t* root, int depth){
	if (root != NULL) {
		printf("%*s", (depth+1)*2, root->label);
		int i;
		for (i = 0; i < root->token_size; i++) {
			printf(" %s", root->token[i]);
		}
		for (i = 0; i < root->token_size; i++) {
			if (strcmp(root->token[i], "") != 0) {
				printf(" %d", root->tokenID[i]);
			}
		}
		printf("\n");
		for (i = 0; i < root->child_size; i++) {
			treePrint(root->child[i], depth + 1);
		}
	}
	else{
		return;
	}
}
