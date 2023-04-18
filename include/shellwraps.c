#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include.h"

// ----------------- Wraps des fonctions -----------------

int wrapper_shell_cd(char** args, int n)
{
    char* home = getenv("HOME");
    if(n == 1)
    {
        shell_cd(home);
    }
    else
    {
        shell_cd(args[1]);
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_ls(char** args, int n)
{
    char* home = getenv("HOME");
    if(n == 0)
    {
        printf("Erreur: ls n'a pas été appelé correctement.\n");
    }
    if(n == 1)
    {
        shell_ls("-all", ".", n);
    }
    if(n == 2)
    {
        if(args[1][0] == ' ' || args[1][0] == '.')
        {
            shell_ls("-all", ".", n);
        }
        else if(args[1][0] == '/')
        {
            shell_ls("-all", args[1], n);
        }
        else if(args[1][0] == '~')
        {
            shell_ls("-all", home, n);
        }
        else if(args[1][0] == '-')
        {
            if(my_strlen(args[1]) == 1)
            {
                printf("Erreur d'usage.\n");
            }
            else
            {
                shell_ls(args[1], ".", n);
            }
        }
    }
    if(n == 3)
    {
        if(args[2][0] == ' ')
        {
            shell_ls(args[1], ".", n);
        }
        else
        {
            shell_ls(args[1], args[2], n);
        }
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_exit(char** args, int n)
{
    shell_exit();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_pwd(char** args, int n)
{
    shell_pwd();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_date(char** args, int n)
{
    shell_date();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_echo(char** args, int n)
{
    if(n == 1)
    {
        printf("Usage : echo \"texte\".\n");
    }
    else
    {
        shell_echo(args[1]);
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_history(char** args, int n)
{
    shell_history();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_head(char** args, int n)
{
    if(n == 1 || n == 2 || args[1][1] == ' ' || args[1][1] == '\0')
    {
        printf("Usage : head -x fichier.\n");
    }
    else
    {
        shell_head(args[2], atoi(&args[1][1]));
    }
    return 1;
}

int wrapper_shell_cat(char** args, int n)
{
    if(n == 1 || args[1][0] == ' ' || args[1][0] == '\0')
    {
        printf("Usage : cat fichier.\n");
    }
    else if(access(args[1], F_OK) == -1)
    {
        printf("Erreur : le fichier n'existe pas.\n");
    }
    else
    {
        shell_cat(args[1]);
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_mkdir(char** args, int n)
{
    if(n == 1 || args[1][0] == ' ' || args[1][0] == '\0')
    {
        printf("Usage : mkdir nom_dossier.\n");
    }
    else if(n == 2)
    {
        shell_mkdir(args[1]);
    }
    else
    {
        printf("Usage : mkdir nom_dossier.\n");
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_rmdir(char** args, int n)
{
    if(n == 1 || args[1][0] == ' ' || args[1][0] == '\0')
    {
        printf("Usage : rmdir nom_dossier.\n");
    }
    else if(n == 2)
    {
        shell_rmdir(args[1]);
    }
    else
    {
        printf("Usage : rmdir nom_dossier.\n");
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_help(char** args, int n)
{
    shell_help();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_clear(char** args, int n)
{
    shell_clear();
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_PS1(char** args, int n)
{
    if(n == 1 || args[1][0] == ' ' || args[1][0] == '\0')
    {
        printf("Usage : PS1 prompt.\n");
    }
    if(n == 2 || args[2][0] == ' ' || args[2][0] == '\0' || args[2][0] == '\n' || args[2][0] == '\t')
    {
        shell_PS1(args[1],"nodisplay");
        printf("Prompt sauvegardé.\n");

    }
    if(n == 3)
    {
        shell_PS1(args[1],args[2]);
        printf("Prompt sauvegardé.\n");
    }
    else
    {
        printf("Usage : PS1 prompt.\n");
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_dump(char** args, int n)
{
    if(n != 6)
    {
        printf("Usage : dump fichier début fin nboctets caractères.\nExemple : dump fichier 0 100 10 x\n");
    }
    else
    {
        shell_dump(args[1],atoi(args[2]),atoi(args[3]),atoi(args[4]),args[5]);
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_search_dump(char** args, int n)
{
    if (n == 3)
    {
        shell_search_dump(args[1],args[2]);
    }
    else
    {
        printf("Usage : searchdump fichier caractères\n");
    }
    return EXIT_SUCCESS_SHELL;
}

int wrapper_shell_adduser(char** args, int n)
{
    if(n == 1)
    {
        shell_adduser();
    }
    else
    {
        printf("Usage : adduser\n");
    }
    return EXIT_SUCCESS_SHELL;
}