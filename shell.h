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
void exec_kjob(int jobid, int signal);
void exec_fg(int jobid);
void exec_bg(int jobid);
void exec_cronjob(char **argv);
void exec_overkill();
void insert_history(char *arg);
void save_history();
void retrieve_history();
void get_history(int n, char *str);
void interprete_commands(char *str);
void execute_commands(char **argv);
void parse_commands();
void bg_handler();
void insert_node(int pid, char *name, int status);
void delete_node(struct child_list *i);