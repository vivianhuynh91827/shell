#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "parse.h"
#include "execute.h"

int main(){
  printf("Initiating shell\n");
  char * s = malloc(256);
  printf("Enter command: ");
  fgets(s, 256, stdin);
  printf("Your command was %s\n", s);
  while(1){
    signal(SIGINT, sig_handler);
    
  }
}
