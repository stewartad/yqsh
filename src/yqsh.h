#include <ctype.h>
#include <termio.h>
#include <stdlib.h>

#ifndef YQSH_H
#define YQSH_H

#define NULL_CHECK(ptr) if (ptr == NULL) { fprintf(stderr, "OOM"); exit(1); } 

#define YQSH_BUFSIZE 1024
#define YQSH_MAX_ARGS 256
#define YQSH_ARG_LEN 256
#define YQSH_VERSION "0.0.1"

typedef struct yqsh_process {
    struct yqsh_process* next;    /* next process in pipeline */
    char** argv;             /* to pass to exec */
    pid_t pid;               /* process ID */
    char completed;          /* true if process has completed */
    char stopped;            /* true if process has stopped */
    int status;              /* reported status value */
} yqsh_process;

typedef struct yqsh_job {
    struct yqsh_job* next;          /* next active job */
    char* command;             /* command line, used for messages */
    yqsh_process* first_process;    /* list of processes in this job */
    pid_t pgid;                /* process group ID */
    char notified;             /* true if user told about stopped job */
    struct termios tmodes;     /* saved terminal modes */
    int stdin, stdout, stderr; /* standard i/o channels */
} yqsh_job;

int job_is_completed(yqsh_job* job);
int job_is_stopped(yqsh_job *job);
yqsh_job* find_job(pid_t pgid);
char** yqsh_separate_args(char *line, size_t* nargs);
void yqsh_loop(int, const char**);

#endif
