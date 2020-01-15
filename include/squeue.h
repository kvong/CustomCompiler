#ifndef SQUEUE_H
#define SQUEUE_H
#include "squeuenode.h"

typedef struct squeue_t{
	int size;
	struct squeue_node* front;
	struct squeue_node* rear;
} squeue_t;

squeue_t* init_squeue();
squeue_node* desqueue(squeue_t*);
int ensqueue(squeue_t*, char*);

#endif

