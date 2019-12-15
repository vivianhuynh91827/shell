#include "parse.h"

/*====== char ** parse_args(char * line, char * delim) ========
  Input: char * line
         char * parsed
  Returns: Double pointer to parts of line seperated by delim
=============================================================*/
char ** parse_args(char * line, char * delim){
  char ** args = calloc (256, sizeof(char));
  char * stripped = strip(line);
  char * token = strsep(&stripped, delim);
  int ind = 0;
  while (token) {
    args[ind] = token;
    ind ++;
    token = strsep(&stripped, delim);
  }
  return args;
}

/*======== char * strip(char * line) ==========
  Input: char * line
  Returns: line without any leading or trailing white space
=============================================*/
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

/*======== int len_args (char ** chars) ==========
  Input: char ** args
  Returns: Number of arguments
================================================*/
int len_args(char ** args){
  char * cur_arg = args[0];
  int len = 0;
  while (cur_arg){
    len ++;
    cur_arg = args[len];
  }
  return len;
}

/*======== int count_redirects (char ** chars) ==========
  Input: char ** args
  Returns: Number of occurences of > or <
=======================================================*/
int count_redirects(char ** args){
  int ans = 0;
  int i = 0;
  while(args[i]){
    if (strcmp(args[i],">") == 0 || strcmp(args[i], "<") == 0){
      ans++;
    }
    i++;
  }
  return ans;
}

/*======== int count_redirect_in (char ** chars) ==========
  Input: char ** args
  Returns: Number of occurences of >
=======================================================*/
int count_redirect_in(char ** args){
  int ans = 0;
  int i = 0;
  while(args[i]){
    if (strcmp(args[i],">") == 0){
      ans++;
    }
    i++;
  }
  return ans;
}
