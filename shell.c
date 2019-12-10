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
  char ** args;
  char * dir= malloc(256 * sizeof(char));;
  while(1){
    signal(SIGINT, sighandler);
    printf("%s$ ", getcwd(dir, 256));
    fgets(command, 256, stdin);
    command[strlen(command)-1] = '\0';
    char * s = command;
    
    args = parse_args(s);

    if (strcmp(args[0], "exit") == 0){
      exit(0);
    }

    else if (strcmp(args[0], "cd")==0){
      //cd
    }

    else {
      if (!fork()){
	execvp(args[0], args);
	free(command);
	return 0;
      }
    }
  }
}
