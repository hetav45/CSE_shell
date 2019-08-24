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

Interpret the input command, it parses the line entered into different commands on the basis of semicolons and then into different arguments based on spaces and calls appropriate functions

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

Implemetns history
maximum commands stored = 20, maximum commands displayed = 10
syntax : history <num(between 1 to 10)>

### shell.h

A header file containing global functions and variables
