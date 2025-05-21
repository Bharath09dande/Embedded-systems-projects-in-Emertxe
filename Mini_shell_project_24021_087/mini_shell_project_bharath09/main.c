#include "header.h"

char *external_commands[200];
char input_string[50];
char prompt[20];
SignalNode signal_link;
int status;
pid_t pid1;

int main(){
    system("clear");//to clear the terminal
    char prompt[20]="[minishell]$";
    struct sigaction act;
    act.sa_handler = signal_handler;
    // Register one handler for multiple signals
    sigaction(SIGINT, &act,NULL);   // Handles Ctrl+C
    sigaction(SIGTSTP, &act,NULL);  // Handles Ctrl+Z
    scan_input(prompt,input_string);
    return 0;
}
void scan_input(char *prompt, char *input_string){
    extract_external_commands(external_commands);
    while(1){
        //print prompt
        printf(ANSI_COLOR_GREEN "%s:~",prompt);
        //scan input string
        fgets(input_string,50,stdin);//fgets stores '\n' character also to remove it we added '\0' at end of the input_string
        input_string[strlen(input_string)-1]='\0';
        //validate your input is PS1 or not(strncmp)
        int res = strncmp("PS1",input_string,3);
        //yes PS1 present
        if(res == 0){
            //check space is present or not in your input string
            if (strchr(input_string, ' ') != NULL){
                //yes space is present
                printf("PS1: command not found\n");
                continue;
            } else {
                //No space is present
                strcpy(prompt,input_string+4);
            }
        }
        else{//No its not PS1
            //get user command
            char *cmd = get_command(input_string);
            if(*cmd == '\n'){
                exit(0);
            }
            //check which command
            int res = check_command_type(cmd);
            //builtin command
            if(res == BUILTIN){
                execute_internal_commands(input_string);
            }
            else if(res == EXTERNAL){//external command
                pid1 = fork();
                // Parent process
                if (pid1 > 0){
                    waitpid(pid1 , &status, WUNTRACED);
                }
                else if (pid1 == 0){//child process
                    execute_external_commands(input_string);
                }
                else{
                    perror("Fork: Failed\n");
                    exit(1);
                }
            } 
            if (strcmp("jobs",input_string) == 0) {
                display_signals();
            }
            else if (strcmp("fg",input_string) == 0){
                fflush(stdin);
                resume_signal();
            }
            else if(strcmp("bg",input_string) == 0){
                fflush(stdin);
                resume_signal();
            }
            else if(res == UNKNOWN_CMD){
                printf("%s: No command found\n",input_string);
            }
        }
        memset(input_string,'\0',sizeof(input_string));//memset() that initializes (or clears) the entire input_string array by filling it with the null character ('\0')
    }
}

void extract_external_commands(char **external_commands){
    //open the external commands.txt file in read mode
    int fd = open("external_commands.txt",O_RDONLY);
    if(fd == -1){
    
        printf("Error : to open the file\n");
    }
    //checkng memory is allocated or not
    if(external_commands == NULL){
        printf("Error: Memory allocation failed\n");
    }
    int i = 0,j = 0,read_flag = 0;
    char array[100];
    char ch;
    //read a command from the file
    while(read(fd,&ch,1) > 0){
        if(ch != '\n'){
            array[j++] = ch;
        }
        //store each command in external_commands array in each row(use only open and read systemcalls don't use other functions)
        else if(ch == '\n'){
            array[j] = '\0';
            external_commands[i] = (char *)malloc(strlen(array)+1);
            strcpy(external_commands[i],array);
            j = 0;
            i++;
        }
    }
}

char *get_command(char *input_string){
    static char command[20];
    //check space is present or not
    if (strchr(input_string, ' ') != NULL){
        //space is present
        sscanf(input_string, "%s", command);
    }else{
        strcpy(command, input_string);
        //space is not present
    }
    return command;
}

int check_command_type(char *cmd){
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
    //compare and check your cmd is BUILTIN or NOT
    for(int i =0; builtins[i]!=NULL; i++){
        if(strcmp(cmd,builtins[i]) == 0){
            return BUILTIN;
        }
    }
    //compare and check your cmd is EXTERNAL or NOT
    for(int i =0; external_commands[i]!=NULL; i++){
        if(strcmp(cmd,external_commands[i]) == 0){
            return EXTERNAL;
        }
    }
    if(*cmd =='\0'){
        return NO_COMMAND;
    }else{
        return UNKNOWN_CMD;
    }
    
}

void execute_internal_commands(char *input_string){
    //compare and check your input_string "exit" or not
    if(strcmp("exit",input_string) == 0){
        _Exit(0);
    }
    //compare and check your input_string is a "PWD" or not
    char buf[50];
    if(strcmp("pwd",input_string) == 0){
        if (getcwd(buf, sizeof(buf)) != NULL) {
            printf("%s\n", buf);
        }else{
            perror("getcwd error");
        }
        return;
    }
    //comapre and check your input_string is "cd" or not
    if(strncmp("cd",input_string,2) == 0){
        int i,count=0;
        //removing space
        for(i = 2 ; input_string[i] == ' ' ; i++){
            count++;
        }
        //storing path name
        char *path = &input_string[2 + count];
        // Check if path is empty or not
        if (strlen(path) == 0){
            fprintf(stderr, "cd: Missing operand\n");
        }else{
            // Change directory
            if (chdir(path) == 0) {
                printf("Directory changed to: %s\n", path);
            } else {
                perror("cd error");
            }
        }
        return;
    }
    echo(input_string, status);
}

void execute_external_commands(char *input_string){
    //convert your 1D(input_string) array(string) into 2D(input) array
    char *input[20];//2D array
    int i=0,j=0;
    char arr[100];//1D array(string) for storing the first word of the command(ls -l)
    for(int k=0; input_string[k]!='\0'; k++){//input string "ls -l | wc"
        if(input_string[k]!=' '){
            arr[i++]=input_string[k];//storing the command until the space
        }
        else if(input_string[k]==' '){//if space occurs
            arr[i]='\0';//placing NULL terminator
            input[j] = (char *)malloc(strlen(arr)+1);//allocating memory
            strcpy(input[j],arr);//stroing string into 2D array
            j++;
            i=0;
        }
    }
    if(i > 0){
        arr[i] = '\0';
        input[j] = (char *)malloc(strlen(arr)+1);
        strcpy(input[j],arr);
        j++;
    }
    input[j] = NULL;
    //compare the 2D array and check pipe '|' is present or not
    int pipe_index_array[10];
    int pipe_count  = 0;
    for(int m = 0; input[m] != NULL; m++){
        if(strcmp(input[m],"|") == 0){
            pipe_index_array[pipe_count++] = m;
        }
    }
    //pipe is present 
    if(pipe_count > 0){
        //call the n_pipe function
        n_pipe(input,pipe_count,pipe_index_array);
    }
    else if(pipe_count == 0){//pipe is not present
        execvp(input[0],input+0);
    }

}

void n_pipe(char **input,int pipe_count,int pipe_index_array[]){
    //make the pipe indexs are null;
    int k = 0;// for pipe_index_array
    for(int i = 0; input[i] != NULL; i++){
        if(i == pipe_index_array[k]){
            k++;
            input[i] = NULL;
        }
    }
    //backup of stdin, stdout
    int backup1 = dup(0);//for stdin
    int backup2 = dup(1);//for stdout
    int count = 0,index = 0,s=0;
    int pid;
    for(int i = 0; i < pipe_count+1; i++){
        //create a pipe
        int fd[2];
        if(pipe(fd) == -1){
            printf("pipe creating is failure\n");
            return;
        }
        //create a process
        pid = fork(); 
        if(count == 0){
            index = 0;
        }
        else if(count > 0){
            index = pipe_index_array[s++]+1;
        }
        //child process
        if(pid == 0){
            //close the read end 
            close(fd[0]);
            //redirect the stdout into the write end of pipe(don't redirect at last)
            if(count != pipe_count){
                dup2(fd[1],1);
            }
            //execute the command
            execvp(input[index],input+index);
        }
        else if(pid > 0){//parent process
            //close the write end
            close(fd[1]);
            //redirect the stdin into read end of pipe
            dup2(fd[0],0);
        }
        count++;
    }
    //redirect stdin, stdout by using back_up.
    dup2(backup1,0);
    dup2(backup2,1);
    waitpid(pid,&status,0);
}
void echo(char *input_string, int status)
{
    //echo$?
    if(strncmp("echo $?",input_string,7) == 0)
    {
        //print the last execution status
        printf("%d\n",status);
        return;
    }
    //echo $$ --> PID of the minishell project
    if(strncmp("echo $$",input_string,7) == 0)
    {
        printf("%d\n",getpid());  
        return;  
    }
    //echo $SHELL
    if(strncmp("echo $SHELL",input_string,11) == 0){
        const char *env_var_name = "SHELL";
        // Get the value of the environment variable
        char *value = getenv(env_var_name);
        printf("%s\n",value);
        return;
    }
    //echo
    if(strncmp("echo",input_string,4) == 0){
        char str[100];
        strcpy(str,input_string+5);
        printf("%s\n",str);
        return;
    }
}

SignalNode *head = NULL;  // Head of the linked list
// Function to add the signal to the linked list
void add_signal_to_list(char *input_string,int pid) {
    SignalNode *new_node = (SignalNode*)malloc(sizeof(SignalNode));
    if (new_node == NULL) {
        perror("Failed to allocate memory for signal node");
        return;
    }
    strcpy(new_node->command,input_string);
    new_node->pid = pid;
    new_node->link = NULL;
    if(head == NULL){
        head = new_node;
        return;
    }
    SignalNode *temp = head;
    while(temp->link != NULL){
        temp = temp->link;
    }
    temp->link = new_node;
    return;
}

// Function to display the signals stored in the linked list
void display_signals() {
    SignalNode *current = head;
    while (current) {
        printf("[%d]+\tStopped\t\tsleep10\n",signal_link.pid);
        current = current->link;
    }
}

int resume_signal(){
    if (head == NULL){
        printf("bash: fg: current: no such job\n");
        return 0;
    }
    printf("%s\n", head->command);
    // Send CONT signal to continue the stopped process
    kill(head->pid, SIGCONT);
    // Wait for it to finish or be stopped again
    waitpid(head->pid, &status, WUNTRACED);
    return 1;
}

/* Signal Handler fot the input signal*/
void signal_handler(int num){
    if (num == SIGINT){// Ctrl+C
        if (pid1 > 0){
            printf("\n");
            kill(pid1, SIGINT);  // Forward SIGINT to child
            // Remove the node from the list
            SignalNode *temp = head;
            head = head->link;
            free(temp);
        }
        else{
            printf("\n");
            return;
        }
    }
    else if (num == SIGTSTP){// Ctrl+Z
        if (pid1 > 0){
            printf("\n");
            kill(pid1, SIGSTOP);
            add_signal_to_list(input_string, pid1);
            printf("[%d]+\tStopped\t\tsleep10\n",signal_link.pid);
        }else{
            printf("\n");
            fflush(stdout);
        }
    }
}

