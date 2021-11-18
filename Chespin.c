#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

/***
parse commands based on " "

  param: fgets string from stdin
  return: an array of arguments ready for execvp

***/
char ** parse_args( char * line ){
  // allocate memory for commands (6 might not be enough size)
  char ** args = calloc(6, sizeof(char*));

  // get rid of \n in fgets
  line = strsep(&line, "\n");

  // seperate command by " ", read each into args array
  char* token;
  int i = 0;
  while(token = strsep(&line," ")){
  args[i] = token;
   i++;
  }

  // make sure last element is null.
  args[i]= NULL;

  return args;
}


/***

reads and executes commands
  return: errno if there is an error, otherwise 0.
  
***/
int run(){

// gets the command from stdin
 char line [100];
 fgets(line, 100, stdin);

//parse arguments
 char ** args = parse_args( line );

//initiate child process


//execute commands
 execvp(args[0], args);
 return 0;
}
