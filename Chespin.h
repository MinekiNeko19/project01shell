#ifndef CHESPIN_H
#define CHESPIN_H
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
#endif
