#include "parse.h"

char ** parse_args(char * line){
  char ** args = calloc (6, 256 * sizeof(char));
  char * stripped = strip(line);
  char * token = strsep(&stripped, " ");
  int ind = 0;
  while (token) {
    args[ind] = token;
    ind ++;
    token = strsep(&stripped, " ");
  }
  return args;
}

int get_len_args(char ** args){
  int len = 0;
  while (args[len] != NULL){
    len ++;
  }
  if (len == 0) return 1;
  return len;
}

char ** parse_line(char * line){
  char ** commands = calloc(6, 256 * sizeof(char));
  char * token = strsep(& line, ";");
  int ind = 0;
  while (token){
    commands[ind] = token;
    ind++;
    token = strsep(&line, ";");
  }
  return commands;
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
