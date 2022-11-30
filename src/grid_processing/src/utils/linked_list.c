#include "../../include/utils/linked_list.h"

list_l* append_linked_list_l(list_l* L,line value)
{
    list_l* res = malloc(sizeof(struct node_ll));
    res->value = value;
    res->next = NULL;
    if (L == NULL)
        return res;
    list_l*  tmp = L;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = res;
    return L;
}


void free_linked_list_l(list_l* L)
{
    while (L != NULL)
    {
        list_l* next = L->next;
        free(L);
        L = next;
    }
}
