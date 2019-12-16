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

/*=============== void exec_line(char * line) =============
  Input: char * line
  Returns: nothing

  Calls parse_args in order to discern commands seperated by semicolons and then calls exec_command
========================================================*/
void exec_line(char * line){
  char ** commands = parse_args(line, ";");
  int i = 0;
  while(commands[i]){
    exec_command(commands[i]);
    i++;
  }
}

/*======= void exec_command (char * command) =============
  Input: char * command
  Returns: nothing

  Calls parse_args in order to discern arguments and continues calling execute functions until it reaches the correct one
  If the correct exec command is called, it will return 1, causing exec_command to return
  Otherwise, it continues trying the other execute functions
========================================================*/
void exec_command(char * command){
  char ** args = parse_args(command, " ");
  //printf("len_args: %d\n", len_args(args));
  if (exec_pipe(args)) return;
  if (exec_cd(args)) return;
  if (exec_exit(args)) return;
  if (exec_double_redirect(args)) return;
  if (exec_redirect_output(args)) return;
  if (exec_redirect_input(args)) return;
  //if (exec_pipe(args)) return;
  else{
    exec_regular(args);
  }
}

/*=============== int exec_cd (char ** args) =============
  Input: char ** args
  Returns: 1 if cd is the first argument
           0 otherwise

  Executes cd
========================================================*/
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

/*=============== int exec_exit (char ** args) =============
  Input: char ** args
  Returns: 1 if exit is the first argument
           0 otherwise

  Executes exit
==========================================================*/
int exec_exit (char ** args){
  if (strcmp(args[0], "exit") == 0){
    printf("logout\n");
    printf("Saving session...\n");
    sleep(1);
    printf("...copying shared history...\n");
    sleep(1);
    printf("...saving history...truncating history files...\n");
    sleep(1);
    printf("...completed.\n\n");
    printf("[Process completed]\n");
    exit(0);
  }
  return 0;
}

/*=============== void exec_regular (char ** args) =============
  Input: char ** args
  Returns: Nothing

  Executes a regular command. Prints an error message if command is invalid.
==============================================================*/
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

/*=============== void exec_double_redirect (char ** args) =============
  Input: char ** args
  Returns: 1 if the command calls double redirecttion
           0 otherwise

  Executes double redirection
  There are two cases accounted for:
    1. Redirecting output twice e.g. ls -l > output > output2
    2. Redirecting input and then output e.g. tr a-z A-Z < output > output2
======================================================================*/
int exec_double_redirect(char ** args){
  if (count_redirects(args) == 2){
    int c = 0;
    while(args[c]){
      if (strcmp(args[c], ">") == 0) break;
      c++;
    }
    int f = fork();
    if (f) {
      int status;
      wait(&status);
    }
    else{
      // Redirect output twice
      if (count_redirect_in(args) == 2){
        char ** input = calloc(256, sizeof(char));
        for (int j = 0; j<c; j++){
          input[j] = args[j];
        }
        fclose(fopen(args[c+1], "w"));
        output(input, args[c+3]);
      }
      // Redirect input and then output
      else{
        char ** input = calloc(256, sizeof(char));
        for (int j = 0; j<(c - 2); j++){
          input[j] = args[j];
        }
        int fd_in = open(args[c - 1], O_RDONLY, 0644);
        if (fd_in < 0) {
          printf("Invalid input file %d: %s\n", errno, strerror(errno));
          return 1;
        }
        dup(STDIN_FILENO);
        dup2(fd_in, STDIN_FILENO);

        fclose(fopen(args[c+1], "w"));
        int fd_out = open(args[c+1], O_CREAT|O_WRONLY, 0644);
        dup(STDOUT_FILENO);
        dup2(fd_out, STDOUT_FILENO);

        int error = execvp(input[0], input);
        if (error == -1){
          printf("That's not a valid command\n");
        }
        close(fd_in);
        close(fd_out);
      }
    }
    return 1;
  }
  else{
    return 0;
  }
}

/*=============== void exec_redirect_output (char ** args) =============
  Input: char ** args
  Returns: 1 if redirect output is called
           0 otherwise

  Executes simple redirection using > (redirecting stdout)
======================================================================*/
int exec_redirect_output(char ** args){
  int c = 0;
  while(args[c]){
    if (strcmp(args[c], ">") == 0){
      int f = fork();
      if (f) {
        int status;
        wait(&status);
      }
      else{
        char ** input = calloc(256, sizeof(char));
        for (int j = 0; j<c; j++){
          input[j] = args[j];
        }
        output(input, args[c+1]);
      }
      return 1;
    }
    c++;
  }
  return 0;
}

/*=============== void output (char ** input, char * output) =============
  Input: char ** input
         char * output
  Returns: Nothing

  Executes the commands in input and outputs it to output
======================================================================*/
void output(char ** input, char * output){
  fclose(fopen(output, "w"));
  int fd = open(output, O_CREAT|O_WRONLY, 0644);
  dup(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);
  int error = execvp(input[0], input);
  if (error == -1){
    printf("That's not a valid command\n");
  }
  close(fd);
}

/*=============== void exec_redirect_input (char ** args) =============
  Input: char ** args
  Returns: 1 if redirect input is called
           0 otherwise

  Executes simple redirection using < (redirecting stdin)
======================================================================*/
int exec_redirect_input(char ** args){
  int c = 0;
  while(args[c]){
    if (strcmp(args[c], "<") == 0){
      int f = fork();
      if (f) {
        int status;
        wait(&status);
      }
      else{
        char ** a = calloc(256, sizeof(char));
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
        int error = execvp(a[0], a);
        if (error == -1){
          printf("That's not a valid command\n");
        }
        close(fd);
      }
      return 1;
    }
    c++;
  }
  return 0;
}

/*=============== void exec_pipe (char ** args) =============                  
  Input: char ** args                                                     
  Returns: 1 if redirect input is called       
           0 otherwise                                                                   
  Executes simple pipe command              
======================================================================*/
int exec_pipe(char ** args) {
  if (len_args(args) != 3){
    return 0;
  }
  if (strcmp(args[1], "|")==0){
    FILE * in = popen( args[0], "r" );
    if (!in) {
      printf("errno: %d, error: %s\n", errno, strerror(errno));
      return 0;
    }
    FILE * out = popen( args[2], "w" );
    if (!out) {
      printf("errno: %d, error: %s\n", errno, strerror(errno));
      return 0;
    }
    char buff[256];
    while (fgets( buff,256,in )) {
      fputs( buff,out );
    }
    pclose(in);
    
    pclose(out);
    return 1;
  }
  return 0;
} 
