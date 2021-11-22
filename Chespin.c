#include "Chespin.h"
// WHY DOES ENTERING YES AS AN ARGUMENT BREAK THE SHELL WHAT??

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
  Changes the current working directory
    param: command ["cd", "path"]
    returns: errno
***/

// it prints an error whenever I use it but it actually changes the directory so it's weird
int cd(char ** args){

  if(args[1][0] == '~'){
    char * path = getenv("HOME");
    strcat(path, ++args[1]);
    printf("path: %s\n", path);
    chdir(path);
  }else{
    chdir(args[1]);
  }
}
/***

reads and executes commands
  return: errno if there is an error, otherwise 0.

***/
int run(){

// gets the command from stdin

char line [100];
char current_dir[100];
getcwd(&current_dir,100);

int w,status;

printf("%s:$ ", current_dir); // (๑•̀ㅂ•́)و✧
fgets(line, 100, stdin);

//parse arguments
char ** args = parse_args( line );

if(strcmp(args[0], "exit")==0){
  exit(0);
}

else if(strcmp(args[0],"cd")==0){
  int a = cd(args);


  //chdir(args[1]);
}
//initiate child process
int child1 = fork();

if (child1) {
  w = wait(&status);
  // printf("child %d finished; parent %d resumes\n",child1,getpid());
}
else if (!child1){
  // printf("pid child: %d\tparent: %d\n", getpid(),getppid());
  //execute commands
  execvp(args[0], args);
  exit(0);
}
 return 0;
}
