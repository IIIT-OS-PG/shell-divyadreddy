#include "pipe_command.h"
#include "redirection.h"
#include "include_statements.h"

void pipe_cmd(char **vect, vector<int> ind, int len, int recfd, int &status)
{
    int i;
    int lind = ind.size();
    pid_t pid;
    int in, fd[2];
    in = 0;
    int stat;
    int start;
    lind--;
    int k = 0;
    int out;
    char** vect1;
    status = 0;
    for (i = 0; i <= lind; ++i)
    {
        pipe(fd);
        // out = fd[1];
        vect1 = (char**) malloc (10* sizeof(char*));
        printf("in pipe : %d %d\n", lind, ind[i]);
        start = k;
        for (; k < ind[i]; k++)
        {
            vect1[k - start] = vect[k];
            printf("%s\n", vect1[k - start]);
        }
        int last = k - start;
        vect1[last] = NULL;
        k++; // to skip | char
        // printf("last : %d\n", last);
        // if (last > 2 && (strcmp(vect1[last-2], ">>") == 0))
        // {
        //     printf(">> is there : pipe\n");
        //     redirect(vect, 1, last, 0, 1);
        // }
        // else if (last > 2 && (strcmp(vect1[last - 2], ">") == 0))
        // {
        //     printf("> is there : pipe\n");
        //     redirect(vect, 2, last, 0 , 1);
        // }
        if (i < lind)
        {
            if ((pid = fork()) == 0)
            {
                if (in != 0)
                {
                    dup2(in, 0);
                    close(in);
                }
                if (out != 1)
                {
                    dup2(fd[1], 1);
                    close(fd[1]);
                }
                if (execvp(vect1[0], vect1) < 0)
                {
                    printf("error in exec of pipe : for loop\n");
                    status = 1;
                    return;
                }
            }
            else if (pid < 0)
            {
                printf("error in fork pipe\n");
                status = 1;
                return;
            }
            else
            {
                // status = 1;
                waitpid(pid, &stat, 0);
                status = stat;
                // {
                //     status = 1;
                // }
            }
            close(fd[1]);
            in = fd[0];
        }
        else
        {
            if ((pid = fork()) == 0)
            {
                if (in != 0)
                {
                    dup2(in, 0);
                    close(in);
                }
                if (execvp(vect1[0], vect1) < 0)
                {
                    printf("error in exec of pipe : last one\n");
                    status = 1;
                    exit(1);
                }
            }
            else if (pid < 0)
            {
                printf("error in fork pipe\n");
                status = 1;
                exit(1);
            }
            else
            {
                // status = 1;
                waitpid(pid, &stat, 0);
                // {
                //     status = 1;
                // }
                status = stat;
            }
        }
        free(vect1);
    }
}
    // char *vect2[10];
    // start = k;
    // for (; k < ind[i]; k++)
    // {
    //     vect2[k - start] = vect[k];
    //     printf("%s\n", vect2[k - start]);
    // }
    // vect2[k - start] = NULL;
    // if ((pid = fork()) == 0)
    // {
    //     if (in != 0)
    //     {
    //         dup2(in, 0);
    //         close(in);
    //     }
    //     if (execvp(vect2[0], vect2) < 0)
    //     {
    //         printf("error in exec of pipe\n");
    //         return;
    //     }
    // }
    // else if (pid < 0)
    // {
    //     printf("error in fork pipe\n");
    // }
    // else
    // {
    //     waitpid(pid, &stat, 0);
    // }
    // close(fd[0]);


// void pipe_cmd(char** vect, vector<int> ind, int len)
// {
//     int lind = ind.size();
//     pid_t pid1, pid2;
//     int pipefd[2];
//     pipe(pipefd);
//     char* vect1[10];
//     int i, j = 0;
//     int stat;
//     int start = 0;
//     printf("in pipe : %d %d\n", lind, ind[j]);

//     for (i = 0; i < ind[j]; i++)
//     {
//         vect1[i] = vect[i];
//         printf("%s\n", vect1[i]);
//     }
//     vect1[i] = NULL;
//     j++;
//     pid1 = fork();
//     int stdoutput = dup(1);
//     int stdinput = dup(0);
//     if (pid1 < 0)
//     {
//         printf("error in forking\n");
//         return;
//     }
//     if (pid1 == 0)
//     {
//         if(dup2(pipefd[1], STDOUT_FILENO)<0)
//         {
//             printf("error in dup2 of pipe\n");
//             return;
//         }
//         close(pipefd[0]);
//         if (execvp(vect1[0], vect1) < 0)
//         {
//             printf("error in exec of pipe\n");
//             return;
//         }
//     }
//     else
//     {
//         waitpid(pid1, &stat, 0);
//         printf("done 1st pipe\n");
//     }
//     lind--;
//     i++;
//     while (lind > 0)
//     {
//         printf("pipe : in while : %d\n",lind);
//         char* vect2[10];
//         lind--;
//         start = i;
//         for (; i < ind[j]; i++)
//         {
//             vect2[i - start] = vect[i];
//             printf("%s\n", vect2[i-start]);
//         }
//         // int curr = i;
//         vect2[i-start] = NULL;
//         j++;
//         pid2 = fork();
//         if (pid2 < 0)
//         {
//             printf("error in forking\n");
//             return;
//         }
//         if (pid2 == 0)
//         {
//             if(dup2(pipefd[0], STDIN_FILENO)<0)
//             {
//             printf("error in dup2 of pipe\n");
//             return;
//             }
//             close(pipefd[1]);
//             printf("fork pipe :  %d\n", lind);
//             if(execvp(vect2[0], vect2)<0)
//             {
//                 perror("exec");
//                 return;
//             }
//         }
//         else
//         {
//             waitpid(pid2, &stat, 0);
//             printf("done 2nd pipe\n");
//         }
//     }
//     close(pipefd[0]);
//     close(pipefd[1]);
//     dup2(stdoutput, 1);
//     dup2(stdinput, 0);
// }