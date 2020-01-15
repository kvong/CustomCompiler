#include <stdlib.h>
#include "cqueue.h"
#include "cnode.h"

// Initialize queue
cqueue_t* init_cqueue(){
	cqueue_t *q = (cqueue_t*)malloc(sizeof(cqueue_t));	
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

// Take out queue and return that node
cnode_t* decqueue(cqueue_t* q){
	if (q->front == NULL){
		return NULL;
	}

	// Update front
	cnode_t* temp = q->front;
	q->front = q->front->next;
	q->size--;
	if (q->front == NULL){
		q->size = 0;
		q->rear = NULL;
	}
	return temp;
}

// Add item to queue
int encqueue(cqueue_t* q, char key){
	cnode_t* temp = new_cnode(key);

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
