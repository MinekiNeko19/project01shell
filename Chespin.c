#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<signal.h>

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
  char* token=calloc(1, 100);
  int i = 0;
  while(token = strsep(&line," ")){

  args[i] = token;
   i++;
  }
//args[i] = strsep(&line," ");
  // make sure last element is null.
  args[i]= NULL;

  return args;
}

/***

change current working directory
  return: errno if there's an error.

***/
int cd(char ** args){
  chdir(args[1]);
//maybe add home replacement
  return errno;
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
 char ** args =parse_args( line );

printf("args[1]: %s\n", args[1]);
 if(strcmp(args[0], "exit")==0){
   exit(0);
 }

else if(strcmp(args[0], "cd") == 0){
  //cd(args);
  chdir(args[1]);
}else{
//initiate child process
int child1 = fork();

if (child1 == 0){
//execute commands
 execvp(args[0], args);

}
}
 return 0;
}
