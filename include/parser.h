#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <token.h>
#include <tnode.h>
#include <tqueue.h>
#include <parsenode.h>

tqueue_t* tokens;
tnode_t* t_node;
parsenode_t* root;

parsenode_t* parser(char*);
parsenode_t* S();
parsenode_t* B();
parsenode_t* V();
parsenode_t* M();
parsenode_t* H();
parsenode_t* R();
parsenode_t* Q();
parsenode_t* T();
parsenode_t* A();
parsenode_t* W();
parsenode_t* I();
parsenode_t* G();
parsenode_t* E();
parsenode_t* Z();

#endif /* end of include guard: PARSER_H */
