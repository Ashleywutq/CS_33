/*
 * driver.h - Various definitions for the Performance Lab.
 * 
 * DO NOT MODIFY ANYTHING IN THIS FILE
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>

#define RIDX(i,j,n) ((i)*(n)+(j))

typedef struct {
  char *team;
  char *name1, *email1;
} user_t;

extern user_t user;

typedef struct {
   unsigned int key;
   unsigned int value;
} kvp;

bool kvp_compare(kvp lhs, kvp rhs);


typedef void (*lab_test_func) (int, kvp*, kvp*);

void setup();
void singlethread(int, kvp *, kvp *);
void multithread(int, kvp *, kvp *);

void register_multithread_functions(void);
void register_singlethread_functions(void);
void add_singlethread_function(lab_test_func, char*);
void add_multithread_function(lab_test_func, char*);

#endif /* _DEFS_H_ */

