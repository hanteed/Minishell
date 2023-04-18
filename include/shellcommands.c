#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
#include "../include.h"

// ----------------- Fonctions du shell -----------------

typedef struct
{
    char *username;
    char *password;
    char *group;
    char *home;
} User;

char rc_path[255];
char passwd_path[255];
char profile_path[255];
char history_path[255];
char encrypted_path[255];
char HOME_SHELL[255];

const char* get_executable_path()
{
    static char HOME_HOME[255];
    const char* executable = getcwd(HOME_HOME, sizeof(HOME_HOME)); // on récupère le chemin du shell
    return executable;
}

void set_shell_path()
{
    const char* executable_path = getcwd(HOME_SHELL, sizeof(HOME_SHELL));
    sprintf(HOME_SHELL, "%s/%s", executable_path, shell_name); // on concatène le chemin du home avec le nom du dossier du shell
}

void set_passwd_path()
{
    const char* executable_path = getenv("HOME"); // on récupère le chemin du home
    sprintf(passwd_path, "%s/.minishell/.minishellpasswd", executable_path); // on concatène le chemin du shell avec le nom du fichier d'historique
}

void set_profile_path()
{
    const char* executable_path = getenv("HOME"); // on récupère le chemin du home
    sprintf(profile_path, "%s/.minishell/.minishellprofile", executable_path); // on concatène le chemin du shell avec le nom du fichier de profil
}

void set_history_path()
{
    const char* executable_path = getenv("HOME"); // on récupère le chemin du home
    sprintf(history_path, "%s/.minishell/history", executable_path); // on concatène le chemin du shell avec le nom du fichier d'historique
}

void set_rc_path()
{
    const char* executable_path = getenv("HOME"); // on récupère le chemin du home
    sprintf(rc_path, "%s/.minishell/.minishellrc", executable_path); // on concatène le chemin du shell avec le nom du fichier de configuration
}

void set_encrypted_path()
{
    const char* executable_path = getenv("HOME"); // on récupère le chemin du home
    sprintf(encrypted_path, "%s/.minishell/history.encrypted", executable_path); // on concatène le chemin du shell avec le nom du fichier d'historique
}

void ascii_art()
{
    printf("\n             _       _     _          _ _ \n            (_)     (_)   | |        | | |\n   _ __ ___  _ _ __  _ ___| |__   ___| | |\n  | '_ ` _ \\| | '_ \\| / __| '_ \\ / _ \\ | |\n  | | | | | | | | | | \\__ \\ | | |  __/ | |\n  |_| |_| |_|_|_| |_|_|___/_| |_|\\___|_|_|\n\n");
}

int nbtoken(char* src, char* delim)
{
    int i = 0;
    int n = 0;
    while(src[i] != '\0')
    {
        if (src[i] == delim[0])
        {
            n++;
        }
        i++;
    }
    return n;
}

FILE * open_file(char * nomFic, char * mode)
{
    FILE * aLire ;
    char* pwd = getenv("PWD");
    aLire = fopen(nomFic, mode); // Ouverture du fichier en lecture

    if (aLire != NULL)
    {
        printf("\nNom du fichier : %s\n", nomFic); // Affichage du nom du fichier
    }
    else
    {
        printf("L'ouverture du fichier à échoué"); // Affichage d'un message d'erreur
        printf("%s",pwd); // Affichage du chemin du fichier
    }
    return aLire;
}

int searchInDumpFile(FILE *infile, char *searchStr)
{
    // rechercher searchStr dans infile et afficher la ligne
    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), infile))
    {
        if (my_strstr(line, searchStr ) != NULL)
        {
            fputs ( line, stdout );
            i++;
        }
    }
    fclose(infile);
    if(i == 0)
    {
        printf("Aucune occurence trouvée.\n");
    }
    else
    {
        printf("%d occurence(s) trouvée(s).\n", i);
    }
    return 0;
}

int shell_dump(char *infilename, int start, int stop, int nbOctet, char *format)
{
    char ch;
    char dot = '.';
    char buffer[stop];
    int i = 0;
    int j = 0;
    nbOctet--;
    int k = nbOctet;
    int l = 0;
    printf("Octet de début : %d\n", start);
    printf("Octet de fin : %d\n", stop);
    printf("Octets par ligne : %d\n", nbOctet);
    printf("Mode d'ouverture : %s\n", format);
    FILE *infile = fopen(infilename, "r");
    FILE *outfile = fopen("dump.txt", "w");
    while (!feof(infile))
    {
        ch = getc(infile);
        if (j >= start && j <= stop)
        {
            if (my_strcmp(format, "x") == 0)
            {
                if(k <= 0 || l == 0)
                {
                    l++;
                    fprintf(outfile,"\n%d. ",l);
                    k = nbOctet;
                }
                else
                {
                    k--;
                }
                fprintf(outfile, "%02x ", ch);
            }
            else if (my_strcmp(format, "d") == 0)
            {
                if(k <= 0 || l == 0)
                {
                    l++;
                    fprintf(outfile,"\n%d. ",l);
                    k = nbOctet;
                }
                else
                {
                    k--;
                }
                fprintf(outfile, "%03d ", ch);
            }
            else if (my_strcmp(format, "a") == 0)
            {
                if (ch <= 32)
                {
                    if(k <= 0 || l == 0)
                    {
                        l++;
                        fprintf(outfile,"\n%d. ",l);
                        k = nbOctet;
                    }
                    else
                    {
                        k--;
                    }
                    fprintf(outfile, "%c", dot);
                }
                else
                {
                    if(k <= 0 || l == 0)
                    {
                        l++;
                        fprintf(outfile,"\n%d. ",l);
                        k = nbOctet;
                    }
                    else
                    {
                        k--;
                    }
                    fprintf(outfile, "%c", ch);
                }
            }
            else if (my_strcmp(format, "o") == 0)
            {
                if(k <= 0 || l == 0)
                {
                    l++;
                    fprintf(outfile,"\n%d. ",l);
                    k = nbOctet;
                }
                else
                {
                    k--;
                }
                fprintf(outfile, "%03o ", ch);
            }
            else
            {
                printf("Format non valide\n");
                return EXIT_FAILURE;
            }
            i++;
        }
        j++;
    }
    printf("\nDump enregistré dans dump.txt\n");
    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}

int shell_search_dump(char *src, char *todump)
{
    if(access(src, F_OK) == -1) // Vérification de l'existence du fichier
    {
        printf("Le fichier %s n'existe pas.\n\n", src);
        return EXIT_FAILURE;
    }
    FILE * aLire = open_file(src,"r");
    searchInDumpFile(aLire, todump);
    return EXIT_SUCCESS;
}

int calcul_indentation()
{
    DIR *dp;
    char *dir=".";
    int indentationspace = 5;
    struct dirent *pe;
    dp = opendir(dir);
    if(dp != NULL)
    {
        while ((pe = readdir(dp)))
        {
            if(indentationspace < my_strlen(pe->d_name))
            {
                indentationspace = my_strlen(pe->d_name);
            }
        }
    }
    return indentationspace;
}

int shell_ls(char *option, char *dir, int nbargs)
{
    DIR *dp;
    int indentation = 0;
    int indentationspace = calcul_indentation()+1;
    int indentationspacecpy = indentationspace;
    struct dirent *pe; // pe = pointeur entree
    dp = opendir(dir);
    if (dp != NULL)
    {
        while ((pe = readdir(dp)))
        {
            if (my_strcmp(option, "-d") == 0 || my_strcmp(option, "--directory") == 0)
            {
                printf("\033[0;34m");
                if (pe->d_type == DT_DIR)
                {
                    if(indentation == 3)
                    {
                        printf("• %s\n", pe->d_name);
                        indentation = 0;
                    }
                    else
                    {
                        indentationspace = indentationspace - my_strlen(pe->d_name);
                        printf("• %s ", pe->d_name);
                        for(int i = 0; i < indentationspace; i++)
                        {
                            printf(" ");
                        }
                        indentationspace = indentationspacecpy;
                        indentation++;
                    }
                }
                printf("\033[0m");
            }
            else if (my_strcmp(option, "-f") == 0 || my_strcmp(option, "--file") == 0)
            {
                printf("\033[0;36m");
                if (pe->d_type == DT_REG)
                {
                    if(indentation == 3)
                    {
                        printf("• %s\n", pe->d_name);
                        indentation = 0;
                    }
                    else
                    {
                        indentationspace = indentationspace - my_strlen(pe->d_name);
                        printf("• %s ", pe->d_name);
                        for(int i = 0; i < indentationspace; i++)
                        {
                            printf(" ");
                        }
                        indentationspace = indentationspacecpy;
                        indentation++;
                    }
                }
                printf("\033[0m");
            }
            else
            {
                if(indentation == 3)
                {
                    if (pe->d_type == DT_REG)
                        printf("\033[0;36m");
                    else if (pe->d_type == DT_DIR)
                        printf("\033[0;34m");
                    printf("• %s\n", pe->d_name);
                    indentation = 0;
                }
                else
                {
                    if (pe->d_type == DT_REG)
                        printf("\033[0;36m");
                    else if (pe->d_type == DT_DIR)
                        printf("\033[0;34m");
                    indentationspace = indentationspace - my_strlen(pe->d_name);
                    printf("• %s ", pe->d_name);
                    for(int i = 0; i < indentationspace; i++)
                    {
                        printf(" ");
                    }
                    indentationspace = indentationspacecpy;
                    indentation++;
                }
                printf("\033[0m");
            }
        }
        printf("\n");
    }
    else
    {
        printf("Le dossier n'a pas pu être ouvert.\n");
    }
    return EXIT_SUCCESS_SHELL;
}

void shell_pwd()
{
    char path[1024];
    if (getcwd(path, sizeof(path)) != NULL) // getcwd renvoie NULL en cas d'erreur
    {
        printf("Path : %s\n", path);
    }
}

void rc_check()
{
    static int rc_path_set = 0;
    if (!rc_path_set)
    {
        set_rc_path();
        rc_path_set = 1;
    }

    FILE *rc = fopen(rc_path, "r");
    if (rc == NULL)
    {
        rc = fopen(rc_path, "w");
        if (rc == NULL)
        {
            perror("Erreur d'ouverture du fichier"); // affiche le message d'erreur
            exit(EXIT_FAILURE);
        }
        fprintf(rc, "PS1=\"$(pwd) >\"");
        fclose(rc);
    }
    else
    {
        fclose(rc);
    }
}

void history_check()
{
    static int history_path_set = 0;
    if (!history_path_set)
    {
        set_history_path();
        history_path_set = 1;
    }
    static int encrypted_path_set = 0;
    if (!encrypted_path_set)
    {
        set_encrypted_path();
        encrypted_path_set = 1;
    }
    if (access(history_path, F_OK) == -1 && access(encrypted_path, F_OK) == -1)
    {
        FILE *history = fopen(history_path, "w");
        if (history == NULL)
        {
            perror("Erreur d'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
        fclose(history);
    }
}

void add_history(char* input)
{
    static int history_path_set = 0; // permet de ne pas appeler la fonction set_history_path() à chaque fois
    if (!history_path_set)
    {
        set_history_path(); // définit le chemin du fichier history
        history_path_set = 1;
    }
    FILE *history = fopen(history_path, "a");
    if (history == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    if(input[0] != '\0' && input[0] != ' ' && input[0] != '\t' && input[0] != '\n')
    {
        fseek(history, 0, SEEK_END);  // placer le curseur de fichier à la fin
        fprintf(history, "%s", input); // ajoute la commande à la fin du fichier
    }
    fclose(history);
}

void shell_history()
{
    static int history_path_set = 0; // permet de ne pas appeler la fonction set_history_path() à chaque fois
    if (!history_path_set)
    {
        set_history_path(); // définit le chemin du fichier history
        history_path_set = 1;
    }
    FILE *history = fopen(history_path, "r"); // ouvre le fichier en lecture seule
    if (history == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    char c;
    while ((c = fgetc(history)) != EOF) // lit le fichier caractère par caractère
    {
        printf("%c", c); // affiche le caractère
    }
}

char* shell_read_line()
{
    char path[1024];
    int history_validation = 1;
    int MAX_INPUT_LENGTH = 1024;
    char* input = malloc(MAX_INPUT_LENGTH);
    if (input == NULL)
    {
        printf("Erreur d'allocation de mémoire");
        fflush(stdin);
        exit(EXIT_FAILURE);
    }
    fgets(input, MAX_INPUT_LENGTH, stdin); // lit la ligne
    if (input[0] == '\n') // si l'utilisateur n'a rien entré
    {
        history_validation = 0;
        sprintf(input, "cd %s", getcwd(path, sizeof(path)));
    }
    if (ferror(stdin))
    {
        printf("Erreur de lecture");
        fflush(stdin);
        exit(EXIT_FAILURE);
    }
    if (history_validation)
    {
        add_history(input); // ajoute la commande à l'historique
    }
    if (input[my_strlen(input) - 1] == '\n') // supprime le retour à la ligne
    {
        input[my_strlen(input) - 1] = '\0';
    }
    fflush(stdin);
    return input;
}

int shell_split_line(char* src, char* tokens[], char* delim)
{
    int i = 0;
    int n = 0;
    int len = my_strlen(src);
    n = nbtoken(src, delim)+1;
    while (src != NULL && i < len && n > 0)
    {
        tokens[i] = my_strtok(src, delim); // récupère le premier token
        src += my_strlen(tokens[i]) + 1; // décale le pointeur de src
        n--;
        i++;
    }
    return i;
}

void handler()
{

}

void handlersigquit()
{
    printf("\nÊtes vous sûr de vouloir quitter ? (y/n) ");
    char c = getchar();
    if (c == 'y')
    {
        if(check_encrypted() == 0)
        {
            encrypt(encrypted_path,history_path, HASH_KEY_FILE);
            remove(history_path);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("\n");
    }
}

void handlersigsegv()
{
    if(check_encrypted() == 0)
    {
        encrypt(encrypted_path,history_path, HASH_KEY_FILE);
        remove(history_path);
    }
    exit(EXIT_FAILURE);
}

void handlersighup()
{
    if(check_encrypted() == 0)
    {
        encrypt(encrypted_path,history_path, HASH_KEY_FILE);
        remove(history_path);
    }
    exit(EXIT_FAILURE);
}

int shell_cd(char* path)
{
    chdir(path); // change le répertoire courant
}

int shell_date()
{
    time_t t;
    time(&t); // récupère la date actuelle
    printf("Date actuelle : %s", ctime(&t)); // ctime() ajoute un retour à la ligne
}

void shell_echo(char* src)
{
    printf("%s \n", src);
}

void shell_head(char* src, int nb)
{
    FILE * aLire = open_file(src,"r"); // ouverture du fichier en lecture
    char* line = malloc(255);
    int i = 0;

    while (fgets(line, 255, aLire) != NULL && i < nb) // lecture ligne par ligne
    {
        printf("%s", line); // affichage de la ligne
        i++;
    }
    free(line);
}

void shell_mkdir(char* src)
{
    mkdir(src, 0700); // 0700 = tous les droits pour le propriétaire
    if (errno == EEXIST)
    {
        printf("Le dossier existe déjà.\n");
    }
    else
    {
        printf("Le dossier %s a été créé.\n",src);
    }
}

void shell_rmdir(char* src)
{
    printf("Êtes-vous sûr de vouloir supprimer %s ? (y/n) ", src);
    char c = getchar();
    while (getchar() != '\n'); // vide le buffer d'entrée jusqu'à la fin de ligne
    if (c == 'y')
    {
        remove(src);
        printf("%s supprimé\n",src);
    }
    else
    {
        printf("Suppression annulée\n");
    }
}

void shell_help()
{
    printf(" Aide du minishell :\n\n cd [dossier]\n ls [-all -d -f] [répertoire]\n pwd\n");
    printf(" date\n history\n echo [texte]\n head [-x] [fichier]\n");
    printf(" cat [fichier]\n mkdir [nom répertoire]\n rmdir [nom répertoire]\n help\n clear\n PS1 [prompt]\n dump dump [fichier] [début] [fin] [nboctets] [caractères] \n searchdump [fichier] [caractères]\n");
    printf(" adduser \n exit\n\n");
}

void shell_clear()
{
    printf("\e[1;1H\e[2J");
}

void shell_PS1(char* src, char* src2)
{
    static int rc_path_set = 0;
    if (!rc_path_set)
    {
        set_rc_path();
        rc_path_set = 1;
    }

    FILE *rc = fopen(rc_path, "w");
    if (rc == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    if (my_strcmp(src2,"nodisplay") == 0)
    {
        fprintf(rc, "PS1=\"%s\"", src);
    }
    else
    {
        fprintf(rc, "PS1=\"%s %s\"", src, src2);
    }
    fclose(rc);
}

void shell_cat(char* src)
{
    FILE * aLire = open_file(src,"r");
    char* line = malloc(255);
    while (fgets(line, 255, aLire) != NULL)
    {
        printf("%s", line);
    }
    printf("\n");
    free(line);
}

int check_encrypted()
{
    static int history_path_set = 0;
    if (!history_path_set)
    {
        set_history_path();
        history_path_set = 1;
    }
    if (access(history_path, F_OK) == -1) // le fichier est chiffré
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int shell_encryption()
{
    int i = 0;
    int tokenlen;
    char line[256];
    char PASSWORD[256];
    char* hashpassword;
    char user_input[256];
    char* name = getenv("USER");

    // unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    // crypto_secretstream_xchacha20poly1305_keygen(key); // key : random HASH_KEY_FILE : prédéfini

    static int history_path_set = 0; // permet de ne pas appeler la fonction set_history_path() à chaque fois
    if (!history_path_set)
    {
        set_history_path(); // définit le chemin du fichier history
        history_path_set = 1;
    }
    static int passwd_path_set = 0; // permet de ne pas appeler la fonction set_history_path() à chaque fois
    if (!passwd_path_set)
    {
        set_passwd_path(); // définit le chemin du fichier history
        passwd_path_set = 1;
    }
    static int encrypted_path_set = 0;
    if (!encrypted_path_set)
    {
        set_encrypted_path();
        encrypted_path_set = 1;
    }

    if (sodium_init() < 0)
    {
        printf("Erreur lors de l'initialisation de sodium\n");
        return 1;
    }

    if (access(passwd_path, F_OK) == -1)
    {
        FILE *fp = fopen(passwd_path, "a");
        printf("Fichier .minishellpasswd non créé, lancement de la configuration.\n Entrez votre mot de passe de chiffrement : ");
        fgets(PASSWORD, 256, stdin);
        PASSWORD[my_strlen(PASSWORD) - 1] = '\0';
        hashpassword = hash(PASSWORD, HASH_KEY);
        printf(" HASH : %s\nVotre mot de passe a été enregistré dans le fichier .minishellpasswd.\n\n", hashpassword);
        fprintf(fp, "%s:%s\n", name, hashpassword);
        fclose(fp);
    }
    else
    {
        printf(" Entrez votre mot de passe : ");
        fgets(user_input, 256, stdin);
        user_input[my_strlen(user_input) - 1] = '\0';

        // rechercher la ligne débutant par le nom de l'utilisateur
        FILE *fp = fopen(passwd_path, "r");
        while (fgets(line, sizeof(line), fp))
        {
            if (my_strcmp(my_strtok(line,":"), name) == 0)
            {
                // printf("line : %s\n", line); // Debug
                tokenlen = my_strlen(my_strtok(line,":"))+1;
                while (line[tokenlen] != '\n')
                {
                    PASSWORD[i] = line[tokenlen];
                    i++;
                    tokenlen++;
                }
            }
        }
        fclose(fp);
        hashpassword = hash(user_input, HASH_KEY);
        if (my_strcmp(hashpassword, PASSWORD) == 0)
        {
            printf("\n Mot de passe correct, déverrouillage du shell");
            fflush(stdout);
            for (i = 0; i < 3; i++) {
                usleep(205000); // on attend 205ms*3 = 615ms
                printf(".");
                fflush(stdout);
            }
            printf("\n");
            if(check_encrypted() == 1)
            {
                decrypt(history_path, encrypted_path, HASH_KEY_FILE);
                remove(encrypted_path);
            }
        }
        else
        {
            printf("  Mot de passe incorrect, veuillez réessayer\n ");
            if(check_encrypted() == 0)
            {
                encrypt(encrypted_path,history_path, HASH_KEY_FILE);
                remove(history_path);
            }
            shell_encryption();
        }
    }
    return 0;
}

int shell_init_shell()
{
    static int shell_path_set = 0;
    if (!shell_path_set)
    {
        set_shell_path();
        shell_path_set = 1;
    }
    return 0;
}

int shell_adduser()
{
    User user;
    char buffer[255];
    int longueur;
    int i;

    static int profile_path_set = 0;
    if (!profile_path_set)
    {
        set_profile_path();
        profile_path_set = 1;
    }
    printf("\nVeuillez entrer le nom de l'utilisateur :\n>");
    scanf("%s", buffer);
    getchar();
    longueur = my_strlen(buffer);
    user.username = malloc((longueur + 1) * sizeof(char));
    user.username = my_strdupin(buffer);

    for (i = 0; i < longueur; i++)
    {
        buffer[i] = '\0';
    }

    longueur = my_strlen(user.username) + my_strlen("/home/");
    user.home = malloc((longueur + 1) * sizeof(char));
    sprintf(buffer, "/home/%s", user.username);
    user.home = my_strdupin(buffer);
    printf("\nVeuillez entrer le mot de passe de %s\n",user.username);
    sprintf(buffer, "/bin/%s", shell_name);
    char *command[]={"sudo","useradd",user.username,"--home",user.home,"--shell",buffer,"--create-home"};
    shell_launch(command,8);

    char *command2[]={"sudo","passwd",user.username};
    shell_launch(command2,3);

    FILE * fp = fopen(profile_path, "a");
    fprintf(fp, "%s:%s:%s\n",user.username, user.home, HOME_SHELL);
    fclose(fp);

    printf("\nUtilisateur %s ajouté avec succès\n",user.username);

    free(user.username);
    free(user.home);
    return 0;
}

int shell_exit()
{
    printf("Fin du shell\n");
    if(check_encrypted() == 0)
    {
        encrypt(encrypted_path,history_path, HASH_KEY_FILE);
        remove(history_path);
    }
    kill(0, SIGTERM);
    while (wait(NULL) > 0){};
    exit(EXIT_SUCCESS);
}

void prompt()
{
    int j,k = 0;
    static int rc_path_set = 0;
    if (!rc_path_set)
    {
        set_rc_path();
        rc_path_set = 1;
    }

    FILE *rc = fopen(rc_path, "r");
    char* ps1[255];
    char* prompt[255];
    char* line = malloc(255);
    if (rc == NULL)
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, 255, rc) != NULL)
    {
        shell_split_line(line, ps1, "=");
    }
    shell_split_line(ps1[1], prompt, "\""); // "\"" = guillemets

    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    j= my_strlen(cwd);
    k= my_strlen(HOME_SHELL)-my_strlen(shell_name)-1;

    if(my_strcmp(cwd, HOME_SHELL) == 0 && j==k)
    {
        if(my_strncmp(prompt[1], "$(pwd)", 6) == 0)
        {
            if(prompt[2] != NULL && prompt[2][0] != '\0' && prompt[2][0] != '\n' && prompt[2][0] != '\t')
            {
                printf("~/%s", prompt[2]);
                printf(" ");
            }
            else
            {
                printf("~/>");
                printf(" ");
            }
        }
        else
        {
            printf("~/%s", prompt[1]);
            printf(" ");
        }
    }
    else
    {
        if(my_strncmp(prompt[1], "$(pwd)", 6) == 0)
        {
            prompt[1]+=6;
            printf("%s%s", cwd, prompt[1]);
            printf(" ");
        }
        else
        {
            printf("%s", prompt[1]);
            printf(" ");
        }
    }
    fflush(stdout);
    fclose(rc);
}
