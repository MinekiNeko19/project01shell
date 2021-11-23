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
    param: parsed command ["cd", "path"]
    returns: errno
***/

// it prints an error whenever I use it but it actually changes the directory so it's weird
int cd(char ** args){

// if the user entered an absolute path from the home directory
  if(args[1][0] == '~'){
    //gets home directory of the user
    char * path = getenv("HOME");
    //concate home path with user's entered path
    strcat(path, ++args[1]);
    printf("path: %s\n", path);
    chdir(path);
  }else{
    chdir(args[1]);
  }

  return errno;
}


char ** redirect_helper(int fd, int std, int * out, char ** temp){

  dup2(fd,std);
  *temp = NULL;
  temp[1] = NULL;
  temp+=2;
  out[0] = std;

  return temp;
}

/***
Redirect to files if the commands contains instructions regarding redirection
  params: parsed commands
  return: file descriptor of the replaced, whether stdin or stdout is replaced [0 (stdin) or 1 (stdout), file descriptor of its location]
***/

int * redirect(char ** args){
  char ** temp = args;

  while(*temp && temp){
      int * out = malloc(2*sizeof(int));
    if (strcmp(*temp,">") == 0){
      //printf("stdout\n");
      int stdout = dup(1);
      int fd = open(temp[1], O_CREAT|O_TRUNC|O_WRONLY, 0644);
      temp = redirect_helper(fd,1,out,temp);
      out[1] = stdout;
      return out;
    }else if (strcmp(*temp,">>")==0){
      //printf("append\n");
      int stdout = dup(1);
      int fd = open(temp[1], O_CREAT|O_APPEND|O_WRONLY, 0644);
      temp = redirect_helper(fd,1,out,temp);
      out [1] = stdout;
      return out;
    }else if (strcmp(*temp, "<")==0){
      //printf("stdin\n");
      int stdin = dup(0);
      int fd = open(temp[1],O_RDONLY);
      temp = redirect_helper(fd,0,out,temp);
      out[1] = stdin;
      return out;
    }


    temp++;
  }
  int * out = malloc(2*sizeof(int));
  out[0] = -1;
  out [1] = -1;
  return out;
}
/***
reads and executes commands
  return: errno if there is an error, otherwise 0.
***/

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
char ** comms = malloc(sizeof(args));
int args_ind = 0;
int comms_ind = 0;

while (args[args_ind]) {
  // printf("%s", comms[comms_ind]);
  if (strcmp(args[args_ind],";")) {
    comms[comms_ind] = args[args_ind];
    printf("copying %s into comms\n",args[args_ind]);
  }
  args_ind++;
  comms_ind++;

  if (!comms[comms_ind]) {
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
      printf("%s\n",comms[0]);
      execvp(comms[0], comms);
      exit(0);
    }
    comms_ind = 0;
    args_ind++;
  }


}

 return 0;
}
