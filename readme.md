# CSE ASSIGNMENT 2


## Aim

Implement a shell in C.

## How to run

```
make
./shell
```

## Source Files 

### main.c

Sets home directory and creates a copy of itself which runs the shell

### prompt.c

Displays the shell prompt(<user_name@system_name:curr_dir_path>)  and checks if a back-ground process has been completed  

### interpreter.c

Interprets the input command, it parses the line entered into different commands on the basis of semicolons and then into different arguments based on spaces and calls appropriate functions; also changes input/output stream as required to implement piping and redirection; attaches signal handlers for SIGINT, SIGSTOP, SIGTTOU; creats and maintains a job list for background process and manges the current foreground process group as well

### cd.c

Implements cd command

### ls.c

Implements ls command
Accepted flags: -l, -a 

### pinfo.c

Implements pinfo command in the shell
syntax : pinfo <num(pid)>

### pwd.c

Implements pwd

### echo.c

Implements echo 

### history.c

Implemetns history and allows 'up' to use its database to get the last kth command
maximum commands stored = 20, maximum commands displayed = 10
syntax : history <num(between 1 to 10)>

### setenv.c

Implements setenv command
usage : setenv var [val]

### unsetenv.c

Implements unsetenv command
usage : unsetenc var1 var2 .. varN

### jobs.c

Implements jobs command which displays all background process

### kjob.c

Implements kjob command
usage : kjob [job number] [signal number] 

### fg.c

Implements fg command to bring a running / stopped background process to foreground(and runs it using SIGCONT in case it was stopped)

### bg.c

Implements bg command to continue a stopped background process

### overkill.c

Implements overkill commands which kills all background process

### quit.c

Implemnts the quit command which serves as the only way to exit the shell; all other signals (ctrlC, ctrlZ, ctrl/, SIGTERM) are discarded

### cronjob.c

Implements cronjob commands
usage : cronjob -c [command] -t [time periiod] -p [total time] 

### shell.h

A header file containing global functions and variables
