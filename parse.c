#include "parse.h"
/*
int  parse_args(char * line, char ** args){
  // args = realloc(args, 6 * sizeof(char *));
  char * token = strsep(&line, " ");
  int cur_ind = 0;
  while (token){
    args[cur_ind] = token;
    cur_ind ++;
    token = strsep(&line, " ");
  }
  return cur_ind;
}
*/

char ** parse_args(char * line){
  char ** args = calloc (6, 256 * sizeof(char));
  char * token = strsep(&line, " ");
  int ind = 0;
  while (token) {
    args[ind] = token;
    ind ++;
    token = strsep(&line, " ");
  }
  return args;
}

int get_len_args(char ** args){
  return 0;
}
