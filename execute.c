#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/dir.h>
#include <time.h>
#include <sys/wait.h>
#include "execute.h"
#include "parse.h"

void exec_line(char * line){
  char ** commands = parse_args(line, ";");
  int i = 0;
  while(commands[i]){
    exec_command(commands[i]);
    i++;
  }
}

void exec_command(char * command){
  char ** args = parse_args(command, " ");
  if (exec_cd(args)) return;
  if (exec_exit(args)) return;
  else{
    exec_regular(args);
  }
}

int exec_cd (char ** args){
  if (strcmp(args[0], "cd") == 0){
    int fd = chdir(args[1]);
    if (fd != 0){
      printf("errno: %d, error: %s\n", errno, strerror(errno));
    }
    return 1;
  }
  return 0;
}

int exec_exit (char ** args){
  if (strcmp(args[0], "exit") == 0){
    printf("logout\n");
    printf("Saving session...\n");
    printf("...copying shared history...\n");
    printf("...saving history...truncating history files...\n");
    printf("...completed.\n\n");
    printf("[Process completed]\n");
    exit(0);
  }
  return 0;
}

void exec_regular (char ** args){
  int f = fork();
  if (f) {
    int status;
    wait(&status);
  }
  else{
    execvp(args[0], args);
  }
}
