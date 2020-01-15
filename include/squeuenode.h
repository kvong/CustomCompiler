#ifndef SQUEUENODE_H
#define SQUEUENODE_H

typedef struct squeue_node{
	char* string;
	struct squeue_node* next;
} squeue_node;

squeue_node* new_snode(char*);
#endif
