#include <stdlib.h>
#include <tqueue.h>
#include <tnode.h>
#include <parser.h>

// Initialize queue
tqueue_t* init_tqueue(){
	tqueue_t* q = (tqueue_t*)malloc(sizeof(tqueue_t));	
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

// Take out queue and return that node
tnode_t* detqueue(tqueue_t* q){
	if (q->front == NULL){
		return NULL;
	}

	// Update front
	tnode_t* temp = q->front;
	q->front = q->front->next;
	q->size--;
	if (q->front == NULL){
		q->size = 0;
		q->rear = NULL;
	}
	return temp;
}

// Add item to queue
int entqueue(tqueue_t* q, token_t token){
	tnode_t* temp = new_tnode(token);

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

void free_queue(){
	int i;
	for (i = 0; i < tokens->size-1; i++) {
		t_node = detqueue(tokens);
		free(t_node->token.token);
	}
	free(t_node);
}
