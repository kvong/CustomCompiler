#include <stdlib.h>
#include <stdio.h>
#include "squeuenode.h"

// Create a queue node
squeue_node* new_qnode(char* string){
	squeue_node* temp = (squeue_node*)malloc(sizeof(squeue_node));
	temp->string = string;
	temp->next = NULL;
	return temp;
}
