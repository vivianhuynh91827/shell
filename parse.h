#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char ** parse_args(char * line);
int get_len_args(char ** args);
char ** parse_line(char * line);
char * strip(char * line);
