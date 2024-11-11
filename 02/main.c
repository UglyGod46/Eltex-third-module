#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

int main() 
{
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARG_SIZE];
    pid_t pid;

    while (1)
    {
        printf("shell> ");
        fgets(input, MAX_INPUT_SIZE, stdin);
        
        input[strcspn(input, "\n")] = 0;

        char *token = strtok(input, " ");
        int i = 0;

        while (token != NULL) 
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; 

        if (strcmp(args[0], "exit") == 0) 
        {
            break;
        }

        pid = fork();

        if (pid == 0) 
        {
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        }

        else if (pid < 0) 
        {
            perror("fork failed");
        }

        else 
        {
            wait(NULL);
        }
    }
    return 0;
}
