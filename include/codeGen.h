#ifndef CODEGEN_H
#define CODEGEN_H
#include <parsenode.h>
#include <stdio.h>
#include <stack.h>
#include <cqueue.h>

static int temp_counter;
static int label_counter;
static FILE *file_ptr;
static int first_bit;
static int first_lock;
static int abs_loader;
static stacknode_t* label_stack;
static stacknode_t* breaker_stack;
static cqueue_t* op_queue;

void run_codeGen(char*, parsenode_t*);
void codeGeneration( parsenode_t* , int );
int isGenerator(char);

#endif /* end of include guard: CODEGEN_H */
