#include "inverted_search.h"
extern char *fname;
int insert_at_last(Wlist **head, data_t *data)
{
    //create node 

    Wlist *new = malloc(sizeof(Wlist));

    //error check

    if(new == NULL)
    {
	    return FAILURE;
    }

    //update
    new->file_count =  1;
    strcpy(new->word,data);
    new->Tlink = NULL;
    new->link = NULL;

    //call fun to update link table for new node

    update_link_table(&new);

    //check Wlist is empty or not

    if(*head == NULL)
    {
	    *head = new;
      	return SUCCESS;
    }
    //not empty

    Wlist *temp = *head;

    //traverse through WList

    while(temp->link)
    {
	    temp = temp->link;
    }
    temp->link = new;
    return SUCCESS;

}

 int update_link_table(Wlist **head)
{
    //create a node

    Ltable *new = malloc(sizeof(Ltable));

    //error check
    if(new == NULL)
    {
	return FAILURE;
    }

    //UPDATE 
    new->word_count = 1;
    strcpy(new->file_name,fname);
    new->table_link = NULL;

    (*head)->Tlink = new;
    return SUCCESS;
}




