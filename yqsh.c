#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#ifndef NULL_CHECK
#define NULL_CHECK(ptr) if (ptr == NULL) { fprintf(stderr, "OOM"); exit(1); } 
#endif

#ifndef YQSH_BUFSIZE
#define YQSH_BUFSIZE 1024
#endif

#ifndef YQSH_MAX_ARGS
#define YQSH_MAX_ARGS 256
#endif

#ifndef YQSH_ARG_LEN
#define YQSH_ARG_LEN 256
#endif

const char* VERSION = "0.0.1";

void yqsh_read_line(char* linebuf) 
{
    memset(linebuf, 0, YQSH_BUFSIZE);
    char* str = fgets(linebuf, YQSH_BUFSIZE, stdin);
    if (feof(stdin))
    {
        return;
    }
    // TODO: Make redirecting a file to stdin work
    if (str == NULL)
    {
        fprintf(stderr, "Fgets error");
        linebuf[0] = '\0';
    }
}

/*
 * line is the line recieved from the user
 * args is an allocated array of char pointers to store pointers to each argument
*/
void yqsh_parse_line(char* line, char** args)
{

    char* start = line;
    char* curr = start;
    char* next = curr;
    int curr_arg = 0;
    while (*curr != '\0' && curr != NULL)
    {
        next = curr + 1;
        if (isspace(*curr))
        {
            // replace the first whitespace character after a word with null terminator,
            // then store a pointer to the beginning of the word in args
            *curr = '\0';
            args[curr_arg] = start;
            // printf("%s\n", args[curr_arg]);
            curr_arg++;
            while(next != NULL && isspace(*next))
            {
                next++;
            }
            start = next;
        }
        curr = next;
    }
    args[curr_arg] = NULL;
}

int run_command(char** args)
{
    int pid = fork();
    if (pid != 0)
    {
        return pid;
    }
    int rc = execvp(args[0], args);
    if (rc == -1)
    {
        fprintf(stderr, "Error while execing");
        
    }
    return pid;
    
}

void yqsh_loop() 
{
    char buf[YQSH_BUFSIZE];
    char* args[YQSH_MAX_ARGS];

    while (1)
    {
        printf("\nyqsh> ");
        
        yqsh_read_line(buf);
        yqsh_parse_line(buf, args);
        run_command(args);
        int rc = 0;
        int pid = wait(&rc);
    }
}

int main(int argc, char const *argv[])
{
    // Initialization
    // TODO: read config file
    printf("yqsh version %s\n", VERSION);   
    // Main loop
    yqsh_loop();
    // Cleanup
    
    return EXIT_SUCCESS;
}
