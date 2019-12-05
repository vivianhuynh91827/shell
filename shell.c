#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "parse.h"
#include "execute.h"

int main(){
  printf("Initiating shell\n");
  int parent = getpid();
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
  }
  /*
  while(1){
    signal(SIGINT, sig_handler);
  }
  */
}
