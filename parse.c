#include "parse.h"

char ** parse_args(char * line){
  char ** args = malloc(6 * sizeof(char*));
  char * token = strsep(&line, " ");
  int cur_ind = 0;
  while (token){
    args[cur_ind] = token;
    token = strsep(&line, " ");
  }
  return args;
}
