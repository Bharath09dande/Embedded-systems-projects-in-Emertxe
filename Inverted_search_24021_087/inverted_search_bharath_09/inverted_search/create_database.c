#include "inverted_search.h"

char *fname;

void create_database(Flist *f_head, Wlist *head[])
{
    //traverse through the file linked list
    while(f_head)
    {
        read_datafile(f_head , head , f_head->file_name);
        f_head = f_head->link;
    }
}

 Wlist* read_datafile(Flist *file, Wlist *head[], char *filename)
{
    fname = filename;

    //open the file read  mode 
    FILE *fptr = fopen(filename , "r");
    if(fptr==NULL)
    {
        printf("Error opening file : %s\n",filename);
        return NULL;
    }

    //declare an array to store the word
    char word[WORD_SIZE];

    while(fscanf(fptr , "%s" , word) != EOF)
    {
        int index = tolower(word[0]) % 97;
        int flag=1;
    	//other than alphabet 
	    if(!(index >= 0 && index <= 25))
	        index = 26;

	    //word is repeated or not
	    if(head[index] != NULL)
     	{
	        Wlist *temp = head[index];
	        //compare the words of each node with new word
	        while(temp)
	        {
	            if(!strcmp(temp->word,word))
	            {
      		        update_word_count(&temp , filename);
	     	        flag = 0;
		            break;
	            }
	            temp = temp->link;
	        }
	    }
	    if(flag == 1)
	    {
	        //if words are not repeated
    	    insert_at_last(&head[index] , word);
	    }
    }
}

int update_word_count(Wlist ** head, char * file_name)
{
    //........TODO........
    //check whether filenames are same or not
    // if filenames are same -> increment word_count
    //if filenames are different -> increment file_count , create TABLE NODE
    Wlist *temp = *head;
    Ltable *ltable = temp->Tlink;
    int found = 0;

    while(ltable)
    {
        if(strcmp(ltable->file_name,file_name) == 0)
        {
            ltable->word_count++;
            found=1;
            break;
        }
        ltable = ltable->table_link;
    }
    if(!found)
    {
      //  temp->file_count++;
        Ltable *new = malloc(sizeof(Ltable));
        if (new == NULL)
        {
            printf("Memory allocation failed\n");
            return FAILURE;
        }
        strcpy(new->file_name, file_name);
        new->word_count = 1;
            new->table_link = temp->Tlink;
            temp->Tlink = new;
            temp->file_count++;
    }
    return SUCCESS;
}
   
