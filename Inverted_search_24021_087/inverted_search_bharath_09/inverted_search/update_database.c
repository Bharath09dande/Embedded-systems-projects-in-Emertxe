#include"inverted_search.h"

void update_database( Wlist *head[], Flist **f_head)
{
    //prompt the user for new file to update the database
    char file_name[FNAME_SIZE];
    printf("Enter the new file name\n");
    scanf("%s" , file_name);

    //file validation 
    //........TODO......
    int empty = isFileEmpty(file_name);
	if(empty == FILE_NOTAVAILABLE )
	{
	    printf("FIle : %s is not available \n" , file_name);
	    printf("Hence we are not adding that file into file linked list\n");	   
	}
	else if(empty == FILE_EMPTY)
	{
	    printf("FIle : %s is not having any content \n" , file_name);
	    printf("Hence we are not adding that file into file linked list\n");
	}
	else
	{
	    int ret_val = to_create_list_of_files(f_head , file_name);

	    if(ret_val == SUCCESS )
	    {
			printf("Successfull : inserting the file name %s into file linked list\n" , file_name);
	    }
	    else if (ret_val == REPEATATION)
	    {
			printf("THis file name : %s is repeated\n" , file_name);
			printf("Hence we are not adding that file\n");
	    }
	    else
	    {
			printf("Failure\n");
	    }
	}

    while(*f_head)
    {
		if(!strcmp((*f_head)->file_name , file_name))
		{
			create_database(*f_head , head);
		}
		*f_head = ((*f_head)->link);
    }
}


