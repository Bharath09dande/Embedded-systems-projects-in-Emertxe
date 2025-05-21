#include "inverted_search.h"

void file_validation_n_file_list(Flist **f_head, char *argv[])
{
    int i = 1 , empty;
    while(argv[i] != NULL)
    {
      	empty = isFileEmpty(argv[i]);
	    if(empty == FILE_NOTAVAILABLE )
    	{
	        printf("FIle : %s is not available \n" , argv[i]);
	        printf("Hence we are not adding that file into file linked list\n");
	        i++;
	        continue;
	    }
	    else if(empty == FILE_EMPTY)
	    {
	        printf("FIle : %s is not having any content \n" , argv[i]);
	        printf("Hence we are not adding that file into file linked list\n");
	        i++;
	        continue;
	    }
        //if the file is non empty
	    else
	    {
	        int ret_val = to_create_list_of_files(f_head , argv[i]);

	        if(ret_val == SUCCESS )
	        {
		         printf("Successfull : inserting the file name %s into file linked list\n" , argv[i]);
	        }
	        else if (ret_val == REPEATATION)
	        {
		        printf("THis file name : %s is repeated\n" , argv[i]);
		        printf("Hence we are not adding that file\n");
	        }
	        else
	        {
		        printf("Failure\n");
	        }

	        i++;
	        continue;
	    }
    }
}

//fun will check for file availability anf checks for file content
int isFileEmpty(char *filename)
{
    FILE *fptr = fopen(filename , "r");

    if(fptr == NULL)
    {
    	if(errno == ENOENT)
	    {
	        return FILE_NOTAVAILABLE;
	    }
    }
    //check for contents

    fseek(fptr , 0 , SEEK_END);
    if(ftell(fptr) == 0)
    {
	     return FILE_EMPTY;
    }
}

 int to_create_list_of_files(Flist **f_head, char *name)
{
    //........TODO........
    // check for duplicancy
    Flist *temp=*f_head;
    while(temp!=NULL)
    {
        if(strcmp(temp->file_name,name)==0)
            return FAILURE;
         temp=temp->link;
    }

   // insert_last
    Flist *new=malloc(sizeof(Flist));
    if(new==NULL)
    {
        return FAILURE;
    }
    strcpy(new->file_name,name);
    new->link=NULL;
    if(*f_head==NULL)
    {
        *f_head=new;
        return SUCCESS;
    }
    Flist *temp1=*f_head;
    while(temp1->link!=NULL)
    {
       temp1=temp1->link;
    }
    temp1->link=new;
    return SUCCESS;
}
