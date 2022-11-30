#include "../../include/utils/stack.h"
/*
 * I use 1 queue for the head and his pred is the top of the queue
*/

stack* get_empty_stack()
{
    stack* res = malloc(sizeof(stack));
    res->pred = NULL;
    return res;
}

int is_empty_stack(stack* s)
{
    return s->pred == NULL;
}

void free_stack(stack* s)
{
    assert(s != NULL);
    while (s->pred != NULL)
    {
        stack* pred = s->pred;
        free(s);
        s = pred;
    }
    free(s);
}

void push_stack(stack* s, coor value)
{
    assert(s != NULL);
    stack* new = malloc(sizeof(stack));
    new->value = value;
    new->pred = s->pred;
    s->pred = new;
}


coor pop_stack(stack* s)
{
    assert(s->pred != NULL);
    stack* rm = s->pred;
    coor res = rm->value;
    s->pred = rm->pred;
    free(rm);
    return res;
}
