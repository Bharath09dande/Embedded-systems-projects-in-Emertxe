#include "inverted_search.h"

void save_database( Wlist *head[])
{
    int i;
    char file_name[FNAME_SIZE];
    printf("enter the file name\n");
    scanf("%s" , file_name);

    // open file
    FILE *fptr = fopen(file_name , "w" );

    for(i = 0; i< 27; i++)
    {
	    if(head[i] != NULL)
	    {
	        write_databasefile(head[i] , fptr);
	    }
    }
    fclose(fptr);
    printf("Database saved\n");
}
void write_databasefile(Wlist *head, FILE *databasefile) {
    // Traverse through each node in the linked list
    while (head != NULL) {
        // Calculate the index and handle non-alphabetic characters safely
        int index = (isalpha(head->word[0])) ? (tolower(head->word[0]) % 97) : 26;

        // Write data to the file
        fprintf(databasefile, "#\n[%d] word is [%s] : file count is %d : ",index, head->word, head->file_count);

        // Traverse the linked list of files for the current word
        Ltable *Thead = head->Tlink;
        while (Thead != NULL) {
            fprintf(databasefile, "file is [%s] : word count %d ",Thead->file_name, Thead->word_count);
            Thead = Thead->table_link;
        }

        fprintf(databasefile, "\n");
        head = head->link;
    }
}

