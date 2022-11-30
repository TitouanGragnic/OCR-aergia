#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct
{
    int x,y;
}coor;


typedef struct node_s stack;
struct node_s{
    coor value;
    stack * pred;
};

stack* get_empty_stack();
int is_empty_stack(stack* s);
void free_stack(stack* s);
void push_stack(stack* s, coor value);
coor pop_stack(stack* s);

#endif
