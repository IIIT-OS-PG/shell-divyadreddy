#include "execute_command.h"
//#include "include_statements.h"

vector<pid_t> proc_exe(0);


void execute_cmd(char **vect, int j, int recfd, int& status)
{
    status = 0;
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("fork probl\n");
        status = 1;
        // exit(1);
    }
    if (pid == 0)
    {
        if(strcmp(vect[j-1], "&")==0)
        {
            int fd;
            if((fd = open("/dev/null", O_RDWR | O_APPEND | O_CREAT))<0)
            {
                status = 1;
                printf("error in opening /dev/null\n");
            }
            vect[j-1] = NULL;
            //setpgid(0, 0);
            dup2(fd, 1);
            dup2(fd, 2);
        }
        
        if (execvp(vect[0], vect) < 0)
        {
            printf("exec probl\n");
            status = 1;
            exit(1);
        }
    }
    else
    {
        if(strcmp(vect[j-1], "&")==0)
        {
            proc_exe.push_back(pid);
        }
        else
        {
            int stat;
            waitpid(pid, &stat, 0);
                // status = 1;
            // if(!WIFEXITED(stat))
            // {
            //     status = 1;
            // }
            // printf("status : %d\n", stat);
            status = stat;
        }
        printf("done execution %d\n",pid);
    }
}