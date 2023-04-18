#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "include.h"

#define shell_BUFSIZE 1024
#define EXIT_FAILURE_SHELL 0
#define EXIT_SUCCESS_SHELL 1
#define EXIT_ERROR_SHELL 2

const char TILD='~';
char HOME_HOME[255];

// prototypes des commandes shell
int wrapper_shell_cd(char** args, int n);
int wrapper_shell_ls(char** args, int n);
int wrapper_shell_pwd(char** args, int n);
int wrapper_shell_date(char** args, int n);
int wrapper_shell_echo(char** args, int n);
int wrapper_shell_history(char** args, int n);
int wrapper_shell_head(char** args, int n);
int wrapper_shell_cat(char** args, int n);
int wrapper_shell_mkdir(char** args, int n);
int wrapper_shell_rmdir(char** args, int n);
int wrapper_shell_help(char** args, int n);
int wrapper_shell_clear(char** args, int n);
int wrapper_shell_PS1(char** args, int n);
int wrapper_shell_dump(char** args, int n);
int wrapper_shell_search_dump(char** args, int n);
int wrapper_shell_adduser(char** args, int n);
int wrapper_shell_exit(char** args, int n);


struct builtin
{
    char *name;
    int (*func)(char **, int);
};

char *builtin_str[] = { // tableau de commandes internes
        "cd",
        "ls",
        "pwd",
        "date",
        "echo",
        "history",
        "head",
        "cat",
        "mkdir",
        "rmdir",
        "help",
        "clear",
        "PS1",
        "dump",
        "searchdump",
        "adduser",
        "exit"
};

char *builtin_str_alias[] = { // tableau des alias
        "cdir",
        "ll",
        "path",
        "time",
        "say",
        "historique",
        "tete",
        "contenu",
        "mk",
        "rm",
        "aide",
        "cls",
        "prompt",
        "extraction",
        "chercher",
        "useradd",
        "quitter"
};

int (*builtin_func[])(char **, int) = { // tableau de pointeurs de fonctions
    &wrapper_shell_cd,
    &wrapper_shell_ls,
    &wrapper_shell_pwd,
    &wrapper_shell_date,
    &wrapper_shell_echo,
    &wrapper_shell_history,
    &wrapper_shell_head,
    &wrapper_shell_cat,
    &wrapper_shell_mkdir,
    &wrapper_shell_rmdir,
    &wrapper_shell_help,
    &wrapper_shell_clear,
    &wrapper_shell_PS1,
    &wrapper_shell_dump,
    &wrapper_shell_search_dump,
    &wrapper_shell_adduser,
    &wrapper_shell_exit
};

int shell_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

// ----------------- Fonctions de lancement du shell -----------------

int shell_launch(char** tokens, int n)
{
    if(fork() == 0)
    {
        char* args[n+1];
        for (int i = 0; i < n; ++i)
        {
            args[i] = tokens[i]; // on copie les arguments
        }
        args[n] = NULL;
        execvp(args[0], args); // on lance la commande
        printf("\n");
        return EXIT_SUCCESS_SHELL;
    }
    else
    {
        wait(NULL);
    }
    return EXIT_SUCCESS_SHELL;
}

int shell_execute(char** tokens, int n)
{
    int i;
    if (tokens[0] == NULL)
    {
        return 1;
    }
    for (i = 0; i < shell_num_builtins(); i++) // on parcourt les commandes internes
    {
        if ((my_strcmp(tokens[0], builtin_str[i]) == 0 || my_strcmp(tokens[0], builtin_str_alias[i]) == 0)&&(my_strlen(tokens[0]) ==
                my_strlen(builtin_str[i]) || my_strlen(tokens[0]) == my_strlen(builtin_str_alias[i]))) // si la commande est interne
        {
            return (*builtin_func[i])(tokens, n); // on l'exécute
        }
    }
    return 0;
}

void shell_loop()
{
    int i = 0;
    char* line;
    char* args[255];
    int status;

    char HOME_PATH[255];
    char* home = getenv("HOME");
    sprintf(HOME_PATH, "%s/.minishell", home);

    if (access(home, F_OK) != -1)
    {
        if (access(HOME_PATH, F_OK) != -1)
        {
            printf(" ");
        }
        else
        {
            mkdir(HOME_PATH, 0700);
        }
    }

    shell_init_shell(); // initialise la variable HOME_SHELL
    rc_check();
    history_check();
    shell_encryption();

    while(1)
    {
        prompt();
        line = shell_read_line();
        int n = 0;
        n = shell_split_line(line, args, " "); // n = nombre d'arguments
        signal(SIGINT, handler); // handler Ctrl+C
        signal(SIGQUIT, handlersigquit); // handler Ctrl+\ //
        signal(SIGSEGV, handlersigsegv); // handler faute de segmentation
        signal(SIGHUP, handlersighup); // handler fermeture du terminal de manière graphique
        status = shell_execute(args, n); // status = 0 si la commande n'est pas interne
        if (status == 0)
        {
            shell_launch(args,n);
        }
    }
}

int main()
{
    shell_clear();
    ascii_art();
    printf("   [exit] pour quitter.\n\n");
    shell_loop(); // boucle principale
    return EXIT_SUCCESS_SHELL;
}