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
char ** comms = args;
int args_ind = 0;
int comms_ind = 0;

// printf("%d\n", strcmp(comms[comms_ind],";"));
while (comms[comms_ind] && strcmp(comms[comms_ind],";")) {
  comms[comms_ind] = args[args_ind];
  printf("%s", comms[comms_ind]);
  args_ind++;
  comms_ind++;
}

if(strcmp(comms[0], "exit")==0){
  exit(0);
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
  execvp(comms[0], comms);
  // exit(0);
}
 return 0;
}
