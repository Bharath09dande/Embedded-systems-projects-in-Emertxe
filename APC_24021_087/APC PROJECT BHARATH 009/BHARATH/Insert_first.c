#include "apc.h"

int Insert_first(Dlist **headR,Dlist **tailR,int data)
{
   
    Dlist *new = malloc(sizeof(Dlist));
    if(new == NULL)
    {
        return FAILURE;
    }
    if((*headR == NULL) && (*tailR == NULL) )
    {
        *headR = new;
        *tailR = new; 
        new -> data = data;
        new -> next = NULL;
        new -> prev = NULL;
    }
    else
    {
        new -> data = data;
        new -> next = *headR;
        new -> prev = (*headR) -> prev;
        (*headR) -> prev = new;
        *headR = new;
    }
    return SUCCESS;
}