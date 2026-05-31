#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <sys/dir.h>
#include <limits.h>
#include <dirent.h>

#define ROZMIAR_BUFORA 1024


void znak_zachety();
void pomoc();
void zmiana_katalogu(char *katalog[]);
void head(char *plik[], int n);
void grep(char *wzorzec[]);
extern void koniec();
void inne_polecenia(char *polecenie[]);


int main(int argc, char *argv[])
{
    while(1)
    {
        znak_zachety();

        char dane[ROZMIAR_BUFORA];
        const char delim[] = " NULL\n";
        char *schowek;
        if(fgets(dane, ROZMIAR_BUFORA, stdin) != NULL)
        {
            schowek = strtok(dane, delim);
        }
        else
        {
            perror("blad wczytania danych z wiersza polecen");
        }

        char *komenda[ROZMIAR_BUFORA];
        int i = 0;
        while(schowek != NULL)
        {
            komenda[i] = schowek;

            schowek = strtok(NULL, delim);
            i++;
        }
        komenda[i] = NULL;


        if(strcmp(komenda[0], "help") == 0)
        {
            if(i != 1)
            {
                printf("blad uzycia: za duzo danych\n");
            }
            else
            {
                pomoc();
            }
        }
        else if(strcmp(komenda[0], "cd") == 0)
        {
            if(i != 2)
            {
                printf("blad uzycia: poprawne uzycie: cd sciezka_do_katalogu\n");
            }
            else
            {
                zmiana_katalogu(komenda);
            }
        }
        else if(strcmp(komenda[0], "head") == 0)
        {
            if(i != 2 && i != 3)
            {
                printf("blad uzycia: poprawne uzycie: head -nLiczba_Wierszy nazwa_pliku\n");
            }
            else if(i == 3 && strncmp(komenda[1], "-n", 2) != 0)
            {
                printf("nipoprawna opcja: poprawne uzycie: head -nLiczba_Wierszy nazwa_pliku\n");
                printf("%s\n", dane);
            }
            else
            {
                head(komenda, i);
            }
        }
        else if(strcmp(komenda[0], "grep") == 0)
        {
            if(i != 3)
            {
                printf("blad uzycia: poprawnne uzycie: grep wzorzec nazwa_pliku\n");
            }
            else
            {
                grep(komenda);
            }
        }
        else if(strcmp(komenda[0], "exit") == 0)
        {
            if(i != 1)
            {
                printf("blad uzycia: za duzo danych\n");
            }
            else
            {
                koniec();
            }
        }
        else
        {
            inne_polecenia(komenda);
        }
    }

    return 0;
}

void znak_zachety()
{
        char biezacy_katalog[FILENAME_MAX];
        char *uzytkownik = getenv("USER");
        if(uzytkownik == NULL)
        {
            perror("blad pobierania nazwy uzytkownika");
        }

        if(getcwd(biezacy_katalog, FILENAME_MAX) != NULL)
        {
            printf("[%c[%dm%s:%c[%dm%s%c[%dm]$\n", 0x1B, 96, uzytkownik, 0x1B, 36, biezacy_katalog, 0x1B, 0);
        }
        else
        {
            perror("bląd pobierania danych o biezacym katalogu roboczym");
        }
}

void pomoc()
{
    printf("%c[%dmautorem programu jest ALICJA ---\n", 0x1B, 95);
    printf("~jesli chcesz przejsc do innego katalogu wpisz: cd [katalog]\n");
    printf("~jesli chcesz sprawdzic kilka pierwszych linii pliku tekstowego wpisz: head -nLiczba_wierszy nazwa_pliku\n");
    printf("~jesli chcesz znalezc slowo w pliku wpisz: grep wzorzec nazwa_pliku\n");
    printf("~jesli jestes usatysfakcjonowany, mozesz zakonczyc dzialanie programu wpisujac: exit\n%c[%dm", 0x1B, 0);
}

void zmiana_katalogu(char *katalog[])
{
    const char *kat = katalog[1];
    if(strcmp(kat, "~") == 0)
    {
        char *kat_dom = getenv("HOME");
        if(kat_dom != NULL)
        {
            int domowy = chdir(kat_dom);
            if(domowy == -1)
            {
                perror("blad przejscia do katalogu domowego");
            }
        }
        else
        {
            perror("blad pobierania sciezki do katalogu domowego");
        }
    }
    else
    {
        int zmiana = chdir(kat);
        if(zmiana == -1)
        {
            perror("blad przejscia do katalogu");
        }
    }

}

void head(char *plik[], int n)
{
    FILE *fp;
    int ile_wierszy;
    if(n == 2)
    {
       fp = fopen(plik[1], "r");
       ile_wierszy = 10;
    }
    else
    {
        char ilosc[strlen(plik[1])];
        int j = 2;
        int k = 0;
        while(k < strlen(plik[1]))
        {
            ilosc[k] = plik[1][j];
            k++;
            j++;
        }
        ile_wierszy = atoi(ilosc);
        fp = fopen(plik[2],"r");
    }

    if(ile_wierszy == 0)
    {
        printf("niepoprawna liczba wierszy\n");
    }
    else if(fp == 0)
    {
        perror("blad otwierania pliku");
    }
    else
    {
        int licznik = 0;
        char wiersz[ROZMIAR_BUFORA];
        while(fgets(wiersz, ROZMIAR_BUFORA, fp) != NULL && licznik < ile_wierszy)
        {
            printf("%s", wiersz);
            licznik++;
        }
    }
    fclose(fp);
}

void grep(char *wzorzec[])
{
    FILE *fp = fopen(wzorzec[2], "r");
    if(fp == 0)
    {
        perror("blad otwierania pliku");
    }
    else
    {
        char wiersz[ROZMIAR_BUFORA];
        while(fgets(wiersz, ROZMIAR_BUFORA,fp) != NULL)
        {
            if(strstr(wiersz, wzorzec[1]) != NULL)
            {
                printf("%s", wiersz);
            }
        }
    }
}

void inne_polecenia(char *polecenie[])
{
    pid_t fork_id = fork();
    if (fork_id == 0)
    {
        int inne = execvp(polecenie[0], polecenie);
        if(inne == -1)
        {
            perror("blad polecenia");
        }
    }
    else if(fork_id == -1)
    {
        perror("blad procesu potomnego");
    }
    else
    {
        wait(NULL);
    }
}

extern void koniec()
{
    exit(EXIT_SUCCESS);
}
