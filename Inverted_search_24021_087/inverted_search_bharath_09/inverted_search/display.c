#include "inverted_search.h"

void display_database( Wlist *head[] )
{
    printf("[index]   [word]    file_count    file/s:  File:File_name   word_count\n");
    printf("----------------------------------------------------------------------------\n");

   for(int i = 0; i < 27; i++)
   {
       if(head[i] != NULL)
       {
	        print_word_count(head[i]);
       }
   }
}
 int print_word_count(Wlist *head)
{
    //traverse through WList 
    while(head != NULL)
    {
        printf(" [%d]     [%s]          %d           file/s   " , tolower(head->word[0]) % 97 , head->word , head->file_count);
	    Ltable *Thead = head->Tlink;

	    //traverse
	    while(Thead)
	    {
	        printf("File : %s     %d" , Thead->file_name , Thead->word_count);
	        Thead = Thead->table_link;
	    }
        printf("\n");

        head = head->link;
    }
    printf("-----------------------------------------------------------------------------\n");
}






