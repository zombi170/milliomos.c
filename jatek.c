#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "lista.h"
#include "file.h"
#include "jatek.h"
#include "debugmalloc.h"

Nehezseg valaszt(Milliomos *game){
    Nehezseg diff;
    char s;
    bool siker = false;
    while (!siker){
        printf("\nValassz nehezsegi szintet!\n\n");
        printf("1. (K)ezdo\n");
        printf("2. (N)ormal\n");
        printf("3. (P)rofi\n");
        scanf("\n");
        scanf(" %c%*[^\n]",&s);
        switch (s){
            case '1':
            case 'K':
            case 'k':
                diff.easy = 8;
                diff.mid = 5;
                diff.hard = 2;
                game->tries = 3;
                game->diff = 1;
                siker = true;
                break;

            case '2':
            case 'N':
            case 'n':
                diff.easy = 5;
                diff.mid = 5;
                diff.hard = 5;
                game->tries = 2;
                game->diff = 2;
                siker = true;
                break;

            case '3':
            case 'P':
            case 'p':
                diff.easy = 2;
                diff.mid = 5;
                diff.hard = 8;
                game->tries = 1;
                game->diff = 3;
                siker = true;
                break;

            default :
                printf("Ervenytelen bevitel!\n");
                break;
        }
    }
    return diff;
}

int sorkerdes(Milliomos *game){
    char s;
    bool siker = false;
    while (!siker) {
        printf("\nSzeretnel sorkerdest?\n\n");
        printf("1. (I)gen\n");
        printf("2. (N)em\n");
        scanf("\n");
        scanf(" %c%*[^\n]", &s);
        switch (s){
            case '1':
            case 'i':
            case 'I':
                game->sor = read_sorkerdes(game->fv); //sorkérdés kiválasztása az adott fáljból
                if (game->sor == NULL){
                    printf("Sorkerdes hibas!\n");
                    return -4;
                }
                game->skerdes = true;
                siker = true;
                break;

            case '2':
            case 'n':
            case 'N':
                game->skerdes = false;
                siker = true;
                break;

            default :
                printf("Ervenytelen bevitel!\n");
                break;
        }
    }
    return 0;
}

void trim(char *kerdes){
    char temp[1000];
    int i=0, j=0;
    while (kerdes[i] != '\0'){
        if (kerdes[i] == '"'){
            if (kerdes[i+1] == '"'){
                temp[j++] = kerdes[i++];
            }
            else{
                i++;
            }
        }
        else{
            temp[j++] = kerdes[i++];
        }
    }
    temp[j] = '\0';
    strcpy(kerdes,temp);
}

FelValasz *jatek_hozzafuz(FelValasz *start, FelValasz *data){
    FelValasz *uj = (FelValasz*) malloc(sizeof(FelValasz));
    if (uj == NULL){
    printf("Nem sikerult a dinamikus foglalas!\n");
    return NULL;
    }

    trim(data->kerdes);

    /*Lemásoljuk a struktúrát egy új dinamikusan foglalt területre (az indexen kívül)*/
    uj->lvl = data->lvl;
    uj->kerdes = dintomb_foglal(data->kerdes);
    uj->v_A = dintomb_foglal(data->v_A);
    uj->v_B = dintomb_foglal(data->v_B);
    uj->v_C = dintomb_foglal(data->v_C);
    uj->v_D = dintomb_foglal(data->v_D);
    uj->valasz = data->valasz;
    uj->kategoria = dintomb_foglal(data->kategoria);
    uj->next = NULL;    //ha a végére visszük a listának, akkor még nem mutat következő elemre

    FelValasz *move = start;
    FelValasz *inchworm = NULL;
    /*Beszúrás helyének megkeresése*/
    while (move != NULL && move->lvl < uj->lvl){
        inchworm = move;
        move = move->next;
    }
    /*Nincs még lista vagy az első elé szúrjuk be*/
    if (inchworm == NULL){
        uj->next = start;
        start = uj;
    }
    /*Lista belsejébe vagy végére szúrás*/
    else{
        inchworm->next = uj;
        uj->next = move;
    }
    return start;
}

FelValasz *generalas(FelValasz **start, const Nehezseg diff, const int db){
    FelValasz *elem = NULL, *game = NULL;
    /*A könnyű kategóriájú kérdések kiválogatása*/
    int j = 0, random;
    random = rand()%db+1;
    while (j != diff.easy) {
        elem = lista_keres(*start, random, 1, 3);
        if (elem != NULL && elem->id < db){
            random = elem->id + 1;
            game = jatek_hozzafuz(game, elem);
            *start = lista_torles(*start, elem->id);
            j++;
        }
        else{
            random = rand()%db+1;
        }
    }
    /*A közepes kategóriájú kérdések kiválogatása*/
    j = 0;
    while (j != diff.mid) {
        elem = lista_keres(*start, random, 4, 7);
        if (elem != NULL && elem->id < db){
            random = elem->id + 1;
            game = jatek_hozzafuz(game, elem);
            *start = lista_torles(*start, elem->id);
            j++;
        }
        else{
            random = rand()%db+1;
        }
    }
    /*A nehéz kategóriájú kérdések kiválogatása*/
    j = 0;
    while (j != diff.hard) {
        elem = lista_keres(*start, random, 8, 15);
        if (elem != NULL && elem->id < db){
            random = elem->id + 1;
            game = jatek_hozzafuz(game, elem);
            *start = lista_torles(*start, elem->id);
            j++;
        }
        else{
            random = rand()%db+1;
        }
    }
    return game;
}

int min(int a, int b){
    return a < b ? a : b;
}

void felezes(Milliomos *game){
    printf("Felezes utan:\n");
    int r = rand()%3+1;
    switch (game->fv->valasz){
        case 'A': switch (r) {
                case 1 :
                    printf("A: %s | B: %s\n", game->fv->v_A, game->fv->v_B);
                    break;
                case 2 :
                    printf("A: %s | C: %s\n", game->fv->v_A, game->fv->v_C);
                    break;
                case 3 :
                    printf("A: %s | D: %s\n", game->fv->v_A, game->fv->v_D);
                    break;
                default : printf("Hibas randomizacio a felezes fuggvenyben!\n");
            }
            break;
        case 'B': switch (r) {
                case 1 :
                    printf("A: %s | B: %s\n", game->fv->v_A, game->fv->v_B);
                    break;
                case 2 :
                    printf("B: %s | C: %s\n", game->fv->v_B, game->fv->v_C);
                    break;
                case 3 :
                    printf("B: %s | D: %s\n", game->fv->v_B, game->fv->v_D);
                    break;
                default : printf("Hibas randomizacio a felezes fuggvenyben!\n");
            }
            break;
        case 'C': switch (r) {
                case 1 :
                    printf("B: %s | C: %s\n", game->fv->v_B, game->fv->v_C);
                    break;
                case 2 :
                    printf("A: %s | C: %s\n", game->fv->v_A, game->fv->v_C);
                    break;
                case 3 :
                    printf("C: %s | D: %s\n", game->fv->v_C, game->fv->v_D);
                    break;
                default : printf("Hibas randomizacio a felezes fuggvenyben!\n");
            }
            break;
        case 'D': switch (r) {
                case 1 :
                    printf("B: %s | D: %s\n", game->fv->v_B, game->fv->v_D);
                    break;
                case 2 :
                    printf("C: %s | D: %s\n", game->fv->v_C, game->fv->v_D);
                    break;
                case 3 :
                    printf("A: %s | D: %s\n", game->fv->v_A, game->fv->v_D);
                    break;
                default : printf("Hibas randomizacio a felezes fuggvenyben!\n");
            }
            break;
        default : printf("Hibas a valasz a jatek dokumentumaban!\n");
    }
    game->felezes = false;
}

void kozonseg(Milliomos *game){
    printf("Kozonseg segitseg:\n");
    int r = min(98,rand()%60+30);
    int a, b, c, d;
    switch (game->fv->valasz){
        case 'A' : a = r;
            b = min(99-a,rand()%(100-a)+2);
            c = min(100-a-b,rand()%(100-a-b)+1);
            d = 100-a-b-c;
            printf("A: %d %% | B: %d %% | C: %d %% | D: %d %%\n",a,b,c,d);
            break;
        case 'B' : b = r;
            a = min(99-b,rand()%(100-b)+2);
            c = min(100-a-b,rand()%(100-a-b)+1);
            d = 100-a-b-c;
            printf("A: %d %% | B: %d %% | C: %d %% | D: %d %%\n",a,b,c,d);
            break;
        case 'C' : c = r;
            b = min(99-c,rand()%(100-c)+2);
            a = min(100-c-b,rand()%(100-c-b)+1);
            d = 100-a-b-c;
            printf("A: %d %% | B: %d %% | C: %d %% | D: %d %%\n",a,b,c,d);
            break;
        case 'D' : d = r;
            b = min(99-d,rand()%(100-d)+2);
            c = min(100-d-b,rand()%(100-d-b)+1);
            a = 100-d-b-c;
            printf("A: %d %% | B: %d %% | C: %d %% | D: %d %%\n",a,b,c,d);
            break;
    }
    game->kozonseg = false;
}

void timer(Milliomos *game, long ido){
    /*Maximum érték elérésénél nem lehet nagyobb*/
    if (ido > 60*60){
        ido = 60*60;
    }
    /*Percekre váltás*/
    if (ido >= 60){
        game->min = ido / 60;
    }
    game->sec = ido % 60;
}

void feloszt(long sz){
    sz /= 1000;
    if (sz < 1000){
        printf("%ld ",sz);
        return;
    }
    else{
        feloszt(sz);
        if (sz % 1000 == 0)
            printf("000 ");
        else
            printf("%0.3ld ",sz % 1000);
    }
}

bool part_sorkerdes(Milliomos *game, const long *start){
    printf("\nOsszesen %d probalkozasod van, hogy eltalald a helyes sorrendet!\n\n",game->tries);
    printf("%s\n",game->sor->kerdes);
    printf("A: %s | B: %s | C: %s | D: %s\n",game->sor->v_A,game->sor->v_B,game->sor->v_C,game->sor->v_D);
    char K[2];
    K[0] = 'K';
    K[1] = '\0';
    int db=0,i=0;
    long end;
    char answer[4+1];
    while (game->tries != 0) {
        scanf("\n");
        scanf(" %4s%*[^\n]", answer);
        while (answer[i] != '\0') {
            answer[i] = toupper(answer[i]);
            i++;
        }
        i = 0;
        if (strcmp(game->sor->valasz, answer) == 0) {     //ha helyes a válasz
            printf("Helyes valasz!\n");
            printf("\nJohetnek a felelet valasztos kerdesek!\n");
            felszabadit2(game->sor);
            game->sor = NULL;
            game->skerdes = false;
            game->tries = 0;
            game->money += 500000;
        }
        else if (strcmp(K, answer) == 0) {    //ha kilép a játékból
            printf("Felbehagyasa a jateknak!\n");
            end = time(0) + (game->min * 60) + game->sec;
            timer(game, (end - *start));
            int mentes = save(*game);
            if (mentes == 0)
                printf("Sikeres mentes!\n");
            else
                printf("Sikertelen mentes! Hibakod: %d\n", mentes);
            return true;
        }
        else if (game->tries > 1) {
            while (game->sor->valasz[i] != '\0') {
                if (answer[i] == game->sor->valasz[i]) {
                    db++;
                }
                i++;
            }
            i = 0;
            if (db > 0)
                printf("%d darabot eltalaltal!\n", db);
            else
                printf("Rossz valasz! Probald ujra!\n");
            game->tries--;
            printf("Maradek probalkozasok szama: %d\n", game->tries);
            db = 0;
        }
        else {   //ha rossz a válasz
            end = time(0) + (game->min * 60) + game->sec;
            timer(game, (end - *start));
            printf("Sajnos rossz a valaszod! Sok sikert legkozelebb :)\n");
            printf("A helyes sorrend: %s\n", game->sor->valasz);
            remove("save.txt");
            return true;
        }
    }
    return false;
}

int part_fv(Milliomos *game, const long *start, int *i, const long *winnings){
    char v;
    long end;
    bool felezett = false, szavazott = false;
    printf("\n%d. Kerdes ",game->fv->id);
    feloszt(1000*winnings[*i]);
    printf("Ft-ert: %s\n",game->fv->kerdes);
    printf("A: %s | B: %s | C: %s | D: %s\n",game->fv->v_A,game->fv->v_B,game->fv->v_C,game->fv->v_D);
    scanf("\n");
    scanf(" %c%*[^\n]",&v);
    v = toupper(v);
    while (v == 'F' || v == 'S'){
        if (v == 'F'){
            if (game->felezes){
                felezes(game);
                felezett = true;
            }
            else
                printf("Mar elhasznaltad a felezest!\n");
            scanf("\n");
            scanf(" %c%*[^\n]",&v);
            v = toupper(v);
        }
        else if (v == 'S'){
            if (game->kozonseg){
                kozonseg(game);
                szavazott = true;
            }
            else
                printf("Mar elhasznaltad a kozonseg szavazast!\n");
            scanf("\n");
            scanf(" %c%*[^\n]",&v);
            v = toupper(v);
        }
    }
    if (v == game->fv->valasz){     //ha helyes a válasz
        printf("Helyes valasz!\n");
        if (!felezett && !szavazott) {
            game->money += winnings[*i];
        }
        else if (felezett && szavazott){
            game->money += winnings[*i] / 8;
            felezett = false;
            szavazott = false;
        }
        else if(felezett){
            game->money += winnings[*i] / 2;
            felezett = false;
        }
        else if(szavazott){
            game->money += winnings[*i] / 4;
            szavazott = false;
        }
        *i += 1;
        game->fv = lista_torles(game->fv,*i);
    }
    else if (v == 'K'){     //ha kilép a játékból
        printf("Felbehagyasa a jateknak!\n");
        end = time(0) + (game->min * 60) + game->sec;
        timer(game,(end - *start));
        int mentes = save(*game);
        if (mentes == 0)
            printf("Sikeres mentes!\n");
        else
            printf("Sikertelen mentes! Hibakod: %d\n",mentes);
        return 1;
    }
    else{   //ha rossz a válasz
        end = time(0) + (game->min * 60) + game->sec;
        timer(game,(end - *start));
        printf("Sajnos rossz a valaszod! Sok sikert legkozelebb :)\n");
        printf("A helyes valasz: %c\n",game->fv->valasz);
        remove("save.txt");
        return 2;
    }
    return 0;
}

void play(Milliomos *game){
    long start = time(0), end;
    Score current;
    printf("\nHa a jatek soran barmikor felbe szeretned hagyni a jatekot nyomj egy (K)ilepest!\n");
    /*Sorkérdés része a játéknak*/
    if (game->sor != NULL){
        if (part_sorkerdes(game,&start)){
            felszabadit1(game->fv);
            felszabadit2(game->sor);
            return;
        }
    }
    /*Felelet választós része a játéknak*/
    printf("\n2 segitseget tudsz felhasznalni a jatek soran.\n");
    printf("Az egyik a (F)elezes, masik pedig a kozonseg (S)zavazas.\n");
    int j = 0, i = game->fv->id - 1;
    long nyeremenyek[] = {10000,20000,50000,100000,250000,500000,750000,1000000,1500000,2000000,5000000,10000000,15000000,25000000,50000000};
    while (game->fv != NULL && j != 2){
        j = part_fv(game,&start,&i,nyeremenyek);
        if (j == 1){
            felszabadit1(game->fv);
            if (game->sor != NULL) {
                felszabadit2(game->sor);
            }
            return;
        }
    }

    /*Dicsőséglistára felírás, ha felfér*/
    end = time(0) + (game->min * 60) + game->sec;
    timer(game,(end-start));
    current.score = (game->money - (end-start)) * game->diff;
    printf("\nA jatek soran szerzett pontjaid: ");
    feloszt(1000*current.score);
    printf("\n");
    if (current.score >= 0){
        current.diff = game->diff;
        current.min = game->min;
        current.sec = game->sec;
        scoreboard(current);
    }

    if (game->fv == NULL) {
        printf("\nGratulalok, megnyerted a jatekot!\n");
    }

    felszabadit1(game->fv);    //felszabadítása a játékban használt dinamikus memóriaterületeknek
    if (game->sor != NULL){
        felszabadit2(game->sor);
    }
}
