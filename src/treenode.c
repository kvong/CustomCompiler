#include <stdlib.h>
#include <string.h>
#include <parsenode.h>

parsenode_t* init_parsenode(int children, int tokens){
	int i;
	parsenode_t* tmp = (parsenode_t*)malloc(sizeof(parsenode_t));
	if (!tmp) {
		perror("TreeNode: Error");
		exit(EXIT_FAILURE);
	}

	tmp->child_size = children;
	tmp->token_size = tokens;
	tmp->label = "";
	tmp->child = (parsenode_t**)malloc(sizeof(parsenode_t*)*children);
	if (!tmp->child) {
		perror("TreeNode: Error");
		exit(EXIT_FAILURE);
	}
	tmp->token = malloc(sizeof(char**)*tokens);
	for (i = 0; i < tokens; i++) {
		tmp->token[i] = (char*)malloc(sizeof(char)*10);
	}
	return tmp;
}

void free_parsetree(parsenode_t* root){
	if (root != NULL) {
		int i;
		for (i = 0; i < root->token_size; i++) {
			free(root->token[i]);
		}
		for (i = 0; i < root->child_size; i++) {
			free_parsetree(root->child[i]);
			free(root->child[i]);
		}
		free(root->child);
		free(root->token);
	}
	else{
		return;
	}

}
