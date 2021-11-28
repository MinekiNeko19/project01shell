#include "Chespin.h"

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

// has a bug about puting empty spaces after the command

char ** parse_args( char * line ){
  // allocate memory for commands (6 might not be enough size)
  char ** args = calloc(6, sizeof(char*));

  // get rid of \n in fgets
  line = strsep(&line, "\n");

  // seperate command by " ", read each into args array
  char* token;
  int i = 0;
  while(token = strsep(&line," ")){
    if(i<5){
  args[i] = token;
   i++;
 }else{
   printf("Too many arguments\n");
   return NULL;
 }
  }

  // make sure last element is null.
  args[i]= NULL;
  print_err();
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
    //printf("path:%s\n", path);
    chdir(path);
  } 
  
  else {
    // if the user entered an absolute path from the home directory
    if(args[1][0] == '~'){

      //concate home path with user's entered path
      strcat(path, ++args[1]);
      //printf("path: %s\n", path);
      chdir(path);

      print_err();
    }
    else {
        chdir(args[1]);
        print_err();
    }
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

int pip(char ** sep, char ** start){

  char * cmd1 = malloc (100);
  char * cmd2 = malloc (100);

  while (start != sep){
    strcat(cmd1, " ");
    strcat(cmd1,start[0]);
    start++;
  }
  cmd1++;
  //printf("cmd1:%s\n", cmd1);

  *sep = NULL;
   sep++;

  while (sep && *sep){
    strcat(cmd2, " ");
    strcat(cmd2, sep[0]);

    sep++;
  }
  cmd2++;
  //printf("cmd2:%s\n", cmd2);


  FILE *pipefrom = popen(cmd1,"r");
  FILE *pipeto = popen(cmd2,"w");
  while (1) {
    if (feof(pipefrom)) {break;}

    fputc(fgetc(pipefrom),pipeto);
    //reading file stream ... read pip from and pipeto to see whtat's going on
    //printf("pipfrom\n", read())
  }

  pclose(pipefrom);
  pclose(pipeto);

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

      print_err();
      if(errno){
          out[0] = -2;
          out [1] = -2;
          return out;
      }
      temp = redirect_helper(fd,0,out,temp);
      out[1] = stdin;
      return out;
    }else if (strcmp(*temp, "|")==0){
      pip(temp, args);
      out[0] = -2;
      out [1] = -2;
      return out;
    }


    temp++;
  }

  out[0] = -1;
  out [1] = -1;
  return out;
}

int exec(int red[2], char ** comms){
  // run the final commmand b/c its ignored in the first while loop
  //initiate child process
  int w,status;
  int child1 = fork();

  if (child1) {

      w = wait(&status);
    // printf("child %d finished; parent %d resumes\n",child1,getpid());
     if(WEXITSTATUS(status)){
       printf("Command not found: %s\n",comms[0]);
     }

    if(red[0] != -1){
      dup2(red[1], red[0]);
      //exit(0); // if we put it here redirection don't work but if not exit don't function properly, or maybe exit does funciton idrk lol
    }

  }
  else {
    //execute commands
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
int w,status;

// prompting
printf("%s:✧ ", current_dir); // (๑•̀ㅂ•́)و✧
fgets(line, 100, stdin);

//parse arguments
char ** args = parse_args( line );
if(args == NULL){
  return 0;
}
int * red = redirect(args);


if(strcmp(args[0], "exit")==0){
  exit(0);
}

else if(strcmp(args[0],"cd")==0){
  int a = cd(args);
}

else {
  // copy current running args into comms
  char ** comms = calloc(6,sizeof(char*));
  int args_ind = 0;
  int comms_ind = 0;

  while(args[args_ind]) {
  //     printf("args[%d]: %s\n",args_ind, args[args_ind]);
  //     printf("comms[%d]: %s\n",args_ind, comms[comms_ind]);

    // copy current command into comms
    if (strcmp(args[args_ind],";")) {
      comms[comms_ind]=args[args_ind];
      // printf("comms[%d] stores %s\n", comms_ind,comms[comms_ind]);
      comms_ind++;
      args_ind++;
    }
    // execute comms
    else {
      // printf("%s\n",args[args_ind]);

      // skips copying ;
      args_ind++;
      comms[comms_ind]=NULL;

    exec(red,comms);
      // resets comms
      comms_ind = 0;
    }
  }

  exec(red,comms);
   free(comms);
}
  free(red);


return 0;
}
