#define shell_H

extern char HOME_DIR[];
extern int flag_exit;

void display_prompt();
void exec_pwd();
void exec_cd(char *dir_path);
void exec_echo(char **arg);
void exec_ls(char **arg);
void exec_pinfo(char *arg);
void exec_history(char *arg);
void insert_history(char *arg);
void save_history();
void retrieve_history();
void interprete_commands();
void bg_handler();