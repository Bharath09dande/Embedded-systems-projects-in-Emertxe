#include "apc.h"

void print_result_data(Dlist *headR,Dlist *tailR,int sign)
{
	Dlist *temp = headR;
	printf("INFO : RESULT = ");
	while(temp != NULL)
	{
		if(sign == 1)
		{
			if(temp -> prev == NULL && temp -> next != NULL)
			{
				if(temp -> data == 0)
				{
					temp -> next -> prev = NULL;
					headR = temp -> next;
					free(temp);
					temp = headR;
				}
				else
				{
					printf("%d",temp -> data);
					temp = temp -> next;
				}
			}
			else
			{
				printf("%d",temp -> data);
				temp = temp -> next;
			}
		}
		else if(sign == -1)
		{
			if( (temp -> prev == NULL) && (temp -> next != NULL) )
			{
				if(temp -> data == 0)
				{
					temp -> next -> prev = NULL;
					headR = temp -> next;
					free(temp);
					temp = headR;
				}
				else
				{
					printf("-");
					printf("%d",temp -> data);
					temp = temp -> next;
				}
			}
			else if( (temp -> prev == NULL) && (temp -> next == NULL) )
			{
				printf("-");
				printf("%d",temp -> data);
				temp = temp -> next;
			}
			else
			{
				printf("%d",temp -> data);
				temp = temp -> next;
			}
		}
		else
		{
			printf("%d",temp -> data);
			temp = temp -> next;
		}
	}
}