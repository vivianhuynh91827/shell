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
#include <fcntl.h>
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
  //printf("len_args: %d\n", len_args(args));
  if (exec_cd(args)) return;
  if (exec_exit(args)) return;
  if (exec_redirect_output(args)) return;
  if (exec_redirect_input(args)) return;
  //if (exec_pipe(args)) return;
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
    int error = execvp(args[0], args);
    if (error == -1){
      printf("That's not a valid command\n");
    }
  }
}

int exec_redirect_output(char ** args){
  int c = 0;
  while(args[c]){
    if (strcmp(args[c], ">") == 0){
      char ** input = calloc(128, 256 * sizeof(char));
      for (int j = 0; j<c; j++){
        input[j] = args[j];
      }
      int fd = open(args[c + 1], O_CREAT|O_EXCL|O_WRONLY, 0755);
      if (fd < 0) {
        fd = open(args[c + 1], O_WRONLY);
      }
      dup(STDOUT_FILENO);
      dup2(fd, STDOUT_FILENO);
      execvp(input[0], input);
      close(fd);
      return 1;
    }
    c++;
  }
  return 0;
}

int exec_redirect_input(char ** args){
  int c = 0;
  while(args[c]){
    if (strcmp(args[c], "<") == 0){
      char ** a = calloc(128, 256 * sizeof(char));
      for (int j = 0; j<c; j++){
        a[j] = args[j];
      }
      int fd = open(args[c + 1], O_RDONLY, 0644);
      if (fd < 0) {
        printf("Invalid input file %d: %s\n", errno, strerror(errno));
        return 1;
      }
      dup(STDIN_FILENO);
      dup2(fd, STDIN_FILENO);
      execvp(a[0], a);
      close(fd);
      return 1;
    }
    c++;
  }
  return 0;
}

int exec_pipe(char ** args) {
  if (strcmp(args[1], "|")==0){
    FILE * in = popen( args[0], "r" );
    if (in < 0) {
      printf("errno: %d, error: %s\n", errno, strerror(errno));
    }
    FILE * out = popen( args[2], "w" );
    if (out < 0) {
      printf("errno: %d, error: %s\n", errno, strerror(errno));
    }
    char buff[256];
    char * input = fgets( buff, 256, in );
    while ( input ) {
      fputs( buff, out );
    }
    pclose(in);
    pclose(out);
    return 1;
  }
  return 0;
}
