#ifndef TQUEUE_H
#define TQUEUE_H
#include <token.h>
#include <tnode.h>

typedef struct tqueue_t {
	int size;
	struct tnode_t* front;
	struct tnode_t* rear;
}tqueue_t;

tqueue_t* init_tqueue();
tnode_t* detqueue(tqueue_t*);
int entqueue(tqueue_t*, token_t);
void free_queue();


#endif /* end of include guard: TQUEUE_H */
