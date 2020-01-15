#ifndef CNODE_H
#define CNODE_H

typedef struct cnode_t {
	char key;
	struct cnode_t* next;
}cnode_t;

cnode_t* new_cnode(char);

#endif /* end of include guard: CNODE_H */
