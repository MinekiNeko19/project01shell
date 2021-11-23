#ifndef CHESPIN_H
#define CHESPIN_H

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>
#include<sys/wait.h>
#include<fcntl.h>

/***
parse commands based on " "

  param: fgets string from stdin
  return: an array of arguments ready for execvp

***/
char ** parse_args( char * line );

/***

reads and executes commands
  return: errno if there is an error, otherwise 0.

***/
int run();

int cd(char ** args);

int * redirect(char ** args);
#endif
