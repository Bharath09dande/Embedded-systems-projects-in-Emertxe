#include "apc.h"

int main(int argc, char *argv[])
{
	/* Declare the pointers */
      
	Dlist *head1=NULL, *tail1=NULL;
    Dlist *head2=NULL, *tail2=NULL; 
    Dlist *headR=NULL, *tailR=NULL;

	//for multiplication we created nodes
	Dlist *headR2 = NULL, *tailR2 = NULL; 
	int res = Read_and_validate_CLA(argc,argv);
	if(res == 0)
	{
		printf("INFO : Read and validate command line args successfully\n");
	}
	else
	{
		printf("ERROR: Failed to read and validate command line args\n");
		exit(0);
	}
	
	char operator=argv[2][0];

		switch (operator)
		{
			case '+':
				/* call the function to perform the addition operation */
				digit_beginning_zeros(argv[1],argv[3]);
				if(insert_digit(&head1,&tail1,&head2,&tail2,argv[1],argv[3]) == 0)
				{
					printf("INFO : Inserted the data successfully \n");
				}
				else
				{
					printf("ERROR : Failed to insert the data\n");
				}
				if(addition(&head1,&tail1,&head2,&tail2,&headR,&tailR) == 0)
				{
					print_result_data(headR,tailR,0);	
					printf("\n");
					printf("INFO : Addition operation done successfully\n");
				}
				else
				{
					printf("ERROR : Failed to do addition operation\n");
				}
				break;
			case '-':
				/* call the function to perform the subtraction operation */
				digit_beginning_zeros(argv[1],argv[3]);
				if(validate_subtraction_operation(head1,tail1,head2,tail2,headR,tailR,argv) == 0)
				{
					printf("\n");
					printf("INFO : Subtraction operation done successfully\n");
				}
				else
				{
					printf("ERROR : Failed to do subtraction operation\n");
				}
				break;
			case 'x':	
				/* call the function to perform the multiplication operation */
				digit_beginning_zeros(argv[1],argv[3]);
				if(insert_digit(&head1,&tail1,&head2,&tail2,argv[1],argv[3]) == 0)
				{
					printf("INFO : Inserted the data successfully\n");
				}
				else
				{
					printf("ERROR : Failed to insert the data\n");
				}
				if(multiplication(&head1,&tail1,&head2,&tail2,&headR,&tailR,&headR2,&tailR2) == 0)
				{
					print_result_data(headR,tailR,0);
					printf("\n");
					printf("INFO : Multiplication operation done successfully\n");
				}
				else
				{
					printf("ERROR : Failed to do multiplication operation\n");
				}
				break;
			case '/':
				digit_beginning_zeros(argv[1],argv[3]);
				if(insert_digit(&head1,&tail1,&head2,&tail2,argv[1],argv[3]) == 0)
				{
					printf("INFO : Inserted the data successfully\n");
				}
				else
				{
					printf("ERROR : Failed to insert the data\n");
				}
				/* call the function to perform the division operation */
				if(division(&head1,&tail1,&head2,&tail2,&headR,&tailR,argv) == 0)
				{
					printf("INFO : Division operation done successfully\n");
				}
				else
				{
					printf("ERROR : Failed to do division operation\n");
				}
				break;
			default:
				printf("ERROR : Invalid Input: Try again...\n");
		}
	return 0;
}

int Read_and_validate_CLA(int argc, char *argv[])
{
	if(argc == 4)
	{
		char *ptr1 = argv[1];
		int i = 0;
		while(ptr1[i] != '\0')
		{
			if(!isdigit(ptr1[i]))
			{
				printf("ERROR: Entered argument has characters\n");
				return FAILURE;
			}
			i++;
		}
		char *ptr2 = argv[3];
		int j = 0;
		while(ptr2[j] != '\0')
		{
			if(!isdigit(ptr2[j]))
			{
				printf("ERROR: Entered arguments has characters\n");
				return FAILURE;
			}
			j++;
		}
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}

}

void print_list(Dlist *head)
{
	/* Cheking the list is empty or not */
	if (head == NULL)
	{
		printf("INFO : List is empty\n");
	}
	else
	{
	    printf("Head -> ");
	    while (head)		
	    {
		    /* Printing the list */
		    printf("%d <-", head -> data);

		    /* Travering in forward direction */
		    head = head -> next;
		    if (head)
		        printf("> ");
	    }
    	printf(" Tail\n");
    }
}

void digit_beginning_zeros(char *str1,char *str2)
{
    int n = strlen(str1),m = strlen(str2),i;
    
    for(i = 0;i < n ;i++)
    {
        if(str1[i] == '0')
        {
            int j = i;
            if(str1[j+1] ==  '\0')
            {
                break;
            }
            while(str1[j] != '\0')
            {
                str1[j] = str1[j+1];
                j++;
            }
            i--;
        }
       else
        {
            break;
        }
     }
    int p;
    for(p = 0; p < m; p++)
    {
        if(str2[p] == '0')
        {
            if(str2[p+1] == '\0')
            {
                break;
            }
            int q = p;
            while(str2[q] != '\0')
            {
                str2[q] = str2[q+1];
                q++;
            }
            p--;
        }
        else
        {
            break;
        }
    }
}
