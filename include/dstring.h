#ifndef DSTRING_H
#define DSTRING_H 
#include <stdio.h>

char** tokenize(char*, char*);
char* d_readfile(FILE*);
char* d_readline(FILE*, int);
int get_dcplength(char**);
int get_cplength(char* str);
int get_fline(FILE*);
int d_writefile(FILE*, char*);

#endif
