#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "rotation.h"

typedef struct{
    double x1;
    double y1;
    double x2;
    double y2;
}line;

typedef struct node_ll list_l;
struct node_ll{
    line value;
    list_l* next;
};

list_l* append_linked_list_l(list_l* L,line value);
void free_linked_list_l(list_l* L);

#endif
