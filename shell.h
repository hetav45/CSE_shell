#define shell_H

extern char HOME_DIR[];
struct child_list
{
    int pid;
    int status; // 0-stopped , 1-running
    char name[64];
    struct child_list *next;
    struct child_list *pre;
};

extern struct child_list fg; 
extern struct child_list *root;

void display_prompt();
void exec_pwd();
void exec_cd(char *dir_path);
void exec_echo(char **arg);
void exec_ls(char **arg);
void exec_pinfo(char *arg);
void exec_history(char *arg);
void exec_setenv(char **arg);
void exec_unsetenv(char **arg);
void exec_jobs(struct child_list *root);
void exec_kjob(int pid, int signal);
void exec_fg(int pid);
void exec_bg(int pid);
void exec_overkill();
void insert_history(char *arg);
void save_history();
void retrieve_history();
void interprete_commands(char *str);
void parse_commands();
void bg_handler();
void delete_node(struct child_list *i);