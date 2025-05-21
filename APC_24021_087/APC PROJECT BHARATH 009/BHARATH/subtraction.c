#include "apc.h"

int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    
    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;
        
    int num1 = 0,num2 = 0,borrow = 0,data = 0;
    while( (temp1 != NULL) )
    {
        num1 = temp1 -> data;
         temp1 = temp1 -> prev;
        if(temp2 == NULL)
        {
            num2 = 0;
        }
        else
        {
            num2 = temp2 -> data;
            temp2 = temp2 -> prev;
        }
        if(borrow == 1)
        {
            num1 = num1 - 1;
            borrow = 0;
        }
        if(num1 < num2)
        {
            num1 = num1 + 10;
            borrow = 1;
        }
        data = num1 - num2;
        Insert_first(headR,tailR,data);
    }    
    return SUCCESS;
}
