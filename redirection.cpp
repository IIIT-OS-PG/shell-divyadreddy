#include "redirection.h"
//#include "include_statements.h"

void redirect(char **vect, int opt, int len, int inp, int out, int recfd, int &status)
{
    int fd;
    status = 0;
    if (opt == 1)
    {
        printf("file: %s \n", vect[len - 1]);
        fd = open(vect[len - 1], O_APPEND | O_CREAT | O_WRONLY,  0777);
        if (fd < 0)
        {
            printf("Error in opening the file");
            status = 1;
            return;
        }
        int stdout1 = dup(1);
        if (stdout1 < 0)
        {
            printf("Error in duplicating standard output\n");
            status = 1;
            return;
        }
        if (dup2(fd, 1) < 0)
        {
            printf("Error in redirecting output\n");
            status = 1;
            return;
        }
        close(fd);
        vect[len - 2] = NULL;
        pid_t pid = fork();
        if (pid < 0)
        {
            status = 1;
            printf("fork probl\n");
            return;
        }
        if (pid == 0)
        {
            if (execvp(vect[0], vect) < 0)
            {
                printf("exec probl\n");
                status = 1;
                return;
            }
        }
        else
        {
            int stat;
            // if(pid != waitpid(pid, &stat, 0))
            //     status = 1;
            // printf("done %d\n",pid);
            waitpid(pid, &stat, 0);
            dup2(stdout1, 1);
            close(stdout1);
            status = stat;
        }
    }
    else if (opt == 2)
    {
        printf("file: %s \n", vect[len - 1]);
        fd = open(vect[len - 1], O_APPEND | O_CREAT | O_TRUNC | O_WRONLY,  0777);
        if (fd < 0)
        {  
            status = 1;
            printf("Error in opening the file");
        }
        int stdout1 = dup(1);
        if (stdout1 < 0)
        {
            status = 1;
            printf("Error in duplicating standard output\n");
        }
        if (dup2(fd, 1) < 0)
        {
            status = 1;
            printf("Error in redirecting output\n");
        }
        close(fd);
        vect[len - 2] = NULL;
        pid_t pid = fork();
        if (pid < 0)
        {
            status = 1;
            printf("fork probl\n");
            exit(1);
        }
        if (pid == 0)
        {
            if (execvp(vect[0], vect) < 0)
            {
                status = 1;
                printf("exec probl\n");
                exit(1);
            }
        }
        else
        {
            int stat;
            // if(pid != )
            //     status = 1;
            // printf("done %d\n",pid);
            waitpid(pid, &stat, 0);
            dup2(stdout1, 1);
            close(stdout1);
            status = stat;
        }
    }
    
}

