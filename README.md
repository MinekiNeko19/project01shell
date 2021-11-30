# Shellos Shell
Group Members: Sarah Leong, Yuqing Wu

**Our shell implements:**
- commands stored in /bin
- cd
  - cd with ~ and cd with no arguments default to home.
- exit
- piping using |, ex. [command1] | [command2]
- redirection using >,<,>>, no chaining. ex: [command1] > [command2]
- ; Usage: [command 1] ; [command 2]. Put spaces between the commands and the ;.

**Eek! Bugs!!**
- [ ] crashes when doing "| |", or some other bad use of piping
- [ ] crashes for things like " > > > > "


**WIP**
- [ ] signal handling

_Optional_
- [ ] piping multiple times in one line
- [ ] unrestricted ordering and locations of redirection
- [X] colored shell prompting
- [ ] unlimited # inputted args
