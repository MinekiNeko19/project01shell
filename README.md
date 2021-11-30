# Shellos Shell
Group Members: Sarah Leong, Yuqing Wu

# Our shell implements:
Please note that our shell only accepts 10 arguments in one line. Anything surrounded by spaces (along with the first character and the final newline character) is considered an argument.
- commands stored in `/bin`
- `cd`
  - Moves current working directory
  - Usage: `cd [directory path]`
  - Example: `cd /documents`
  - `cd ~` refers to user account's home directory 
  - `cd` (with no follow up arguments) moves working directory to home directory
- `exit`
  - Exits the shell 
- simple piping using `|`
  - Usage: `[command1] | [command2]`
  - Example: `ls | wc`
- simple redirection using `>`,`<`,`>>`,`<<`
  - Does not implement chained redirection (Do not redirect multiple times in one line) 
  - Usage: `[command1] > [command2]`
  - Example: `cat > meow.txt`
- run a series of commands in one line using `;`
  - Usage: `[command 1] ; [command 2]`
  - Example: `ls -l ; echo hello`
 
# Unimplemented/Unsuccessful
- piping multiple times in one line
- unrestricted ordering and locations of redirection (chained redirection)
- unlimited # inputted args
- 
# Eek! Bugs!!
- crashes when doing `| |`, or some other improper use of piping
- crashes for silly syntax like `> > > >`
- error messages can get kind of weird at times
- sometimes there is random weird behavior that fixes itself
  - Example: echo caused `malloc(): corrupted top size` errors but it fixed itself after more runs

_Fixed Bugs_
- Extra spaces breaks the shell
- `;` at the end of the line breaks the shell
- Working directory moves but shell also prints error
- Error messages print although no error occurred
- Defunct forked processes

# Function Headers
- `char ** parse_args( char * line );`
- `int run();`
- `int cd(char ** args);`
- `int * redirect(char ** args);`
- `void print_err();`

Helpers 
- `char ** redirect_helper(int fd, int std, int * out, char ** temp)`
- `int pip(char ** sep, char ** start)`
- `int exec(int red[2], char ** comms)`
