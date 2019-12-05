#include "parse.h"

char ** parse_args(char * line){
  char ** args = malloc(6 * sizeof(line));
  char * token;
  for (int i = 0; token; i++){
    token = strsep(&line, " ");
    args[i] = token;
  }
  return args;
}
