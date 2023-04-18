#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include.h"

bool DEBUG=false;

int my_strlen(const char *str)
{
    int i = 0;
    while (*str++) i++;
    return (i);
}

unsigned int my_strnlen(const char *str, unsigned int max_len)
{
    unsigned int len = 0;
    while (len < max_len && str[len] != '\0')
    {
        len++;
    }
    return len;
}

char * my_strcpy(char * dest, const char * src)
{
    char * tmp;
    tmp = dest;
    while(*src != '\0')
    {
        *dest=*src;
        dest++;
        src++;
    }
    *dest = '\0';
    if(DEBUG)
    {
        printf("Copy String : %s\n",tmp);
    }
    return tmp;
}


char * my_mstrcpy(char * dest, const char * src)
{
    char * tmp;
    tmp = dest;
    while(*src != '\0')
    {
        *dest=*src;
        dest++;
        src++;
    }
    *dest = '\0';
    printf("Copy String : %s\n",tmp);
    return tmp;
}


char * my_mstrncpy(const char * src, int count)
{
    char * tmp;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    int c = 0;
    while(c < count)
    {
        src++;
        c++;
    }
    while(*src != '\0')
    {
        *dest=*src;
        dest++;
        src++;
    }
    *dest = '\0';
    printf("Copy String : %s\n",tmp);
    return tmp;
}


char * my_strcat(char * a, char * b)
{
    char * tmp;
    char * dest = malloc(my_strlen(a) * sizeof(char));
    tmp = dest;
    while(*a != '\0')
    {
        *dest=*a;
        dest++;
        a++;
    }
    while(*b != '\0')
    {
        *dest=*b;
        dest++;
        b++;
    }
    *dest = '\0';
    return tmp;
}


char * my_strdup(char *src, int nb_de_fois)
{
    int i;
    char * tmp;
    char * tmp2;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    for(i=0;i<nb_de_fois;i++)
    {
        tmp2 = src;
        tmp = my_strcat(tmp2,tmp);
    }
    return tmp;
}


char * my_strdupin(const char * src)
{
    char * tmp;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    while(*src != '\0')
    {
        *dest=*src;
        dest++;
        src++;
    }
    *dest='\0';
    return tmp;
}


char * my_strdup2(char * src, char * str, int nb_de_fois)
{
    int i;
    char * tmp;
    char * tmp2;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    for(i=0;i<nb_de_fois;i++)
    {
        tmp2 = my_strcat(src,str);
        tmp = my_strcat(tmp2,tmp);
    }
    return tmp;
}


char * my_strtrim(char * src)
{
    char * tmp;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    while(*src != '\0')
    {
        if(*src != ' ')
        {
            *dest=*src;
            dest++;
        }
        src++;
    }
    return tmp;
}


char * my_strtrimCar(char * src, char * asuppr)
{
    char * tmp;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    while(*src != '\0')
    {
        if(*src != *asuppr)
        {
            *dest=*src;
            dest++;
        }
        src++;
    }
    return tmp;
}


int my_strcmp(char * str1, char * str2)
{
    while(*str1 != '\0' && *str2 != '\0')
    {
        if(*str1 != *str2)
        {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return 0;
}

int my_strncmp(const char *s1, const char *s2, unsigned int n)
{
    while (*s1 && *s2 && n > 0)
    {
        if (*s1 != *s2)
        {
            return (*s1 - *s2);
        }
        s1++;
        s2++;
        n--;
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return (*s1 - *s2);
    }
}

void * my_strMin2Maj(char * str)
{
    while(*str != '\0')
    {
        if(*str >= 'a' && *str <= 'z')
        {
            *str = *str - 32;
        }
        str++;
    }
    return str;
}


void * my_strMaj2Min(char * str)
{
    while(*str != '\0')
    {
        if(*str >= 'A' && *str <= 'Z')
        {
            *str = *str + 32;
        }
        str++;
    }
    return str;
}


void my_str_est_entier(char * str)
{
    char * tmp = str;
    while(*str != '\0')
    {
        if(*str < '0' || *str > '9')
        {
            printf("%s => n'est pas un entier",tmp);
            return;
        }
        str++;
    }
    printf("%s => est un entier",tmp);
}


int my_str_est_alpha(char * str)
{
    char * tmp = str;
    while(*str != '\0')
    {
        if((*str < 'a' || *str > 'z') && (*str < 'A' || *str > 'Z'))
        {
//            printf("%s => n'est pas alphabétique",tmp);
            return 1;
        }
        str++;
    }
//    printf("%s => est alphabétique",tmp);
    return 0;
}


void my_strmiroir(char * str)
{
    int i = 0;
    while(*str != '\0')
    {
        str++;
        i++;
    }
    str--;
    while(i > 0)
    {
        printf("%c",*str);
        str--;
        i--;
    }
}


void my_strmiroir2(char* str)
{
    int longueur, i;
    char cara,*tmp, *fin;
    longueur = my_strlen(str);
    tmp = str;
    fin = str;

    for (i = 0; i < longueur - 1; i++)
        fin++;

    for (i = 0; i < longueur / 2; i++) {
        cara = *fin;
        *fin = *tmp;
        *tmp = cara;
        tmp++;
        fin--;
    }
}


int my_str_palindrome(char * str)
{
    my_strMaj2Min(str);
    char dest[10]="";
    my_strcpy(dest,str);
    my_strmiroir2(str);
    if(DEBUG)
    {
        printf("Chaine : %s\n", dest);
        printf("Chaine inversée : %s", str);
    }
    if(my_strcmp(dest,str) == 0)
    {
        return 1;
    }
    return 0;
}


int nbmots(char * str)
{
    int i=0;

    while(*str != '\0')
    {
        if(*str == ' ')
        {
            i++;
        }
        str++;
    }
    return i+1;
}


char *my_strpbrk(char *s1, char *s2)
{
    int i = 0;
    int j = 0;
    while (s1[i] != '\0')
    {
        while (s2[j] != '\0')
        {
            if (s1[i] == s2[j])
            {
                return &s1[i];
            }
            j++;
        }
        j = 0;
        i++;
    }
    return NULL;
}


char * my_strtok(char * src, char * token)
{
    char * tmp;
    char * dest = malloc(my_strlen(src) * sizeof(char));
    tmp = dest;
    while(*src != *token && *src != '\0' && *src != '\n')
    {
        *dest=*src;
        dest++;
        src++;
    }
    *dest='\0';
    return tmp;
}


char *my_strstr(const char *str1, const char *str2)
{
    const char *h, *n;

    while (*str1 != '\0')
    {
        h = str1;
        n = str2;

        while (*n != '\0' && *h == *n)
        {
            h++;
            n++;
        }

        if (*n == '\0')
        {
            return (char *) str1;
        }

        str1++;
    }

    return NULL;
}