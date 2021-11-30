#include "Chespin.h"

/***

prints errno and sets it back to 0

***/

void print_err(){
  if(errno){
    printf("%s\n",strerror(errno));
  }
  errno = 0;
}

/***
parse commands based on " "

  param: fgets string from stdin
  return: an array of arguments ready for execvp

***/
char ** parse_args( char * line ){
  // allocate memory for commands (10 args taken)
  char ** args = calloc(10, sizeof(char*));

  // get rid of \n in fgets
  line = strsep(&line, "\n");

  // seperate command by " ", read each into args array
  char* token;
  int i = 0;

  while(token = strsep(&line," ")){
    //limits amount of args to 10
    if(i<10){
      args[i] = token;
      i++;
    }
    else {
      //error handling
      printf("Too many arguments. Max: 10 arguments.\n");
      return NULL;
    }
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
  //gets home directory of the user
  char * path = getenv("HOME");

  // if user does not give arguments, default to home directory
  if(args[1]==NULL){
    chdir(path);
  }

  else {
    // if the user entered a path from the home directory
    if(args[1][0] == '~'){

      //concate home path with user's entered path
      strcat(path, ++args[1]);

      chdir(path);

      //prints error
      print_err();
    }
    else {
        // changes directory relative to the current directory / root directory
        chdir(args[1]);
        print_err();
    }
  }
  return errno;
}


/***
Refactored code in redirection
  param:
    fd - file descriptor of the file to input/output to
    std - 0 for stdin, 1 for stdout
    out - containing information regarding whether it's a stdin/out
    temp - location of pointer in the argument list
  return:
    temp - the location of pointer after movement
***/
char ** redirect_helper(int fd, int std, int * out, char ** temp){

  //replaces std/stdout with file descriptor
  dup2(fd,std);

  //setting arguments to null
  *temp = NULL;
  temp[1] = NULL;
  temp+=2;

  //recording whether it's stdin or out
  out[0] = std;

  return temp;
}

/***
Does piping
  param:
    sep - where the pip "|" is
    start - the start of the piping process
  return 0
***/

int pip(char ** sep, char ** start){

  // malloc memory for commands
  char * cmd1 = malloc (100);
  char * cmd2 = malloc (100);

  // joining commands separated with spaces together
  while (start != sep){
    strcat(cmd1, " ");
    strcat(cmd1,start[0]);
    start++;
  }
  cmd1++;

  // moving to the other end of pip
  *sep = NULL;
   sep++;

   // joining the other end of pip
  while (sep && *sep){
    strcat(cmd2, " ");
    strcat(cmd2, sep[0]);

    sep++;
  }
  cmd2++;

  // doing piping
  FILE *p1 = popen(cmd1,"r");
  FILE *p2 = popen(cmd2,"w");

  while (!feof(p1)) {

    fputc(fgetc(p1),p2);

  }

  pclose(p1);
  pclose(p2);

  return 0;
}

/***

Redirect to files if the commands contains instructions regarding redirection
  params: parsed commands
  return: file descriptor of the replaced, whether stdin or stdout is replaced [0 (stdin) or 1 (stdout), file descriptor of its location]

***/

int * redirect(char ** args){
  char ** temp = args;
  int * out = malloc(2*sizeof(int));

  while(*temp && temp){

    if (strcmp(*temp,">") == 0){
      // duplicate stdout
      int stdout = dup(1);
      // open file to output to
      int fd = open(temp[1], O_CREAT|O_TRUNC|O_WRONLY, 0644);
      // redirect
      temp = redirect_helper(fd,1,out,temp);
      // record stdout copied location
      out[1] = stdout;
      return out;
    }else if (strcmp(*temp,">>")==0){
      // duplicate stdout
      int stdout = dup(1);
      // open file to output to
      int fd = open(temp[1], O_CREAT|O_APPEND|O_WRONLY, 0644);
      // redirect
      temp = redirect_helper(fd,1,out,temp);
      // record stdout copied location
      out [1] = stdout;
      return out;
    }else if (strcmp(*temp, "<")==0){
      // duplicate stdin
      int stdin = dup(0);
      // open file to take input from
      int fd = open(temp[1],O_RDONLY);
      // error handling if file does not exist
      print_err();
      if(errno){
          out[0] = -2;
          out [1] = -2;
          return out;
      }
      // redirect
      temp = redirect_helper(fd,0,out,temp);
      // record stdin location
      out[1] = stdin;
      return out;
    }else if (strcmp(*temp, "|")==0){
      // does piping
      pip(temp, args);

      out[0] = -2;
      out [1] = -2;
      return out;
    }
    temp++;
  }

  out[0] = -1;
  out[1] = -1;
  return out;
}

/***
Refactored code for executing commands
  params:
    red - redirection returning information
    comms - commands
***/
int exec(int red[2], char ** comms){

  //initiate child process
  int w,status;
  int child1 = fork();

  if (child1) {
    // wait for child to terminate
      w = wait(&status);

    // check for errors
     if(WEXITSTATUS(status)){
       printf("Command not found: %s\n",comms[0]);
     }

    // if redirection, give back stdin/out
    if(red[0] != -1){
      dup2(red[1], red[0]);
    }

  }

  else {
    //execute commands if not piping
    if(red[0] != -2){
      exit(execvp(comms[0], comms));
    }

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
getcwd(current_dir,100);

// prompting
printf("%s:✧ ", current_dir); // (๑•̀ㅂ•́)و✧
fgets(line, 100, stdin);

//parse arguments
char ** args = parse_args( line );

// if arguments bad, stop
if(args == NULL){
  return 0;
}

// check redirection
int * red = redirect(args);

// exit
if(strcmp(args[0], "exit")==0){
  exit(0);
}

//cd
else if(strcmp(args[0],"cd")==0){
  int a = cd(args);
}

// the other commands
else {
  // copy current running args into comms
  char ** comms = calloc(6,sizeof(char*));
  int args_ind = 0;
  int comms_ind = 0;

  // handles ; chaining
  while(args[args_ind]) {

    // copy current command into comms
    if (strcmp(args[args_ind],";")) {
      comms[comms_ind]=args[args_ind];
      comms_ind++;
      args_ind++;
    }
    // execute comms
    else {
      // skips copying ;
      args_ind++;
      comms[comms_ind]=NULL;

      exec(red,comms);

      // resets comms
      comms_ind = 0;
    }
  }

  comms[comms_ind]=NULL;
  exec(red,comms);
  free(comms);
}
  free(red);


return 0;
}
