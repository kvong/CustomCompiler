#ifndef STATICSEM_H
#define STATICSEM_H
#include <parsenode.h>
#include <squeue.h>

squeue_t* sym_table;

void test_semantic(parsenode_t* root);
void static_semantic(parsenode_t* root);
int verify_semparse(parsenode_t*, char*);

void free_table();

#endif /* end of include guard: STATICSEM_H */
