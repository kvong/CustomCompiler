#include <stdlib.h>
#include "squeue.h"
#include "squeuenode.h"

// Initialize queue
squeue_t* init_squeue(){
	squeue_t *q = (squeue_t*)malloc(sizeof(squeue_t));	
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

// Take out queue and return that node
squeue_node* desqueue(squeue_t* q){
	if (q->front == NULL){
		return NULL;
	}

	// Update front
	squeue_node* temp = q->front;
	q->front = q->front->next;
	q->size--;
	if (q->front == NULL){
		q->size = 0;
		q->rear = NULL;
	}
	return temp;
}

// Add item to queue
int ensqueue(squeue_t* q, char* string){
	squeue_node* temp = new_qnode(string);

	if (q->rear == NULL){
		q->front = temp;
		q->rear = temp;
		q->size = 1;
		return q->size;
	}
	else{
		q->rear->next = temp;
		q->rear = temp;
		q->size++;
	}
	return q->size;
}
