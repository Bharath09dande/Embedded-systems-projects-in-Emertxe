#include "apc.h"

int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
    int sum = 0,data = 0,carry = 0,num1 = 0,num2 = 0;
    
    while((temp1 != NULL) || (temp2 != NULL))
    {
        if(temp1 == NULL)
        {
            num1 = 0;   
        }
        else
        {
            num1 = temp1 -> data;
            temp1 = temp1 -> prev; 
        }
        
        if(temp2 == NULL)
        {
            num2 = 0;
        }
        else
        {
            num2 = temp2 -> data; 
            temp2 = temp2 -> prev;
        }   
    
        sum = num1 + num2 + carry;
        data = sum % 10;

        carry = sum / 10;
        Insert_first(headR,tailR,data);
        
    }
    if(carry > 0)
    {
        Insert_first(headR,tailR,carry);
    }

    return SUCCESS;
}
