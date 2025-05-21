#include "inverted_search.h"

void search(Wlist *head, char *word)
{
    //check list is empty or not
    if(head == NULL)
    {
		printf("List is empty , search not possible\n");
		return;
    }

    //non empty , traverse through Wlist
    while(head)
    {
		//compare the search word with each node word
		if(!strcmp(head->word , word))
		{
			printf("Word %s is present in %d file/s\n" , word , head->file_count);
			Ltable *Thead = head->Tlink;

			while(Thead)
			{
			printf("In file : %s    %d time/s \n" , Thead->file_name , Thead->word_count);
			Thead = Thead->table_link;
			}
			return;
		}

		head = head->link;
    }

    printf("Search word not found\n");
}



