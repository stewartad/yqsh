#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "yqsh.h"

yqsh_job* first_job = NULL;
yqsh_job* last_job = NULL;

/* Returns true if given job is completed */
int job_is_completed(yqsh_job* job)
{
    for (yqsh_process* p = job->first_process; p; p = p->next)
    {
        if (!p->completed)
        {
            return 0;
        }
    }
    return 1;
}

/* Returns true if given job is stopped */
int job_is_stopped(yqsh_job *job)
{
    for (yqsh_process* p = job->first_process; p; p = p->next) 
    {
        if (!p->completed && !p->stopped)
        {
            return 0;
        }
    }
    return 1;
}

/* Searches all jobs and returns one with pgid, or NULL if no job with that pgid exists */
yqsh_job* find_job(pid_t pgid)
{
    yqsh_job* job = first_job;
    for (job = first_job; job; job = job->next)
    {
        if (job->pgid == pgid)
        {
            return job;
        }
    }
    return NULL;
}

void yqsh_read_line(char* linebuf) 
{
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
 * Separates arguments in line by whitespace.
 * If line is empty or contains only whitespace, returns NULL, 
 * otherwise allocates and returns an array of strings.
 * Stores the number of arguments parsed in nargs.
 */
char** yqsh_separate_args(char *line, size_t* nargs)
{
    char* delim = " \r\n\t";
    char* arg = strtok(line, delim);
    if (arg == NULL)
    {
        return NULL;
    }
    size_t cur_size = 512;
    char** args = malloc(cur_size * sizeof(char*));
    args[0] = arg;
    size_t n = 1;
    while (arg = strtok(NULL, delim), arg != NULL)
    {
        if (n >= cur_size)
        {
            args = realloc(args, cur_size * 2 * sizeof(char*));
            cur_size *= 2;
        }
        args[n++] = arg;
    }
    *nargs = n;
    return args;
}

void yqsh_parse_args(char *line, char** args, size_t* nargs)
{
    char *p = line;
    int argc = 0;
    
    // ignore leading whitespace
    while (*p != 0 && isspace(*p))
    {
        p++;
    }

    char *start = p;
    while (*p != 0)
    {
        if (isspace(*p))
        {
            *p = '\0';
            if (p != line && *(p - 1) != '\0')
            {
                // add as an arg only if prev character is not null
                // i.e. this string is not empty
                args[argc++] = start;
            }
            p = p + 1;
            start = p;
        }
        else 
        {
            p = p + 1;
        }
    }
    *nargs = argc;
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
        fprintf(stderr, "Error while execing: %d", errno);
        
    }
    return pid;
    
}

void yqsh_loop(int argc, const char** argv) 
{
    char buf[YQSH_BUFSIZE];
    char* args[YQSH_MAX_ARGS];

    size_t bufsize = YQSH_BUFSIZE;
    char* linebuf = malloc(bufsize);
    size_t nargs;
    NULL_CHECK(linebuf);
    int running = 1;
    while (running)
    {
        memset(linebuf, 0, YQSH_BUFSIZE);
        memset(args, 0, YQSH_BUFSIZE);

        printf("yqsh> ");
        if (argc > 1)
        {
            //char** args2 = yqsh_separate_args(argv, &nargs);
            //args2[nargs] = 0;
            //run_command(argv);
            //int rc = 0;
            //running = 0;
            //int pid = wait(&rc);
        } else 
        {
            //__ssize_t n = getline(&linebuf, &bufsize, stdin);
            yqsh_read_line(buf);
            yqsh_parse_args(buf, args, &nargs);
            if (nargs > 0)
            {
                run_command(args);
            }
        }
        int rc = 0;
        int pid = wait(&rc);
   }
}

