# Shellos Shell
Group Members: Sarah Leong, Yuqing Wu

**Our shell implements:**
- commands stored in /bin
- cd
  - cd with `~` 
  - cd with no arguments moves working directory to home directory
- exit
- piping using `|`, ex. `[command1] | [command2]`
- redirection using `>`,`<`,`>>`,`<<` no chaining. ex: `[command1] > [command2]`
- `;` ex: `[command 1] ; [command 2]`. Put spaces between the commands and the `;`.

**Eek! Bugs!!**
- crashes when doing `| |`, or some other bad use of piping
- crashes for things like `> > > > `
- error messages can get kind of weird at times
- sometimes there is random weird behavior that fixes itself
  - ex.: echo caused `malloc(): corrupted top size` errors but it fixed itself after more runs

**Function Headers**
- `char ** parse_args( char * line );`
- `int run();`
- `int cd(char ** args);`
- `int * redirect(char ** args);`
- `void print_err();`

**Unimplemented/Unsuccessful**
- piping multiple times in one line
- unrestricted ordering and locations of redirection (chained redirection)
- unlimited # inputted args
