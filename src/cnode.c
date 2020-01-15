#include <stdlib.h>
#include <stdio.h>
#include "cnode.h"

// Create a queue node
cnode_t* new_cnode(char key){
	cnode_t* temp = (cnode_t*)malloc(sizeof(cnode_t));
	temp->key = key;
	temp->next = NULL;
	return temp;
}
