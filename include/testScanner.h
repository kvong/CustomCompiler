#ifndef TESTSCANNER_H
#define TESTSCANNER_H
#include <stdio.h>
#include <token.h>
#include <tqueue.h>

tqueue_t* testScan(char*);
char* filter_one(FILE*, char);
char* tab_remove(char*);
#endif /* end of include guard: TESTSCANNER_H */
