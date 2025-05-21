#include "apc.h"

int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR,char *argv[])
{
    
    int count = 0;
    printf("INFO : RESULT = ");
    while(1)
    {
        int List1_length =  length_of_list(head1,tail1);
        int List2_length =  length_of_list(head2,tail2);;

        if(List1_length < List2_length)
        {
            break;
        }
        else if(List1_length > List2_length)
        {
            count++;
            subtraction(head1,tail1,head2,tail2,headR,tailR);
            delete_List1(head1,tail1,headR,tailR);
            delete_zeros_in_list1(head1,tail1);
        }
        else
        {
            //when both List length are same, List1_length == List2_length
            //compare with digit by digit in the two list.
            Dlist *temp1 = *head1;
            Dlist *temp2 = *head2;
            int flag=0;
            for(int i = 0;i < List1_length;i++)
            {
                if((temp1 != NULL )&&(temp2 != NULL))
                { 
                    if( ( temp1 -> data ) > ( temp2 -> data ) )
                    {
                        flag = 1;
                        break;
                    }
                    else if( (temp1 -> data ) < (temp2 -> data))
                    {
                        flag = 0;
                        break;
                    }
                    else
                    {
                        flag = 1;
                        //when both the digits in the lists are same
                        temp1 = temp1 -> next;
                        temp2 = temp2 -> next;
                    }
                }
            }
            if(flag == 1)
            {
               
                count++;
                subtraction(head1,tail1,head2,tail2,headR,tailR);
                delete_List1(head1,tail1,headR,tailR);
                delete_zeros_in_list1(head1,tail1);
            }
            else
            {
                break;
            }
        }
    }
    if(count == 0)
    {
        printf("ERROR : The operand1 is less than operand2\n");
        return FAILURE;
    }
    printf("%d\n",count);
    return SUCCESS;

}

int  length_of_list(Dlist **head,Dlist **tail)
{
    Dlist *temp1 = *tail;
    
    int length = 0;
    while(temp1 != NULL)
    {
        (length++);
        temp1 = temp1 -> prev;
    }
    return length;
}

void  delete_List1(Dlist **head1,Dlist **tail1,Dlist **headR,Dlist **tailR)
{
    Dlist *temp1 = *tail1;
    while(temp1 != NULL)
    {
        if((temp1 -> prev == NULL) && (temp1 -> next == NULL))
        {
            free(temp1);
            *head1 = NULL;
            *tail1 = NULL;
            temp1 = NULL;
        }
        else
        {
            temp1 -> prev -> next = NULL;
            *tail1 = temp1 -> prev;
            free(temp1);
            temp1 = *tail1;
        }
    }
    // Connecting the back LIST up data to result1
    *head1 = *headR;
    *tail1 = *tailR;

    //after updating header make the Back_up_headR and Back_up_tailR as NULL
    *headR = NULL;
    *tailR = NULL;
}

void delete_zeros_in_list1(Dlist **head1,Dlist **tail1)
{
    Dlist *temp = *head1;

    while(temp != NULL)
    {
        if( (temp -> prev == NULL) && (temp -> next != NULL) )
        {
            if(temp -> data == 0)
            {
                temp -> next -> prev = NULL;
                (*head1) = temp -> next;
                free(temp);
                temp = *head1;
            }
            else
            {
                temp = temp -> next; 
            }
        }
       else
       {
         temp = temp -> next;
       }
    }
}