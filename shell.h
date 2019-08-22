#define shell_H

extern char HOME_DIR[];

void display_prompt();
void exec_pwd();
void exec_cd(char * dir_path);
void exec_echo(char * str);
void exec_ls(char * arg);
void exec_pinfo(char * arg);
void interprete_commands();