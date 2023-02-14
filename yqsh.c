#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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
    char* str = fgets(linebuf, YQSH_BUFSIZE, stdin);
    if (str == NULL)
    {
        fprintf(stderr, "Fgets error");
        linebuf[0] = '\0';
    }
}

void yqsh_parse_line(char* line, char** args)
{
    char* start = line;
    char* curr = start;
    int curr_arg = 0;
    while (*curr != '\0' && curr != NULL)
    {    
        if (isspace(*curr))
        {
            // clear the contents of args[curr_arg], then copy the word
            memset(args[curr_arg], 0, YQSH_ARG_LEN);
            memcpy(args[curr_arg], start, curr - start);
            printf("%s\n", args[curr_arg]);
            curr_arg++;
            while(curr != NULL && isspace(*curr))
            {
                curr++;
            }
            start = curr;
        }
        curr++;
    }

}

void yqsh_loop() 
{
    char buf[YQSH_BUFSIZE];
    char* args[YQSH_MAX_ARGS];
    for (int i = 0; i < YQSH_MAX_ARGS; i++)
    {
        args[i] = malloc(sizeof(char) * YQSH_ARG_LEN);
        NULL_CHECK(args[i]);
    }

    while (1)
    {
        printf("yqsh> ");
        
        yqsh_read_line(buf);
        yqsh_parse_line(buf, args);
    }
    
    // TODO: intercept SIGTERM to cleanup
    for (int i = 0; i < YQSH_MAX_ARGS; i++)
    {
        free(args[i]);
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
