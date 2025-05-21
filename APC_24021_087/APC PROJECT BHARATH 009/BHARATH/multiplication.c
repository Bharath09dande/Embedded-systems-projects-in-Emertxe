#include "apc.h"

int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR1, Dlist **tailR1, Dlist **headR2,Dlist **tailR2)
{
    Dlist *temp1 = NULL ;
    Dlist *temp2 = *tail2;
    //backup LIST
    Dlist *Back_up_headR = NULL;
    Dlist *Back_up_tailR = NULL;
    int num1 = 0,num2 = 0,carry = 0,count = 0,mul = 0,data = 0;

    while(temp2 != NULL)
    {
       temp1 = *tail1;
       while(temp1 != NULL)
       {
            num1 = temp1 -> data;
            num2 = temp2 -> data;
            mul = num1*num2 + carry;
            data = mul % 10;
            carry = mul / 10;
            temp1 = temp1 -> prev;
            if(count == 0)
            {
                Insert_first(headR1,tailR1,data);
            }
            else
            {
                Insert_first(headR2,tailR2,data);
            }
       }
       count ++;
       if(count > 1)
       {
            
            if(carry > 0)
            {
                Insert_first(headR2,tailR2,carry);
                carry = 0;
            }
            addition(headR1,tailR1,headR2,tailR2,&Back_up_headR,&Back_up_tailR);

            delete_result_list(headR1,tailR1,headR2,tailR2,&Back_up_headR,&Back_up_tailR);
            
            for(int i = 0;i < count ;i++)
            {
                Insert_first(headR2,tailR2,0);
            }
       }
       else
       {
           
            for(int i = 0;i < count ;i++)
            {
                Insert_first(headR2,tailR2,0);
            }
            if(carry > 0)
            {
                Insert_first(headR1,tailR1,carry);
                carry = 0;
            }
       }
        
       temp2 = temp2 -> prev;
       
    }
    
    return SUCCESS;
}

void  delete_result_list(Dlist **headR1,Dlist **tailR1,Dlist **headR2,Dlist **tailR2,Dlist **Back_up_headR,Dlist **Back_up_tailR)
{
    Dlist *temp1 = *tailR1;
    Dlist *temp2 = *tailR2;

    while(temp1 != NULL)
    {
        if((temp1 -> prev == NULL) && (temp1 -> next == NULL))
        {
            free(temp1);
            *headR1 = NULL;
            *tailR1 = NULL;
            temp1 = NULL;
        }
        else
        {
            temp1 -> prev -> next = NULL;
            *tailR1 = temp1 -> prev;
            free(temp1);
            temp1 = *tailR1;
        }
    }

    while(temp2 != NULL)
    {
        if((temp2 -> prev == NULL) && (temp2 -> next == NULL))
        {
            free(temp2);
            *headR2 = NULL;
            *tailR2 = NULL;
            temp2 = NULL;
        }
        else
        {
            temp2 -> prev -> next = NULL;
            *tailR2 = temp2 -> prev;
            free(temp2);
            temp2 = *tailR2;
        }
    }
    // Connecting the back_up_LIST  to result1
    *headR1 = *Back_up_headR;
    *tailR1 = *Back_up_tailR;

    //after updating header make the Back_up_header and Back_up_tailR as NULL
    *Back_up_headR = NULL;
    *Back_up_tailR = NULL;

}