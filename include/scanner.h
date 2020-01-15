#ifndef SCANNER_H
#define SCANNER_H
#include <token.h>
token_t scanner(char*, int*, int);
int get_class(char);
int get_new_state(int, int);
#endif /* end of include guard: SCANNER_H */
