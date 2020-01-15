#ifndef STACK_H
#define STACK_H

typedef struct stacknode_t {
	int data;
	struct stacknode_t* next;
}stacknode_t;

stacknode_t* new_node(int);
int isEmpty(stacknode_t*);
void push(stacknode_t**, int);
int pop(stacknode_t**);
int peek(stacknode_t*);

#endif /* end of include guard: STACK_H */
