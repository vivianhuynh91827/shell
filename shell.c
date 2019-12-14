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

/*=========== int main() ==================
  Input: nothing
  Returns: nothing

  Forever loop that displays the user's path and prompts the user for input
=========================================*/
int main(){
  printf("\nInitiating shell\n");
  char input[256];
  char * dir = malloc(256 * sizeof(char));
  while(1){
    printf("%s$ ", getcwd(dir, 256));
    fgets(input, 256, stdin);
    input[strlen(input)-1] = '\0';
    char * line = input;
    exec_line(line);
  }
}
