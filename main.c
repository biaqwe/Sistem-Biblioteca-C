#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


int nrCarti=0;
int nrUtilizatori=0;

typedef struct
{
    char titlu[100];
    char autor[100];
    char tip[100];
    char locatie[100];
    int disponibila;
}carte;

typedef struct
{
    char nume[100];
    char parola[100];
    struct
    {
        char titlu[100];
        int zile;
    }imprumutate[10];
    int nrImprumutate;
}utilizator;

carte biblioteca[10000];
utilizator utilizatori[1000];
utilizator *currentUser;

void citire(const char *file)
{
    FILE *fisier=fopen(file, "r");
    if(fisier==NULL)
    {
        printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
        return;
    }
    while(nrCarti<10000 && fscanf(fisier, "%99[^,],%99[^,],%99[^,],%99[^,],%d", biblioteca[nrCarti].titlu, biblioteca[nrCarti].autor, biblioteca[nrCarti].tip, biblioteca[nrCarti].locatie, &biblioteca[nrCarti].disponibila) == 5)
    {
        nrCarti++;
        int c;
        while((c=fgetc(fisier))!=EOF && c!='\n');
    }
    if(nrCarti==10000)
    {
        printf("\033[38;2;231;29;54mAtentie: Numarul maxim de carti a fost atins.\033[0m\n");
    }
    fclose(fisier);
}

int existaUtilizator(const char *numeUtilizator, const char *numeFisier)
{
    FILE *fisier=fopen(numeFisier, "r");
    if(fisier==NULL)
    {
        printf("\033[38;2;231;29;54mEroare la deschiderea fisierului pentru citire.\033[0m\n");
        return 0;
    }
    char nume[100];
    char parola[100];
    while(fscanf(fisier, "%s %s", nume, parola)==2)
    {
        if(strcmp(nume, numeUtilizator)==0)
        {
            fclose(fisier);
            return 1;
        }
    }
    fclose(fisier);
    return 0;
}

void salveazaUtilizatoriInFisier(const char *numeFisier)
{
    FILE *fisier=fopen(numeFisier, "a");
    int i;
    if(fisier==NULL)
    {
        printf("\033[38;2;231;29;54mEroare la deschiderea fisierului pentru scriere.\033[0m\n");
        return;
    }
    for(i=0; i<nrUtilizatori; i++)
    {
        if(!existaUtilizator(utilizatori[i].nume, numeFisier))
            fprintf(fisier, "%s %s\n", utilizatori[i].nume, utilizatori[i].parola);
    }
    fclose(fisier);
}

void citesteUtilizatoriDinFisier(const char *numeFisier)
{
    FILE *fisier=fopen(numeFisier, "r");
    if(fisier==NULL)
    {
        printf("\033[38;2;231;29;54mEroare la deschiderea fisierului pentru citire.\033[0m\n");
        return;
    }
    while(nrUtilizatori<1000 && fscanf(fisier, "%99s %99s", utilizatori[nrUtilizatori].nume, utilizatori[nrUtilizatori].parola)==2)
    {
        nrUtilizatori++;
    }
    if(nrUtilizatori==1000)
        printf("\033[38;2;231;29;54mAtentie: Numarul maxim de utilizatori a fost atins.\033[0m\n");
    fclose(fisier);
}

void meniu()
{
    printf("\n");
    printf("                              *----------------------------------------------*\n");
    printf("                              |                  \033[38;2;216;221;239mMeniu:\033[0m                      |\n");
    printf("                              *----------------------------------------------*\n");
    printf("                              | \033[38;2;216;221;239m1.\033[0m \033[38;2;160;164;184mAdauga o carte.                           \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m2.\033[0m \033[38;2;114;147;160mCauta o carte.                            \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m3.\033[0m \033[38;2;69;182;156mImprumuta o carte.                        \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m4.\033[0m \033[38;2;33;209;159mReturneaza o carte.                       \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m5.\033[0m \033[38;2;67;249;159mVizualizare profil.                       \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m6.\033[0m \033[38;2;88;255;208mAjutor.                                   \033[0m|\n");
    printf("                              | \033[38;2;216;221;239m7.\033[0m \033[38;2;231;29;54mIesire.                                   \033[0m|\n");
    printf("                              *----------------------------------------------*\n");
    printf("\n");
}
void eliminareSpatii(char *str)
{
    int i=0, j=strlen(str)-1;
    while(isspace(str[i]))
        i++;
    while(j>=0 && isspace(str[j]))
        j--;
    str[j+1]='\0';
    if(i>0)
        strcpy(str, str + i);
}

int gaseste(char numeCarte[], char numeAutor[])
{
    int i;
    for(i=0; i<nrCarti; i++)
    {
        char titlu[100], autor[100];
        strcpy(titlu, biblioteca[i].titlu);
        strcpy(autor, biblioteca[i].autor);
        eliminareSpatii(titlu);
        eliminareSpatii(autor);
        if(strcmp(titlu, numeCarte)==0 && strcmp(autor, numeAutor)==0)
            return i;
    }
    return -1;
}

void vizualizareProfil(utilizator *user)
{
    printf("\033[38;2;67;249;159mProfilul utilizatorului\033[0m \033[38;2;247;191;180m%s\033[0m\n", user->nume);
    printf("\033[38;2;137;49;104mCarti imprumutate:\033[0m\n");
    printf("%-40s %-10s\n", "Titlu", "Zile");
    FILE *fisier=fopen("imprumuturi.txt", "r");
    if(fisier==NULL)
    {
        printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
        return;
    }
    char numeUtilizator[100];
    char titluCarte[100];
    int zile;
    while(fscanf(fisier, "%99s", numeUtilizator)!=EOF)
    {
        fscanf(fisier, " %99[^0123456789] %d", titluCarte, &zile);
        if(strcmp(numeUtilizator, user->nume)==0)
            printf("%-40s %-10d\n", titluCarte, zile);
    }
    fclose(fisier);
}

void adauga()
{
    printf("\033[38;2;160;164;184mAdauga o carte:\033[0m\n");
    if(nrCarti==10000)
    {
        printf("\033[38;2;231;29;54mAtentie: Numarul maxim de carti a fost atins.\033[0m\n");
    }
    else
    {
        printf("\033[38;2;137;49;104mIntroduceti datele necesare...\033[0m\n");
        printf("\033[38;2;219;147;176mIntroduceti numele cartii:\033[0m\n");
        fflush(stdin);
        fgets(biblioteca[nrCarti].titlu, sizeof(biblioteca[nrCarti].titlu), stdin);
        if(strlen(biblioteca[nrCarti].titlu)>0 && biblioteca[nrCarti].titlu[strlen(biblioteca[nrCarti].titlu)-1]=='\n')
            biblioteca[nrCarti].titlu[strlen(biblioteca[nrCarti].titlu)-1]='\0';
        printf("\033[38;2;219;147;176mIntroduceti numele autorului:\033[0m\n");
        fflush(stdin);
        fgets(biblioteca[nrCarti].autor, sizeof(biblioteca[nrCarti].autor), stdin);
        if(strlen(biblioteca[nrCarti].autor)>0 && biblioteca[nrCarti].autor[strlen(biblioteca[nrCarti].autor)-1]=='\n')
            biblioteca[nrCarti].autor[strlen(biblioteca[nrCarti].autor)-1]='\0';
        printf("\033[38;2;219;147;176mIntroduceti tipul cartii:\033[0m\n");
        fflush(stdin);
        fgets(biblioteca[nrCarti].tip, sizeof(biblioteca[nrCarti].tip), stdin);
        if(strlen(biblioteca[nrCarti].tip)>0 && biblioteca[nrCarti].tip[strlen(biblioteca[nrCarti].tip)-1]=='\n')
            biblioteca[nrCarti].tip[strlen(biblioteca[nrCarti].tip)-1]='\0';
        printf("\033[38;2;219;147;176mIntroduceti locatia cartii:\033[0m\n");
        fflush(stdin);
        fgets(biblioteca[nrCarti].locatie, sizeof(biblioteca[nrCarti].locatie), stdin);
        if(strlen(biblioteca[nrCarti].locatie)>0 && biblioteca[nrCarti].locatie[strlen(biblioteca[nrCarti].locatie)-1]=='\n')
            biblioteca[nrCarti].locatie[strlen(biblioteca[nrCarti].locatie)-1]='\0';
        printf("\033[38;2;125;175;156mCartea\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156ma fost adaugata.\033[0m\n", biblioteca[nrCarti].titlu);
        biblioteca[nrCarti].disponibila=1;
        nrCarti++;
        FILE *fisier=fopen("biblioteca.txt", "a");
        if(fisier==NULL)
        {
            printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
            return;
        }
        fprintf(fisier, "%s,%s,%s,%s,%d\n", biblioteca[nrCarti-1].titlu, biblioteca[nrCarti-1].autor, biblioteca[nrCarti-1].tip, biblioteca[nrCarti-1].locatie, biblioteca[nrCarti-1].disponibila);
        fclose(fisier);
    }

}

void afisare(carte biblioteca[], int nrCarti)
{
    int i;
    printf("\033[38;2;137;49;104mCarti disponibile in biblioteca:\033[0m\n");
    printf("%-5s %-40s %-30s %-15s %-20s\n", "Nr.", "Titlu", "Autor", "Tip", "Locatie");
    for(i=0; i<nrCarti; i++)
    {
        if(biblioteca[i].disponibila)
        {
            printf("%-5d %-40s %-30s %-15s %-20s\n", i+1, biblioteca[i].titlu, biblioteca[i].autor, biblioteca[i].tip, biblioteca[i].locatie);
        }
    }
}

void cauta()
{
    while(true)
    {
        printf("\033[38;2;114;147;160mCauta o carte:\033[0m\n");
        printf("\033[38;2;137;49;104mIntroduceti datele necesare...\033[0m\n");
        printf("\033[38;2;219;147;176mIntroduceti criteriul de cautare:\033[0m\n\033[38;2;216;221;239m1.\033[0m \033[38;2;229;179;199mDupa autor.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;229;179;199mDupa tip.\033[0m\n");
        int criteriu, i;
        scanf("%d", &criteriu);
        if(criteriu==1)
        {
            char numeAutor[100];
            printf("\033[38;2;240;211;223mIntroduceti numele autorului:\033[0m\n");
            fflush(stdin);
            fgets(numeAutor, sizeof(numeAutor), stdin);
            if(strlen(numeAutor)>0 && numeAutor[strlen(numeAutor)-1]=='\n')
                numeAutor[strlen(numeAutor)-1]='\0';
            printf("\033[38;2;125;175;156mCartile scrise de autorul\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156msunt:\n\033[0m", numeAutor);
            printf("%-5s %-40s %-30s %-15s %-20s\n", "Nr.", "Titlu", "Autor", "Tip", "Locatie");
            for(i=0; i<nrCarti; i++)
            {
                if(strcasecmp(biblioteca[i].autor, numeAutor)==0)
                {
                    printf("%-5d %-40s %-30s %-15s %-20s\n",i+1, biblioteca[i].titlu, biblioteca[i].autor, biblioteca[i].tip, biblioteca[i].locatie);
                }
            }
            break;
        }
        else if(criteriu==2)
        {
            char numeTip[100];
            printf("\033[38;2;240;211;223mIntroduceti tipul cartii:\033[0m\n");
            fflush(stdin);
            fgets(numeTip, sizeof(numeTip), stdin);
            if(strlen(numeTip)>0 && numeTip[strlen(numeTip)-1]=='\n')
                numeTip[strlen(numeTip)-1]='\0';
            printf("\033[38;2;125;175;156mCartile de tipul\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156msunt:\033[0m\n", numeTip);
            printf("%-5s %-40s %-30s %-15s %-20s\n", "Nr.", "Titlu", "Autor", "Tip", "Locatie");
            for(i=0; i<nrCarti; i++)
            {
                if(strcasecmp(biblioteca[i].tip, numeTip)==0)
                {
                     printf("%-5d %-40s %-30s %-15s %-20s\n",i+1, biblioteca[i].titlu, biblioteca[i].autor, biblioteca[i].tip, biblioteca[i].locatie);
                }
            }
            break;
        }
        else
        {
            printf("\033[38;2;231;29;54mCriteriu de cautare invalid.\033[0m\n");
            continue;
        }
    }
}

void adaugaUtilizator(const char *nume, const char *parola)
{
    if(nrUtilizatori<1000)
    {
        strcpy(utilizatori[nrUtilizatori].nume, nume);
        strcpy(utilizatori[nrUtilizatori].parola, parola);
        nrUtilizatori++;
        printf("\033[38;2;231;29;54mUtilizatorul a fost adaugat cu succes. Este necesara redeschiderea aplicatiei pentru a va putea conecta.\033[0m\n");
        salveazaUtilizatoriInFisier("utilizatori.txt");
    }
    else
        printf("Atentie: Numarul maxim de utilizatori a fost atins.\n");
}

utilizator* logare(utilizator utilizatori[], int nrUtilizatori)
{
    char nume[100], parola[100];
    int optiune, i;
    while(true)
    {
        printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;137;49;104mConectare\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;137;49;104mInregistrare\033[0m\n\033[38;2;216;221;239m3.\033[0m \033[38;2;137;49;104mIesire\033[0m\n");
        printf("Selectati o optiune:\033[0m ");
        scanf("%d", &optiune);
        if(optiune==3)
            return NULL;
        if(optiune==2)
        {
            printf("\033[38;2;219;147;176mIntroduceti numele de utilizator pentru noul cont:\033[0m\n");
            scanf("%s", nume);
            for(i=0; i<nrUtilizatori; i++)
                if(strcmp(utilizatori[i].nume, nume)==0)
                {
                    printf("Numele de utilizator exista deja. Incercati alt nume.\n");
                    return;
                }
            if(i!=nrUtilizatori)
                continue;
            printf("\033[38;2;219;147;176mIntroduceti parola pentru noul cont:\033[0m\n");
            scanf("%s", parola);
            adaugaUtilizator(nume, parola);
            continue;
        }
        else if(optiune==1)
        {
            printf("\033[38;2;219;147;176mIntroduceti numele de utilizator:\033[0m\n");
            scanf("%s", nume);
            printf("\033[38;2;219;147;176mIntroduceti parola:\033[0m\n");
            scanf("%s", parola);
            for(i=0; i<nrUtilizatori; i++)
                if(strcmp(utilizatori[i].nume, nume)==0 && strcmp(utilizatori[i].parola, parola)==0)
                {
                    currentUser=&utilizatori[i];
                    return currentUser;
                }
            printf("\033[38;2;231;29;54mNume de utilizator sau parola incorecta. Incercati din nou.\033[0m\n");
        }
        else
        {
            printf("\033[38;2;231;29;54mOptiune invalida. Selectati din nou.\033[0m\n");
            continue;
        }
    }
    return NULL;
}
void adaugaCarteImprumutata(utilizator *user, const char *titlu, int zile)
{
    if(user->nrImprumutate<100)
    {
        strcpy(user->imprumutate[user->nrImprumutate].titlu, titlu);
        user->imprumutate[user->nrImprumutate].zile=zile;
        user->nrImprumutate++;
        FILE *fisier = fopen("imprumuturi.txt", "a");
        if(fisier==NULL)
        {
            printf("Eroare la deschiderea fisierului.\n");
            return;
        }
        fprintf(fisier, "%s ", user->nume);
        fprintf(fisier, "%s %d\n", titlu, zile);
        fclose(fisier);
    }
    else
        printf("Utilizatorul %s a atins numarul maxim de carti imprumutate.\n", user->nume);
}

void imprumuta(utilizator *user)
{
    int j;
    while(true)
    {
        printf("\033[38;2;69;182;156mImprumuta o carte:\033[0m\n");
        printf("\033[38;2;137;49;104mIntroduceti datele necesare...\033[0m\n");
        printf("\033[38;2;219;147;176mIntroduceti numele cartii:\033[0m\n");
        char numeCarte[100];
        fflush(stdin);
        fgets(numeCarte, sizeof(numeCarte), stdin);
        if(strlen(numeCarte)>0 && numeCarte[strlen(numeCarte)-1]=='\n')
            numeCarte[strlen(numeCarte)-1] = '\0';
        printf("\033[38;2;219;147;176mIntroduceti numele autorului:\033[0m\n");
        char numeAutor[100];
        fflush(stdin);
        fgets(numeAutor, sizeof(numeAutor), stdin);
        if(strlen(numeAutor)>0 && numeAutor[strlen(numeAutor)-1]=='\n')
            numeAutor[strlen(numeAutor)-1]='\0';
        int i=gaseste(numeCarte, numeAutor);
        if(i!=-1)
            if(biblioteca[i].disponibila)
            {
                printf("\033[38;2;219;147;176mIntroduceti numarul de zile pentru imprumut:\033[0m\n");
                int zile;
                scanf("%d", &zile);
                printf("\033[38;2;125;175;156mCartea\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156ma fost imprumuta de\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156mtimp de\033[0m \033[38;2;247;191;180m%d\033[0m \033[38;2;125;175;156mzile.\033[0m\n", numeCarte, user->nume, zile);
                biblioteca[i].disponibila--;
                adaugaCarteImprumutata(user, numeCarte, zile);
                FILE *fisier=fopen("biblioteca.txt", "w");
                if(fisier==NULL)
                {
                    printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
                    return;
                }
                for(j=0; j<nrCarti; j++)
                    fprintf(fisier, "%s,%s,%s,%s,%d\n", biblioteca[j].titlu, biblioteca[j].autor, biblioteca[j].tip, biblioteca[j].locatie, biblioteca[j].disponibila);
                fclose(fisier);
                break;
            }
            else
                printf("\033[38;2;231;29;54mAceasta carte nu este disponibila.\033[0m\n");
        else
            printf("\033[38;2;231;29;54mCartea nu a fost gasita.\033[0m\n");
    }
}

void returneaza(utilizator *user)
{
    int j;
    while(true)
    {
        printf("\033[38;2;33;209;159mReturneaza o carte:\033[0m\n");
        printf("\033[38;2;137;49;104mIntroduceti datele necesare...\033[0m\n");
        printf("\033[38;2;219;147;176mIntroduceti numele cartii:\033[0m\n");
        char numeCarte[100];
        fflush(stdin);
        fgets(numeCarte, sizeof(numeCarte), stdin);
        if(strlen(numeCarte)>0 && numeCarte[strlen(numeCarte)-1]=='\n')
            numeCarte[strlen(numeCarte)-1]='\0';
        printf("\033[38;2;219;147;176mIntroduceti numele autorului:\033[0m\n");
        char numeAutor[100];
        fflush(stdin);
        fgets(numeAutor, sizeof(numeAutor), stdin);
        if(strlen(numeAutor)>0 && numeAutor[strlen(numeAutor)-1]=='\n')
            numeAutor[strlen(numeAutor)-1]='\0';
        int i=gaseste(numeCarte, numeAutor);
        if(i!=-1)
        {
            printf("\033[38;2;125;175;156mCartea\033[0m \033[38;2;247;191;180m%s\033[0m \033[38;2;125;175;156ma fost returnata.\033[0m\n", numeCarte);
            biblioteca[i].disponibila++;
            if(user!=NULL)
            {
                FILE *imprumuturi=fopen("imprumuturi.txt", "r");
                if(imprumuturi==NULL)
                {
                    printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
                    return;
                }
                FILE *temp=fopen("temp.txt", "w");
                if(temp==NULL)
                {
                    printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
                    fclose(imprumuturi);
                    return;
                }
                char numeUtilizator[100];
                char titluCarte[100];
                int zile;
                while(fscanf(imprumuturi, "%s %s %d", numeUtilizator, titluCarte, &zile) == 3)
                {
                    if(strcmp(numeUtilizator, user->nume)==0 && strcmp(titluCarte, numeCarte)==0)
                        continue;
                    fprintf(temp, "%s %s %d\n", numeUtilizator, titluCarte, zile);
                }
                fclose(imprumuturi);
                fclose(temp);
                remove("imprumuturi.txt");
                rename("temp.txt", "imprumuturi.txt");
            }
            FILE *fisier=fopen("biblioteca.txt", "w");
            if(fisier==NULL)
            {
                printf("\033[38;2;231;29;54mEroare la deschiderea fisierului.\033[0m\n");
                return;
            }
            for(j=0; j<nrCarti; j++)
                fprintf(fisier, "%s,%s,%s,%s,%d\n", biblioteca[j].titlu, biblioteca[j].autor, biblioteca[j].tip, biblioteca[j].locatie, biblioteca[j].disponibila);
            fclose(fisier);
            break;
        }
        else
            printf("\033[38;2;231;29;54mCartea nu a fost gasita.\033[0m\n");
    }
}

void ajutor()
{
    printf("\033[38;2;137;49;104mDespre care optiune din meniu doriti sa aflati mai multe detalii?\033[0m\n");
    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;160;164;184mAdauga o carte.\033[0m\n");
    printf("\033[38;2;216;221;239m2.\033[0m \033[38;2;114;147;160mCauta o carte.\033[0m\n");
    printf("\033[38;2;216;221;239m3.\033[0m \033[38;2;69;182;156mImprumuta o carte.\033[0m\n");
    printf("\033[38;2;216;221;239m4.\033[0m \033[38;2;33;209;159mReturneaza o carte.\033[0m\n");
    printf("\033[38;2;216;221;239m5.\033[0m \033[38;2;67;249;159mVizualizare profil.\033[0m\n");
    int optiune;
    scanf("%d", &optiune);
     switch(optiune)
     {
        case 1:
            printf("\033[38;2;160;164;184mAdauga o carte:\033[0m\nAceasta optiune permite utilizatorului sa adauge o noua carte in biblioteca.\n");
            break;
        case 2:
            printf("\033[38;2;114;147;160mCauta o carte:\033[0m\nAceasta optiune permite utilizatorului sa caute o carte in biblioteca dupa tip sau dupa autor.\n");
            break;
        case 3:
            printf("\033[38;2;69;182;156mImprumuta o carte:\033[0m\nAceasta optiune permite utilizatorului sa imprumute o carte din biblioteca.\n");
            printf("\033[38;2;231;29;54m*Nume cartii si al autorului trebuie scrise exact ca in lista cu carti disponibile afisate la inceput.\n");
            break;
        case 4:
            printf("\033[38;2;33;209;159mReturneaza o carte:\033[0m\nAceasta optiune permite utilizatorului sa returneze o carte imprumutata.\n");
            printf("\033[38;2;231;29;54m*Nume cartii si al autorului trebuie scrise exact ca in lista cu carti disponibile afisate la inceput.\033[0m\n");
            break;
        case 5:
            printf("\033[38;2;67;249;159mVizualizare profil:\033[0m\nAceasta optiune permite utilizatorului sa vizualizeze cartile imprumutate de el si numarul de zile corespunzator fiecarui imprumut.\033[0m\n");
            break;
        default:
            printf("\033[38;2;231;29;54mOptiune invalida.\033[0m\n");
            break;
    }
}

void afisareASCII()
{
    printf("\033[38;2;217;184;196m");
    printf(" ___           _                _       _                        \n");
    printf("| _ \\__ _ __ _(_)_ _  __ _   __| |___  | |___  __ _ __ _ _ _ ___ \n");
    printf("|  _/ _` / _` | | ' \\/ _` | / _` / -_) | / _ \\/ _` / _` | '_/ -_)\n");
    printf("|_| \\__,_\\__, |_|_||_\\__,_| \\__,_\\___| |_\\___/\\__, \\__,_|_| \\___|\n");
    printf("          |___/                                |___/             \n");
    printf("\033[0m");
    printf("\n");
}
void afisareASCII2()
{
    printf("\033[38;2;217;184;196m");
    printf("                  ,-----.   ,--. ,--.    ,--. ,--.           ,--.                           \n");
    printf("                  |  |) /_  `--' |  |-.  |  | `--'  ,---.  ,-'  '-.  ,---.   ,---.  ,--,--. \n");
    printf("                  |  .-.  \\ ,--. | .-. ' |  | ,--. | .-. | '-.  .-' | .-. : | .--' ' ,-.  | \n");
    printf("                  |  '--' / |  | | `-' | |  | |  | ' '-' '   |  |   \\   --. \\ `--. \\ '-'  | \n");
    printf("                  `------'  `--'  `---'  `--' `--'  `---'    `--'    `----'  `---'  `--`--' \n");
    printf("                                    __...--~~~~~-._   _.-~~~~~--...__    \n");
    printf("                                   //               `V'               \\   \n");
    printf("                                  //                 |                 \\  \n");
    printf("                                 //__...--~~~~~~-._  |  _.-~~~~~~--...__\\ \n");
    printf("                                //__.....----~~~~._\\ | /_.~~~~----.....__\\\n");
    printf("\033[0m");
    printf("\n");
}


int main()
{
    afisareASCII();
    citire("biblioteca.txt");
    citesteUtilizatoriDinFisier("utilizatori.txt");
    utilizator *utilizatorAutentificat;
    utilizatorAutentificat=logare(utilizatori, nrUtilizatori);
    if(utilizatorAutentificat!=NULL)
    {
        system("cls");
        afisareASCII2();
        int optiune, n;
        do
        {
            afisare(biblioteca, nrCarti);
            meniu();
            printf("\033[38;2;137;49;104mSelectati o optiune:\033[0m ");
            scanf("%d", &optiune);
            switch (optiune)
            {
                case 1:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    adauga();
                    break;
                case 2:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    cauta();
                    break;
                case 3:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    imprumuta(utilizatorAutentificat);
                    break;
                case 4:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    returneaza(utilizatorAutentificat);
                    break;
                case 5:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    vizualizareProfil(utilizatorAutentificat);
                    break;
                case 6:
                    printf("\033[38;2;216;221;239m1.\033[0m \033[38;2;219;147;176mInapoi la meniu.\033[0m\n\033[38;2;216;221;239m2.\033[0m \033[38;2;219;147;176mContinua.\033[0m\n");
                    scanf("%d", &n);
                    if(n==1)
                    {
                        system("cls");
                        break;
                    }
                    system("cls");
                    ajutor();
                    break;
                case 7:
                    system("cls");
                    printf("\033[38;2;137;49;104mLa revedere!\033[0m\n");
                    break;
                default:
                    system("cls");
                    printf("\033[38;2;231;29;54mOptiune invalida. Selectati din nou.\033[0m\n");
                    break;
            }
        }while(optiune!=7);
    }
    else
        printf("\033[38;2;137;49;104mLa revedere!\033[0m\n");
    return 0;
}
