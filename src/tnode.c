#include <stdlib.h>
#include <stdio.h>
#include <tnode.h>
#include <token.h>

tnode_t* new_tnode(token_t token){
	tnode_t* tmp = (tnode_t*)malloc(sizeof(tnode_t));
	tmp->token = token;
	tmp->next = NULL;
	return tmp;
}
