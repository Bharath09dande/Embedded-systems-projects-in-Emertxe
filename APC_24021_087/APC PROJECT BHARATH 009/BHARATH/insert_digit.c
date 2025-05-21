#include "apc.h"

int insert_digit(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *ptr1,char *ptr2)
{
	Dlist *new = malloc(sizeof(Dlist));
	if(new == NULL)
	{
		return FAILURE;
	}
	int i = 0;
	if(*head1 == NULL)
	{
		new -> data = (ptr1[i] - 48);
		new -> prev = NULL;
		new -> next = NULL;
		*head1 = new;
		*tail1 = new;
		i++;
	}
	while(ptr1[i] != '\0')
	{
		Dlist *new = malloc(sizeof(Dlist));
		if(new == NULL)
		{
		return FAILURE;
		}
		new -> data = (ptr1[i] - 48);
		new -> next = NULL;
		(*tail1) -> next = new;
		new -> prev = *tail1;
		*tail1 = new;
		i++;  
	}
	int j = 0;
	Dlist *new1 = malloc(sizeof(Dlist));
	if(new1 == NULL)
	{
		return FAILURE;
	}
	if(*head2 == NULL)
	{
		new1 -> data = (ptr2[j] - 48);
		new1 -> prev = NULL;
		*head2 = new1;
		*tail2 = new1;
		j++;
	}
	while(ptr2[j] != '\0')
	{
		Dlist *new1 = malloc(sizeof(Dlist));
		if(new1 == NULL)
		{
			return FAILURE;
		}
		new1 -> data = (ptr2[j] - 48);
		new1 -> next = NULL;
		(*tail2) -> next = new1;
		new1 -> prev = *tail2;
		*tail2 = new1;
		j++;  
	}
	return SUCCESS;
}