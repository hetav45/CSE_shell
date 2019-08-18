#define shell_H

extern char * HOME_DIR;

void display_prompt();
void exec_pwd();
void exec_cd(char * dir_path);
void exec_echo(char * str);
void interprete_commands();