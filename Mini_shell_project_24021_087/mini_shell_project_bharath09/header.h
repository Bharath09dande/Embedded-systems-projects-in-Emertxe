#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
 
#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3
#define UNKNOWN_CMD 4

/*Linked list to store the signal info*/
typedef struct SignalNode 
{
    int pid;
    char command[100];//input_string
    struct SignalNode *link;//next
} SignalNode;


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);
void execute_external_commands(char *input_string);
void n_pipe(char **input,int pipe_count,int pipe_index_array[]);

void add_signal_to_list(char *input_string,int pid);
void display_signals();
int resume_signal();

#endif