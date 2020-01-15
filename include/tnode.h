#ifndef TNODE_H
#define TNODE_H
#include <token.h>

typedef struct tnode_t {
	token_t token;
	struct tnode_t* next;
}tnode_t;

tnode_t* new_tnode(token_t);

#endif /* end of include guard: TNODE_H */
