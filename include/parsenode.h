#ifndef PARSENODE_H
#define PARSENODE_H

typedef struct parsenode_t {
	char* label;
	struct parsenode_t** child;
	char** token;
	int* tokenID;
	int child_size;
	int token_size;
}parsenode_t;

parsenode_t* init_parsenode(int, int);
void free_parsetree(struct parsenode_t*);

#endif /* end of include guard: TREENODE_H */
