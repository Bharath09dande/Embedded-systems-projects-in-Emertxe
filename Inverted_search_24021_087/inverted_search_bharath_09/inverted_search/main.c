#include "inverted_search.h"

int main(int argc , char *argv[])
{
    system("clear");

    Wlist *head[27]={NULL} ;
    int index;

    //validate CLA
    if(argc <= 1)
    {
        printf("Enter the valid no. arguments\n");
        printf("./Slist.exe file1.txt file2.txt .........\n");
        return 0;
    }

    //declare head pointer
    Flist *f_head = NULL;

    //file validation
    file_validation_n_file_list(&f_head , argv);

    if(f_head == NULL)
    {
        printf("No files added to file linked list\n");
        printf("Hence process got terminated\n");
        return 0;
    }

    char option;
    do
    {
       // printf("successfully inserting file name: %s into file linked list\n",argv[1]);
        printf("select your choice among the following options:\n");
        printf("1.Create Database\n");
        printf("2.Display Database\n");
        printf("3.Update Database\n");
        printf("4.Search\n");
        printf("5.Save Database\n");
        printf("6.Exit\n");
        printf("Enter your choice: \n");
        
      	char word[WORD_SIZE];
        int choice;
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
   	            create_database(f_head , head);
                break;
            case 2:
                display_database(head);
                break;
            case 3:    
            	update_database(head,&f_head);
                break;
            case 4:
            	//search
            	printf("Enter the word name\n");
            	scanf("%s" , word);
            	//find the index
                index = tolower(word[0]) % 97;
	            search(head[index] , word); 
                break;
            case 5:
              	save_database(head);
                break;
            case 6:
                return 0;
        }
        printf("Do you want to continue?\n");
        printf("Enter y/Y to continue or n/N to discontinue:");
        scanf(" %c",&option);

     } while(option == 'y' || option == 'Y');

}
