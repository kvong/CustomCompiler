#ifndef CQUEUE_H
#define CQUEUE_H
#include <cnode.h>

typedef struct cqueue_t {
	int size;
	struct cnode_t* front;
	struct cnode_t* rear;
}cqueue_t;

cqueue_t* init_cqueue();
cnode_t* decqueue(cqueue_t*);
int encqueue(cqueue_t*, char);

#endif /* end of include guard: CQUEUE_H */
