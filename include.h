#ifndef INCLUDE_H
#define INCLUDE_H
#define EXIT_SUCCESS_SHELL 1
#include <stdbool.h>
#include <sodium.h>

// defines pour le shell
#define shell_name "minishell"

// defines pour le chiffrement
#define CHUNK_SIZE 4096
#define SHORT_DATA_LEN 15
#define HASH_KEY "4zzdPhRhRJ95tccCqW4sWg$hLUkCarZtZMVJrixZE0yBaDDkwAv2WOpunE21TsBPOE"
#define HASH_KEY_FILE "uzg08708ckjHZVFKDNZLGUHREDJUFH$ZFkgvhze$92jfoqhfpfuhzeifj$fgE"

int my_strlen(const char *str); //Retourne la longueur d'une chaine de caractères
unsigned int my_strnlen(const char *str, unsigned int max_len); //Retourne la longueur d'une chaine de caractères jusqu'à un nombre de caractères spécifié
char * my_strcpy(char * dest, const char * src); //Copie une chaine de caractères dans une autre chaine
char * my_mstrcpy(char * dest, const char * src); //Copie une chaine de caractères dans une autre chaine
char * my_mstrncpy(const char * src, int count); //Copie une chaine de caractères à partir d'un n caractère
char * my_strcat(char * a, char * b); //Concatène une chaine de caractères a et une chaine de caractères b
char * my_strdup(char *src, int nb_de_fois); //Duplique une chaine de caractères n fois
char * my_strdupin(const char * src); //Duplique une chaine de caractères dans une autre
char * my_strdup2(char * src, char * str, int nb_de_fois); //Duplique une chaine de caractères n fois dans une autre chaine de caractères
char * my_strtrim(char * src); //Retire les espaces d'une chaine de caractères
char * my_strtrimCar(char * src, char * asuppr); //Retire le caractère spécifié de la chaine de caractères
int my_strcmp(char * str1, char * str2); //Compare deux chaines de caractères et retourne 0 si elles sont égales
int my_strncmp(const char *s1, const char *s2, unsigned int n); //Compare deux chaines de caractères et retourne 0 si elles sont égales
void * my_strMin2Maj(char * str); //Converti une chaine de catactères en majuscules
void * my_strMaj2Min(char * str); //Converti une chaine de caractères en minuscules
void my_str_est_entier(char * str); //Regarde si la chaine de caractères est composée d'entiers
int my_str_est_alpha(char * str); //Regarde si la chaine de caractères est composée de caractères alphabétiques
void my_strmiroir(char * str); //Renverse une chaine de caractères
void my_strmiroir2(char* str); //Renverse une chaine de caractères
int my_str_palindrome(char * str); //Retourne 0 si la chaine de caractères est un palindrome
int nbmots(char * str); //Retourne le nombre de mots dans une chaine de caractères
char *my_strpbrk(char *s1, char *s2); //Recherche le premier caractère d'une chaine de caractères str2 comprise dans la chaine de caractères str1
char * my_strtok(char * src, char * token); //Retourne le premier mot devant un token spécifié
char *my_strstr(const char *str1, const char *str2);

char* hash(const char *password, const char *hash_key);
int encrypt(const char *target_file, const char *source_file, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);
int decrypt(const char *target_file, const char *source_file, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);

void set_shell_path();
const char* get_executable_path();
void set_history_path();
void set_rc_path();
void ascii_art();
int nbtoken(char* src, char* delim);
FILE * open_file(char * nomFic, char * mode);
int shell_dump(char *infilename, int start, int stop, int nbOctet, char *format);
int shell_search_dump(char *src, char *todump);
int shell_ls(char *option, char *dir, int nbargs);
void shell_pwd();
void rc_check();
void history_check();
void add_history(char* input);
void shell_history();
char* shell_read_line();
int shell_split_line(char* src, char* tokens[], char* delim);
void handler();
void handlersigquit();
void handlersigsegv();
void handlersighup();
int shell_cd(char* path);
int shell_date();
void shell_echo(char* src);
void shell_head(char* src, int nb);
void shell_mkdir(char* src);
void shell_rmdir(char* src);
void shell_help();
void shell_clear();
void shell_PS1(char* src, char* src2);
void shell_cat(char* src);
int shell_sudo();
int check_encrypted();
int shell_encryption();
int shell_init_shell();
int shell_adduser();
int shell_exit();
void prompt();

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
int wrapper_shell_adduser(char** args, int n);
int wrapper_shell_exit(char** args, int n);

int shell_launch(char** tokens, int n);

#endif
