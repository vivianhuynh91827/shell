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
    printf("%s $ ", getcwd(dir, 256));
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

    //   else if (args[0][0] != '\0'){
    else {
      execvp(args[0], args);
      free(command);
      return 0;
    }
  }
  /*
  int f = fork();
  if (f== -1){
    printf("Errno: %d, Error: %s\n",errno, strerror(errno));
  if (!fork()){
    wait(NULL);
  }
  else{
    char input[256];
    printf("Enter command: ");
    fgets(input, 256, stdin);
    input[strlen(input)-1] = '\0';
    char * s = input;
    char ** args = parse_args(s);
    execvp(args[0], args);
    return 0;
  }
  */
}
