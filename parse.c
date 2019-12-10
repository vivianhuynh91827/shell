#include "parse.h"

int parse_args(char * line, char ** args){
  char * token = strsep(&line, " ");
  int cur_ind = 0;
  while (token){
    args[cur_ind] = token;
    cur_ind ++;
    token = strsep(&line, " ");
  }
  return cur_ind;
}
