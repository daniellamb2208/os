#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <malloc.h>
#include <signal.h>

#define true 1
#define BUFF_SIZE 1 << 8
#define STD_INPUT 0
#define STD_OUTPUT 1
#define MAX_ARGVS 10

typedef struct Command
{
    char *name;
    char **argv;
    int input_file_descriptor;
    int output_file_descriptor;
    int background;
    struct Command *next;
} command;

typedef struct Raw_command
{
    char *token;
    struct Raw_command *next;
} raw;

void push(raw *, char *);
raw *seperate(char *);

void print_prompt();
command *parse_command(raw *);
void exe(command *);
void pipeline(command *);

int main()
{
    while (true)
    {
        print_prompt(); //init the shell

        char *input_buffer = (char *)malloc(sizeof(char) * BUFF_SIZE);
        memset(input_buffer, 0, BUFF_SIZE); //prepare a buffer to recieve command
        fgets(input_buffer, BUFF_SIZE, stdin);

        if (strcmp(input_buffer, "\n") == 0) //input is end of line
            continue;

        input_buffer = strtok(input_buffer, "\n"); //remove the input \n

        if (strcmp(input_buffer, " ") == 0) //a space
            continue;

        raw *root = seperate(input_buffer); //restore the command
        command *cmd = parse_command(root); //parse it

        if (strcmp(cmd->name, "exit") == 0)
            exit(EXIT_SUCCESS);

        if (cmd->next != NULL) //only finish implement of "two"-process pipeline
            pipeline(cmd);
        else //normal exec the command
            exe(cmd);
    }

    return 0;
}

void pipeline(command *cmd) //exit immediately ?
{
    int fd[2];
    pipe(&fd[0]);

    int pid = fork();
    if (pid == 0)
    {
        close(fd[1]);
        close(STD_INPUT);
        dup(fd[0]);
        close(fd[0]);
        execvp(cmd->next->name, cmd->next->argv);
    }
    else
    {
        close(fd[0]);
        close(STD_OUTPUT);
        dup(fd[1]);
        close(fd[1]);
        execvp(cmd->name, cmd->argv);
    }
}

command *parse_command(raw *root)
{
    raw *current = root;
    command *cmd = (command *)malloc(sizeof(command));
    cmd->name = (char *)malloc(sizeof(char) * strlen(current->token));
    strcpy(cmd->name, current->token);
    cmd->input_file_descriptor = 0;
    cmd->output_file_descriptor = 1;
    cmd->background = 0;
    cmd->next = NULL;
    cmd->argv = (char **)malloc(sizeof(char *) * (MAX_ARGVS));

    int index = 0; //argv index
    while (current->next != NULL)
    {
        if (strcmp(current->token, "<") == 0)
        {
            current = current->next;
            cmd->input_file_descriptor = open(current->token, O_RDONLY);
            current = current->next;
            continue;
        }
        else if (strcmp(current->token, ">") == 0)
        {
            current = current->next;

            if (current->token == NULL)
            {
                perror("Error: Redirection file name is necessary.\n");
                strcpy(cmd->name, "exit");
            }
            else
                cmd->output_file_descriptor = open(current->token, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            current = current->next;
            continue;
        }
        else if (strcmp(current->token, "&") == 0)
        { //not yet finished
            cmd->background = 1;
            current = current->next;
            continue;
        }
        else if (strcmp(current->token, "|") == 0)
        {
            cmd->argv[index] = NULL;
            cmd->next = parse_command(current->next);
            break;
        }
        cmd->argv[index] = (char *)malloc(sizeof(char) * strlen(current->token));
        strcpy(cmd->argv[index], current->token);
        index++;

        current = current->next;
    }
    return cmd;
}

raw *seperate(char *input)
{
    char *token = strtok(input, " ");
    raw *root = (raw *)malloc(sizeof(raw));
    root->token = token;
    root->next = NULL;

    while (token != NULL)
    {
        token = strtok(NULL, " ");
        push(root, token);
    }
    return root;
}

void push(raw *first, char *t)
{
    raw *newone = (raw *)malloc(sizeof(raw));
    newone->token = t;
    newone->next = NULL;

    raw *current = first;
    while (current->next != NULL)
        current = current->next;
    current->next = newone;
}

void exe(command *cmd)
{
    if (strcmp(cmd->name, "cd") == 0)
    {
        if (cmd->argv[1] == NULL)
            return;
        if (strcmp(cmd->argv[1], "~") == 0)
            strcpy(cmd->argv[1], "/home");
        if (chdir(cmd->argv[1]) == -1)
            perror("ERROR");
        return;
    }

    int pid = fork();
    if (pid < 0)
        perror("Fork error\n");
    else if (pid == 0) //child
    {
        if (cmd->input_file_descriptor != STDIN_FILENO)
        {
            close(STDIN_FILENO);
            dup2(cmd->input_file_descriptor, STDIN_FILENO);
        }
        if (cmd->output_file_descriptor != STDOUT_FILENO)
        {
            close(STDOUT_FILENO);
            dup2(cmd->output_file_descriptor, STDOUT_FILENO);
        }

        if (execvp(cmd->name, cmd->argv) == -1)
        {
            printf("ERROR : command not found\n");
            return ;
        }
        fflush(stdout);
    }
    else
        wait(NULL); //waitpid(-1, &status, WNOHANG);
}

void print_prompt()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char *username = getenv("USER");

    printf("%s: ", username);
    printf("~%s", cwd);
    printf("$ ");
}
