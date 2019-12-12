#include "parse.h"

char ** parse_args(char * line, char * parsed){
  char ** args = calloc (6, 256 * sizeof(char));
  char * stripped = strip(line);
  char * token = strsep(&stripped, parsed);
  int ind = 0;
  while (token) {
    args[ind] = token;
    ind ++;
    token = strsep(&stripped, parsed);
  }
  return args;
}

char * strip(char * line){
  char * start = line;
  while(* start == ' '){
    start++;
  }
  char * end = line;
  while(* end){
    end++;
  }
  end--;
  while (* end == ' ') {
    * end = 0;
    end--;
  }
  return start;
}

int len_args(char ** args){
  char * cur_arg = args[0];
  int len = 0;
  while (cur_arg){
    len ++;
    cur_arg = args[1];
  }
  return len;
}
