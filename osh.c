#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 80 /* The maximum length command */
#define MAXCMD 10000

void parse(char commandline[],char*args[])
{

        char **next = args;
        char *temp = strtok(commandline, " \n");
        while (temp != NULL)
        {
            *next++ = temp;
            temp = strtok(NULL, " \n");
        }
        *next = NULL;
}
void print_history(int cmd_counter, char **cmd_array)
{
  for (int i = cmd_counter-1; i > cmd_counter-11; i--)
    if (i > 0)
    printf("%d %s\n", i, cmd_array[i]);




}
int main(int argc, char **argv[])
{
  char *args[MAXLINE/2 + 1]; /* command line arguments */
  char *cmd_array[MAXCMD];
  int cmd_counter = 1;
  int should_run = 1; /* flag to determine when to exit program */
  pid_t pid = 0;
  char *commandline;
  size_t sizebuffer = 0;
  char *history = "history\0";
  char *copier;
  copier = malloc(1024);



  for (int i = 0; i < MAXCMD; i++)
  {
    cmd_array[i] = malloc(1024);

    if (!cmd_array[i])
    {
      printf("Failed to allocate cmd_array[%i]", i);
    }
  }
  while (should_run)
  {

      do
      {
        fflush(stdout);
        printf("osh> ");
        fflush(stdout);
        getline(&commandline, &sizebuffer, stdin);
      } while(strcmp(commandline, "\n") == 0);

      // Checking that it is not a bang call, for it to be stored it has to be a normal command not a bang

      if (commandline[0]!='!')
      {
          strcpy(cmd_array[cmd_counter], commandline);
          cmd_counter++;
      }
      // Reserving the original command line because the parsing fills the sent pointer to the array with null characters
      // making it impossible to reprocess it as a commandline

      strcpy(copier, commandline);
      parse(copier, args);

      pid = fork();

      if (pid < 0)
      {
        fprintf(stderr, "Fork Failed");
        exit(-1);
      }
      else if (pid == 0)
        {
          fflush(stdout);
          if (!strcmp(args[0],history))
            {
                print_history(cmd_counter, cmd_array);

            }
            else
            {
              if (!strcmp(args[0],"!!\0"))
              {
                if (cmd_counter > 1)
                {

                strcpy(copier, cmd_array[(cmd_counter-1)]);
                strcpy(cmd_array[cmd_counter], cmd_array[cmd_counter-1]);

                cmd_counter++;

                parse(copier, args);

                if (!strcmp(args[0],history))
                {
                    print_history(cmd_counter, cmd_array);
                    exit(0);
                }
              }
              else
              {
                  printf("\n No Previous Commands \n");
                  exit(0);
              }
            }

            else
            {
              if (commandline[0]=='!')
              {
                  if (cmd_counter > 1 )
                  {
                    int cmd_idx = (atoi(&commandline[1]));

                    strcpy(commandline, cmd_array[cmd_idx]);
                    strcpy( cmd_array[cmd_counter], commandline);
                    cmd_counter++;

                    strcpy(copier, commandline);
                    printf("%s", copier);
                    parse(copier, args);
                    if (!strcmp(args[0],history))
                    {
                        print_history(cmd_counter, cmd_array);

                    }
                  }
                  else
                  {
                    printf("\n No Previous Commands \n");
                    exit(0);
                  }

              }

            }
              if (execvp(args[0],args) == -1)
              {
                printf("\n%s Command Not found\n", commandline);
              }

            }

            exit(0);
        }
      else
      {


        wait(NULL);


      }

    }
    return 0;

}
