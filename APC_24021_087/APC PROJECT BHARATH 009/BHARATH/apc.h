#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
	struct node *prev;
	int data;
	struct node *next;
}Dlist;

/* Include the prototypes here */
/*Read and validate command line arguments*/
int Read_and_validate_CLA(int argc, char *argv[]);
/*printing the list*/
void print_list(Dlist *head);
/*printing result data*/
void print_result_data(Dlist *headR,Dlist *tailR,int sign);
/*inserting the data into the list*/
int Insert_first(Dlist **headR,Dlist **tailR,int data);
/*store the operands into the list */
int insert_digit(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *ptr1,char *ptr2);
/*Addition */
int addition(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);
/*validate the subtraction*/
int validate_subtraction_operation(Dlist *head1,Dlist *tail1,Dlist *head2,Dlist *tail2,Dlist *headR,Dlist *tailR,char *argv[]);
/*removing zeros at beginning*/
void digit_beginning_zeros(char *str1,char *str2);
/*subtraction*/
int subtraction(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);
/*Multiplication*/
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR1, Dlist **tailR1, Dlist **headR2,Dlist **tailR2);
/*removing data in the list*/
void  delete_result_list(Dlist **headR1,Dlist **tailR1,Dlist **headR2,Dlist **tailR2,Dlist **Back_up_headR,Dlist **Back_up_tailR);
/*Division */
int division(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR,char *argv[]);
/*length of the list*/
int  length_of_list(Dlist **head,Dlist **tail);
/*removing the data in the list*/
void  delete_List1(Dlist **headR1,Dlist **tailR1,Dlist **headR,Dlist **tailR);
/*removing the zeros in the list*/
void delete_zeros_in_list1(Dlist **head1,Dlist **tail1);
#endif
