#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "parse.h"
#include "execute.h"

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("\nProgram exit due to SIGINT");
    exit(0);
  }
}


int main(){
  printf("Initiating shell\n");
  int parent = getpid();
  char * command= malloc ( 256 * sizeof(char));
  char ** args= malloc ( 6 * sizeof(char *));
  char * dir= malloc(256 * sizeof(char));;

  while(1){
    signal(SIGINT, sighandler);
    printf("%s$ ", getcwd(dir, 256));
    fgets(command, 256, stdin);
    command[strlen(command)-1] = '\0';
    char * s = command;
    int len_args = parse_args(s, args);
    printf("%d\n", len_args);
    if (strcmp(args[0], "exit") == 0){
      exit(0);
    }

    else if (strcmp(args[0], "cd")==0){
      if (len_args < 2 || len_args > 2){
	printf("Incorrect Syntax: cd <PATH>\n");
      }
      else {
	int fd = chdir(args[1]);
	if (!fd){
	  printf("errno: %d, error: %s\n", errno, strerror(errno));
	}
      }
    }

    else {
      int f= fork();
      if (f) {
	int status;
	wait(&status);
      }
      else{
	execvp(args[0], args);
	return 0;
      } 
    }

    free(command);
    for (int i = 0; i < len_args; i ++){
      free(&args[i]);
    }
  }
}
