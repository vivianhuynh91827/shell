#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char ** parse_args(char * line, char * parsed);
int len_args(char ** args);
char * strip(char * line);
