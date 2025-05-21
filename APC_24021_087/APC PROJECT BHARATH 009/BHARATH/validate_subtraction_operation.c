#include "apc.h"

int validate_subtraction_operation(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist *headR,Dlist *tailR,char *argv[])
{
	int sign_flag;
	//checking and comparing the length of the strings arv[1],argv[3]
	int operand1 = strlen(argv[1]), operand2 = strlen(argv[3]);
	if( operand1 > operand2 )
	{
		sign_flag = 1;
		if(insert_digit(&head1,&tail1,&head2,&tail2,argv[1],argv[3]) == 0)
		{
			if(subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR) == 0)
			{	
				print_result_data(headR,tailR,sign_flag);	
			}
			return SUCCESS;
		}
		else
		{
			printf("ERROR : Failed to add  substraction data in the list\n");
			return FAILURE;
		}
	}
	else if(operand1 < operand2)
	{
		sign_flag = -1;
		if(insert_digit(&head1,&tail1,&head2,&tail2,argv[3],argv[1]) == 0)
		{
			if(subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR) == 0)
			{
				print_result_data(headR,tailR,sign_flag);		
				printf("INFO : Subtraction operation done successfully\n");
			}
			else
			{
				printf("ERROR : Failed to do subtraction\n");
			}
			return SUCCESS;
		}
		else
		{
			printf("ERROR : Failed to add  substraction data in the list\n");
			return FAILURE;
		}
	}
	else
	{
		//compare the each characters when the strings length are equal.
		int cmpvalue = strcmp(argv[1],argv[3]) ;
		if( cmpvalue > 0)
		{
			sign_flag = 1;
			if(insert_digit(&head1,&tail1,&head2,&tail2,argv[1],argv[3]) == 0)
			{
				if(subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR) == 0)
				{
					print_result_data(headR,tailR,sign_flag);
				}
				return SUCCESS;
			}
			else
			{
				printf("ERROR : Failed to add  substraction data in the list\n");
				return FAILURE;
			}
		}
		else if( cmpvalue < 0)
		{
			sign_flag = -1;
			if(insert_digit(&head1,&tail1,&head2,&tail2,argv[3],argv[1]) == 0)
			{
				if(subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR) == 0)
				{
					print_result_data(headR,tailR,sign_flag);		
				}
				return SUCCESS;
			}
			else
			{
				printf("ERROR : Failed to add  substraction data in the list\n");
				return FAILURE;
			}
		}
		else
		{
			sign_flag = 0;
			//both characters in two strings are same.
			if(Insert_first(&headR,&tailR,0) == 0)
			{
				print_result_data(headR,tailR,sign_flag);
				return SUCCESS;
			}
			else
			{
				printf("ERROR : Inserting result at first is failure\n");
				return FAILURE;
			}
		}
	}
}