#include "include_statements.h"
#include "input_command.h"
#include "execute_command.h"
#include "redirection.h"
#include "pipe_command.h"
#include "initialize_shell.h"
#include <stdlib.h>

// #include "share_var.h"
void initialise();
// int * histsize, map<char*, char* > &env)

char **input_cmd(); //queue<char*> &history);
char *open_app(char *command);
char *matchExt(char *ext);
void alarm();
void prompt();

int histsize;

map<char *, char *> env;
map<char *, char *> alias;
int histfd;
int root = 0;
FILE *hifd;
queue<char *> history;

int main()
{
  int rec_started = 0;
  int recfd = -1;
  // struct termios mode;
  // histfd = open("history.txt", O_APPEND| O_RDWR, 0777);
  // tcgetattr(0, &mode);
  // mode.c_lflag &= ~(ECHO | ICANON);
  // tcsetattr(0, TCSANOW, &mode);

  int status = 0;
  if ((hifd = fopen("history.txt", "w+")) == NULL)
  {
    printf("Error! opening history file");
    // Program exits if file pointer returns NULL.
    exit(1);
  }
  else
  {
    fseek(hifd, SEEK_END , 0);
    printf("opened file\n");
    // fputs("HISTORY : \n", hifd);
  }
  
  // reads text until newline

  alarm();
  initialise();
  while (true)
  {

    char **vect = input_cmd(); //history);
    map<char *, char *>::iterator itr;
    int j = 0;
    int flag_pipe = 0;
    vector<int> pipe_ind(0);
    int flag_echo = 0;
    // char* home = getenv("$HOME");
    if (vect == NULL)
    {
      printf("empty: main\n");
      continue;
    }
    if (strcmp(vect[0], "echo") == 0)
    {
      flag_echo = 1;
    }
    while (vect[j] != NULL)
    {
      if (strcmp(vect[j], "|") == 0)
      {
        pipe_ind.push_back(j);
      }
      if (flag_echo && vect[j][0] == '$')
      {
        if (strcmp(vect[j], "$$") == 0)
        {
          // vect[j] =
          // itoa(getpid(),vect[j],10);
          sprintf(vect[j], "%d", getpid());
          printf("updated : %s\n", vect[j]);
        }
        else if (strcmp(vect[j], "$?") == 0)
        {
          sprintf(vect[j], "%d", status);
          printf("updated : %s\n", vect[j]);
        }
        else
        {
          for (itr = env.begin(); itr != env.end(); ++itr)
          {
            if (strcmp(itr->first, vect[j]) == 0)
              vect[j] = itr->second;
          }
        }
        // if(strcmp(vect[j],"$HOME")==0)
        // {
        //     vect[j] = env["HOME"];
        //     printf("updated : %s\n", env["HOME"]);
        // }
      }
      // printf("%s\n", vect[j]);
      j++;
    }

    if (j == 1 && strcmp("$$", vect[0]) == 0)
    {
      printf("%d\n", getpid());
      status = 0;
    }
    else if (j == 1 && strcmp("$?", vect[0]) == 0)
    {
      printf("%d\n", status);
      status = 0;
    }
    else if (j == 1 && strcmp("history", vect[0]) == 0)
    {
      // fclose(hifd);
      // if ((hifd = fopen("history.txt", "rw")) == NULL)
      // {
      //   printf("Error! opening history file");
      //   // Program exits if file pointer returns NULL.
      //   exit(1);
      // }
      fseek(hifd, 0, SEEK_SET);
      int newlines = 0;
      char c[100];
      while (fgets(c, 1000, hifd) != NULL)
      {
        newlines++;
      }
      fseek(hifd, 0, SEEK_SET);
      if(newlines>histsize)
      {
      newlines -= histsize;
      while (newlines>0 && (fgets(c, 1000, hifd) != NULL))
      {
        newlines--;
      }
      }
      printf("History: \n");
      while (fgets(c, 1000, hifd) != NULL)
      {
          printf("%s",c);
      }
      fseek(hifd, 0, SEEK_END);
      status = 0;
    }
    else if (j == 2 && (strcmp(vect[0], "record") == 0 && strcmp(vect[1], "start") == 0))
    {
      if (rec_started == 1)
      {
        printf("It is already recording\n");
      }
      else
      {
        recfd = open("record", O_APPEND | O_CREAT | O_WRONLY, 0777);
        rec_started = 1;
        if (recfd < 0)
        {
          printf("Error : Not able to record\n");
          rec_started = 0;
          status = 1;
          continue;
        }
      }
      status = 0;
    }
    else if (j == 2 && (strcmp(vect[0], "record") == 0 && strcmp(vect[1], "stop") == 0))
    {
      if (rec_started == 0)
      {
        printf("Error : It is not recording\n");
        status = 1;
      }
      else
      {
        close(recfd);
        rec_started = 0;
        status = 0;
      }
    }
    else if (j == 2 && (strcmp(vect[0], "sudo") == 0 && strcmp(vect[1], "su") == 0))
    {
      root = 1;
      status = 0;
    }
    // else
    else if (j == 2 && strcmp("cd", vect[0]) == 0)
    {
      int lpath = strlen(vect[1]);
      int tilde = -1;
      int i;
      for (i = 0; i < lpath; i++)
      {
        if (vect[1][i] == '~')
        {
          tilde = i;
          break;
        }
      }
      if (tilde > -1)
      {

        // for (itr = env.begin(); itr != env.end(); ++itr) {
        //         if(strcmp(itr->first, "$HOME")==0)
        //            home = itr->second;
        //     }
        //     printf("home is : %s\n", home);
        //
        printf("not allocated mem : home \n");
        // char* home = (char*) malloc(100*sizeof(char));
        char *home = getenv("HOME");
        printf("home %s\n", home);
        int lhome = strlen(home);
        // lhome--;
        char *pathwithhome = (char *)malloc((lpath + lhome - 1) * sizeof(char));
        printf("allocated mem : home %d %s\n", lhome, lpath);
        for (i = 0; i < tilde; i++)
        {
          pathwithhome[i] = vect[1][i];
        }
        int hi = i + 1;
        for (; i < lhome; i++)
        {
          pathwithhome[i] = home[i - tilde];
        }
        printf("%s\n", pathwithhome);
        for (; i < (lpath + lhome) && hi < lpath; i++, hi++)
        {
          pathwithhome[i] = vect[1][hi];
        }
        pathwithhome[i] = '\0';
        printf("total path : %s\n", pathwithhome);
        vect[1] = pathwithhome;
        // if(chdir(vect[1])<0)
        // {
        //     printf("Error : not able to change directory\n");
        // }
      }
      status = 0;
      if (chdir(vect[1]) < 0)
      {
        printf("Error : not able to change directory\n");
        status = 1;
      }
    }
    else if (!pipe_ind.empty())
    {
      pipe_ind.push_back(j);
      pipe_cmd(vect, pipe_ind, j, recfd, status);
    }
    else if (j > 2 && (strcmp(vect[j - 2], ">>") == 0))
    {
      // printf(">> is there");
      redirect(vect, 1, j, 0, 1, recfd, status);
    }
    else if (j > 2 && (strcmp(vect[j - 2], ">") == 0))
    {
      // printf("> is there");
      redirect(vect, 2, j, 0, 1, recfd, status);
    }
    else if (strcmp(vect[0], "") == 0)
    {
      printf("here\n");
      continue;
    }
    else
    {
      execute_cmd(vect, j, recfd, status);
      printf("status %d\n", status);
    }
  }
  return 0;
}

//clear();
// int f;
// if((f = open("abc.txt", O_APPEND | O_CREAT | O_WRONLY | S_IRWXU | S_IRWXG | S_IRWXO))>0)
// {
//     perror("error in opening file");
//     exit(1);
// }
// close(f);

// printf("started\n");
// char* cmd;
// char* x = NULL;
// printf("%s", x);

// char number[100];
// FILE *f = popen("ls", "r");
// while (fgets(number, 100, f) != NULL)
// {
//     printf( "%s\n", number );
// }
// pclose(f);

void initialise()
{
  int size;

  char *p[5] = {"PATH ", "\nHOME ", "\nUSER ", "\nHOSTNAME ", "\nPS1 "};
  char *values[5] = {getenv("PATH"), getenv("HOME"), getenv("USER"), "HOSTNAME", "root # user $"};

  for (int i = 0; i < 5; i++)
  {
    printf("%s\n", values[i]);
  }
  //   env["PATH"] = getenv("PATH");
  // printf("%s\n", getenv("HOSTNAME"));
  //   printf("hgd\n");
  //   int fd = open("dishrc.txt", O_APPEND | O_RDWR, 0777);
  //   if (fd < 0)
  //   {
  //     printf("Error : Cannot open .dishrc file");
  //   }
  int i;
  int offset = 0;
  // lseek(fd, SEEK_SET , 0);
  // for(i = 0; i < 5; i++)
  // {
  // size = strlen(p[i]);
  // printf("size : %d\n", size);
  // if(write(fd, p[i], size) < 0){
  //     printf("Error : in writing to .dishrc\n");
  // }
  // size = strlen(values[i]);
  // printf("size : %d\n", size);
  // if(write(fd, values[i], size) < 0){
  //     printf("Error : in writing to .dishrc\n");
  // }
  // }
  char c[1000];
  FILE *fptr;
  if ((fptr = fopen("dishrc.txt", "r")) == NULL)
  {
    printf("Error! opening file");
    // Program exits if file pointer returns NULL.
    exit(1);
  }
  // reads text until newline

  while (fgets(c, 1000, fptr) != NULL)
  {
    printf("%s", c);
    if (strncmp(c, "alias", 5) == 0)
    {
      printf("inside alias : %s\n", c);
      char *key = (char *)malloc(200 * sizeof(char));
      char *val = (char *)malloc(200 * sizeof(char));
      int length = strlen(c);
      int k = 6, l = 0;
      for (; c[k] != '=' && k < length; k++, l++)
      {
        key[l] = c[k];
      }
      key[l] = '\0';
      k++;
      l = 0;
      for (; k < length && c[k] != '\n'; k++, l++)
      {
        val[l] = c[k];
      }
      val[l] = '\0';
      alias[key] = val;
      printf("ini : %s %s\n", key, alias[key]);
    }
    else if (strncmp(c, "HISTSIZE", 8) == 0)
    {
      char *key1 = (char *)malloc(20 * sizeof(char));
      int length = strlen(c);
      int k = 9, l = 0;
      for (; (c[k] != ' ' && c[k] != '\n') && k < length; k++, l++)
      {
        key1[l] = c[k];
        // printf("%c",c[l]);
      }
      key1[l] = '\0';
      l--;
      // printf("key : %s %d\n", key1, strlen(key1));
      int mul = 1;
      int num = 0;
      for (; l > -1; l--)
      {
        num = num + (key1[l] - '0') * mul;
        mul = mul * 10;
      }
      histsize = num;
      // printf("ini : %s %d\n", key1, histsize);
    }
    else
    {
      printf("inside env var : %s\n", c);
      char *key = (char *)malloc(200 * sizeof(char));
      char *val = (char *)malloc(200 * sizeof(char));
      int length = strlen(c);
      int k = 0, l = 1;
      key[0] = '$';
      for (; c[k] != ' ' && k < length; k++, l++)
      {
        key[l] = c[k];
      }
      key[l] = '\0';
      k++;
      l = 0;
      for (; k < length && c[k] != '\n'; k++, l++)
      {
        val[l] = c[k];
      }
      val[l] = '\0';
      env[key] = val;
      //   printf("ini : %s %s\n", key, env[key]);//, strcmp(p1, key),env[p1], p1, env["PATH"]);
    }
  }
  // free(key);
  // free(val);
  fclose(fptr);
  //
}

char **input_cmd()
{
  char cmd[1000];
  printf("histsize : %d\n", histsize);
  printf("alias size : %ld\n", alias.size());

  map<char *, char *>::iterator itr;
  for (itr = env.begin(); itr != env.end(); ++itr)
  {
    cout << '\t' << itr->first
         << '\t' << itr->second << '\n';
  }
  if (histsize == history.size())
  {
    history.pop();
  }

  // printf("divya$ ");
  prompt();
  fgets(cmd, 1000, stdin);
  if( fputs(cmd, hifd) < 0)
  {
    printf("%s", cmd);
    printf("NOT writing history\n");
  }
  // scanf("%[^\n]s",cmd);
  //string scmd = cmd;
  int cmd_len = strlen(cmd);
  cmd_len--; // for fgets
  if (cmd_len > 5 && strncmp(cmd, "open", 4) == 0)
  {
    cmd_len++;
    printf("before open app  : %s \n", cmd);
    char *cmd2;
    int p = 0;
    cmd2 = open_app(cmd);
    int cmd2_len = strlen(cmd2);
    for (p = 0; p < cmd2_len; p++)
    {
      cmd[p] = cmd2[p];
    }
    cmd[p] = '\0';
    printf("%s %s\n", cmd, cmd2);
    // return NULL;
  }

  int flag = 0, space_count = 1;
  int start = 0;
  printf("%s %d\n", cmd, cmd_len);
  // cmd[cmd_len] = '\0';

  // if(strcmp("history",cmd)!=0)
  // {
  char histcmd[cmd_len];
  int h;
  for (h = 0; h < cmd_len && cmd[h] != '\n'; h++)
  {
    histcmd[h] = cmd[h];
  }
  histcmd[h] = '\0';
  printf("histcmd : %s\n", histcmd);
  history.push(histcmd);
  printf("pushed : %s %s\n", history.back(), history.front());
  printf("pushed in func : %s %s\n", history.back(), history.front());
  // }
  // for (int h = 0; h < cmd_len; h++)
  // {
  //     cout << cmd[h] << endl;
  // }
  //
  printf("Command length : %d\n", cmd_len);
  while (cmd[cmd_len - 1] == ' ') // to remove ending spaces
  {
    cmd_len--;
  }
  while (cmd[start] == ' ') // to remove starting spaces
  {
    start++;
  }
  int i;
  // printf("removed spaces\n");
  for (i = start; i < cmd_len; i++)
  {
    if (cmd[i] == ' ')
    {
      if (flag == 0)
      {
        flag = 1;
        space_count++;
      }
    }
    else
    {
      flag = 0;
    }
  }
  // printf("counted spaces\n");
  // char vect[space_count + 1][100]; // giving seg fault for char*, so assuming max token size is 100
  char **vect = (char **)malloc((space_count + 1) * sizeof(char *));
  int j = 0, k = 0;
  flag = 0;
  int last_ind = -1;

  vect[j] = (char *)malloc(100 * sizeof(char));
  for (i = start; i < cmd_len; i++)
  {
    if (cmd[i] == ' ')
    {
      // if(last_ind>-1)
      // {
      //     last_ind = -1;
      //     vect[last_ind][k] = '\0';
      // }
      if (flag == 0)
      {
        vect[j][k] = '\0';
        j++;
        vect[j] = (char *)malloc(100 * sizeof(char));
        flag = 1;
        k = 0;
      }
    }
    else
    {
      vect[j][k++] = cmd[i];
      last_ind = j;
      flag = 0;
    }
  }
  vect[j][k] = '\0';
  printf("copied to vect : %d\n", j + 1);
  vect[++j] = NULL;

  //   char* temp = (char *)malloc(100 * sizeof(char));

  //   for (i = start; i < cmd_len; i++)
  //   {
  //     if (cmd[i] == ' ')
  //     {
  //       // if(last_ind>-1)
  //       // {
  //       //     last_ind = -1;
  //       //     vect[last_ind][k] = '\0';
  //       // }
  //       printf("in here\n");
  //       if (flag == 0)
  //       {
  //         temp[k] = '\0';
  //         printf("alias : %s\n", alias["k"]);
  //         if (alias[temp])
  //         {
  //           char *token = strtok(alias[temp], " ");

  //           // Keep printing tokens while one of the
  //           // delimiters present in str[].
  //           while (token != NULL)
  //           {
  //             printf("alias copying : %s\n", token);
  //             vect[j] = token;
  //             j++;
  //             vect[j] = (char *)malloc(100 * sizeof(char));
  //             token = strtok(NULL, " ");
  //           }
  //         }
  //         else
  //         {
  //           printf("copying : %s\n", temp);
  //           vect[j] = temp;
  //             j++;
  //         vect[j] = (char *)malloc(100 * sizeof(char));
  //         }
  //         flag = 1;
  //         k = 0;
  //       }
  //     }
  //     else
  //     {
  //       temp[k++] = cmd[i];
  //       flag = 0;
  //     }
  //   }
  //   if (flag == 0)
  //       {
  //         temp[k] = '\0';
  //         if (alias[temp])
  //         {
  //           char *token = strtok(alias[temp], " ");

  //           // Keep printing tokens while one of the
  //           // delimiters present in str[].
  //           while (token != NULL)
  //           {
  //             printf("alias copying : %s\n", token);
  //             vect[j] = token;
  //             j++;
  //             // vect[j] = (char *)malloc(100 * sizeof(char));
  //             token = strtok(NULL, " ");
  //           }
  //           j--;
  //         }
  //         else
  //         {
  //           printf("copying : %s\n", temp);
  //           vect[j] = temp;
  //         }
  //       }
  //   // vect[j][k] = '\0';
  for (i = 0; i < j; i++)
  {
    printf("%s\n", vect[i]);
  }
  return vect;
}

#include <stdio.h>
#include <string.h>

char *matchExt(char *ext)
{
  FILE *config = fopen("openvid.txt", "rw");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  char *parser;

  if (config != NULL)
  {
    printf("reading openvid file\n");
    while ((read = getline(&line, &len, config)) != -1)
    {
      parser = strtok(line, " ");
      ext[3] = '\0';
      printf("checking %s %s %d\n", parser, ext, strcmp(parser, ext));
      if (strcmp(parser, ext) == 0)
      {
        parser = strtok(NULL, "\n");
        printf("found app %s\n", parser);
        return parser;
      }
    }
  }
  return "open";
}

char *open_app(char *command)
{
  char *command_mod = strdup(command);
  char *token = strtok(command_mod, " ");
  char *path;
  char *ext;
  char *app;
  char *res;

  while (token != NULL)
  {
    ext = token;
    token = strtok(NULL, ".");
  }

  command_mod = strdup(command);
  token = strtok(command_mod, " ");
  path = strtok(NULL, " ");
  app = matchExt(ext);
  res = (char *)malloc(sizeof(char) * (strlen(app) + 1 + strlen(path)));
  strcpy(res, app);
  strcat(res, " ");
  strcat(res, path);
  return res;
}

void notify(struct tm td, char *message)
{
  printf("\n************************\n%s%s\n************************\n\n", asctime(&td), message);
}

void copy_file(char *sor, char *dest)
{
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *sorstream = fopen(sor, "r");
  FILE *deststream = fopen(dest, "w");

  if (sorstream != NULL)
  {
    while ((read = getline(&line, &len, sorstream)) != -1)
    {
      fprintf(deststream, "%s", line);
    }
  }

  fclose(sorstream);
  fclose(deststream);
}

void insertAlarm(char *timestr)
{
  struct tm td;
  time_t t;
  FILE *stream;
  strptime(timestr, "%H:%M:%S::%Y-%m-%d", &td);

  t = mktime(&td);

  stream = fopen("alarm.txt", "a");
  if (stream == NULL)
  {
    perror("Error opening file.");
  }
  else
  {
    fprintf(stream, "%d:%d:%d::%d-%d-%d\n", td.tm_hour, td.tm_min, td.tm_sec, td.tm_year + 1900, td.tm_mon + 1, td.tm_mday);
    notify(td, "Alarm Created");
  }
  // prompt();
  fclose(stream);
}

void checkAlarm()
{
  FILE *stream;
  FILE *temp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  struct tm td;
  time_t now = time(NULL);

  while (1)
  {
    stream = fopen("alarm.txt", "r");
    temp = fopen("alarmtemp.txt", "w");
    if (stream != NULL)
    {
      while ((read = getline(&line, &len, stream)) != -1)
      {
        strptime(line, "%H:%M:%S::%Y-%m-%d", &td);

        if (difftime(mktime(&td), now) <= 0.0)
          notify(td, "Alarm Triggered");
        else
          fprintf(temp, "%s", line);
        // prompt();
      }
      fclose(temp);
      fclose(stream);
      copy_file("alarmtemp.txt", "alarm.txt");
    }
    sleep(1);
  }
  free(line);
}

void alarm()
{
  int pid;
  if (fork() == 0)
  {
    checkAlarm();
  }
  else
  {
    //insert in this format "HH:mm:ss::YYYY-MM-DD"
    insertAlarm("18:39:01::2019-09-09");
    insertAlarm("18:37:01::2020-09-09");
    // while(1){
    //   printf("a\n");
    //   sleep(2);
    // }
  }
}

// void main() {
//

void prompt()
{
  if (root)
  {
    printf("root#");
  }
  else
  {
    printf("user$");
  }
}