#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lista.h"
#include "jatek.h"
#include "file.h"
#include "debugmalloc.h"

FelValasz *read_felvalasz(FILE *fp){
    char temp[1000];
    FelValasz *uj = (FelValasz*) malloc(sizeof(FelValasz));
    if (uj == NULL){
        printf("Nem sikerult a dinamikus foglalas!\n");
        return NULL;
    }

    if (fscanf(fp,"%d;",&uj->lvl) != EOF){  //nehézségi szint beolvasása
        fscanf(fp,"%[^;]",temp);
        uj->kerdes = dintomb_foglal(temp);  //kérdés beolvasása
        fscanf(fp,"%c",temp);   //pontosvessző elhagyása

        fscanf(fp,"%[^;]",temp);
        uj->v_A = dintomb_foglal(temp); //A válasz beolvasása
        fscanf(fp,"%c",temp);

        fscanf(fp,"%[^;]",temp);
        uj->v_B = dintomb_foglal(temp); //B válasz beolvasása
        fscanf(fp,"%c",temp);

        fscanf(fp,"%[^;]",temp);
        uj->v_C = dintomb_foglal(temp); //C válasz beolvasása
        fscanf(fp,"%c",temp);

        fscanf(fp,"%[^;]",temp);
        uj->v_D = dintomb_foglal(temp); //D válasz beolvasása
        fscanf(fp,"%c",temp);

        fscanf(fp,"%c;",&uj->valasz);   //helyes válasz beolvasása

        fscanf(fp,"%s",temp);
        uj->kategoria = dintomb_foglal(temp);   //kategória beolvasása
    }
    else{
        fclose(fp);
        free(uj);
        return NULL;
    }
    return uj;
}

int counter_sorkerdes(void){
    /*Sorkérdések fájl megnyitása*/
    FILE *fp;
    fp = fopen("sorkerdesek.csv","rt");
    if (fp == NULL){
        printf("Nem sikerult a fajlt beolvasni!\n");
        return -1;
    }

    int db=1, c;
    while ((c = fgetc(fp)) != EOF){ //sorok megszámlálása a fájlban
        if (c == 10)
            db++;
    }
    fclose(fp);
    return db;
}

void scan_sorkerdes(FILE *fp, Sorkerdes *first){
    char temp[1000];
    fscanf(fp, "%[^;]", temp);
    first->kerdes = dintomb_foglal(temp);    //kérdés beolvasása
    fscanf(fp,"%c",temp);

    fscanf(fp, "%[^;]", temp);
    first->v_A = dintomb_foglal(temp);   //A válasz beolvasása
    fscanf(fp,"%c",temp);

    fscanf(fp, "%[^;]", temp);
    first->v_B = dintomb_foglal(temp);   //B válasz beolvasása
    fscanf(fp,"%c",temp);

    fscanf(fp, "%[^;]", temp);
    first->v_C = dintomb_foglal(temp);   //C válasz beolvasása
    fscanf(fp,"%c",temp);

    fscanf(fp, "%[^;]", temp);
    first->v_D = dintomb_foglal(temp);   //D válasz beolvasása
    fscanf(fp,"%c",temp);

    fscanf(fp, "%[^;]", first->valasz);  //helyes válasz beolvasása
    first->valasz[4] = '\0';
    fscanf(fp,"%c",temp);

    fscanf(fp, "%s", temp);
    first->kategoria = dintomb_foglal(temp); //kategória beolvasása
}

Sorkerdes *read_sorkerdes(const FelValasz *start){
    int db = counter_sorkerdes();
    if (db == -1)
        return NULL;
    /*Sorkérdések fájl megnyitása*/
    FILE *fp;
    fp = fopen("sorkerdesek.csv","r");
    if (fp == NULL){
        printf("Nem sikerult a fajlt beolvasni!\n");
        return NULL;
    }

    Milliomos sub;
    sub.sor = NULL;

    sub.sor = (Sorkerdes*) malloc(sizeof(Sorkerdes));
    if (sub.sor == NULL){
        printf("Nem sikerult a dinamikus foglalas!\n");
        return NULL;
    }
    /*Random sor kiválasztása a fájlban*/
    char temp[1000];
    int melyik = rand()%db+1;
    for(int i=0; i < melyik; i++){
        fgets(temp, sizeof(temp), fp);
    }
    scan_sorkerdes(fp,sub.sor);
    while (strcmp(start->kategoria,sub.sor->kategoria) == 0){
        felszabadit2(sub.sor);
        sub.sor = (Sorkerdes*) malloc(sizeof(Sorkerdes));
        if (sub.sor == NULL){
            printf("Nem sikerult a dinamikus foglalas!\n");
            return NULL;
        }
        scan_sorkerdes(fp,sub.sor);
    }
    fclose(fp);
    return sub.sor;
}

int save(const Milliomos game){
    FILE *fp;
    fp = fopen("save.txt","wt");
    if (fp == NULL){
        printf("Nem sikerult a fajlt letrehozni!\n");
        return -5;
    }

    fprintf(fp,"MILLIOMOS V1.0\n");
    if (game.skerdes){
        fprintf(fp,"%d;%d;%d;%d;%d;%d;%ld;%d\n",game.skerdes,game.tries,game.felezes,game.kozonseg,game.min,game.sec,game.money,game.diff);
        fprintf(fp,"%s;%s;%s;%s;%s;%s;%s\n",game.sor->kerdes,game.sor->v_A,game.sor->v_B,game.sor->v_C,game.sor->v_D,game.sor->valasz,game.sor->kategoria);
    }
    else
        fprintf(fp,"%d;%d;%d;%d;%d;%d;%ld;%d\n",game.skerdes,game.tries,game.felezes,game.kozonseg,game.min,game.sec,game.money,game.diff);
    FelValasz *move;
    for (move = game.fv; move != NULL; move = move->next){
        fprintf(fp,"%d;%d;%s;%s;%s;%s;%s;%c;%s\n",move->id,move->lvl,move->kerdes,move->v_A,move->v_B,move->v_C,move->v_D,move->valasz,move->kategoria);
    }
    fclose(fp);
    return 0;
}

void scan_save_felvalasz(FILE *fp, Milliomos *game){
    int idx;
    FelValasz *new;
    fscanf(fp, "%d;", &idx);
    new = read_felvalasz(fp);
    if (new == NULL){
        printf("Ures a fajl!\n");
        return;
    }
    game->fv = lista_hozzafuz(game->fv,new);
    lista_bejar(game->fv,idx);
    while (fscanf(fp, "%d;", &idx) != EOF){
        new = read_felvalasz(fp);
        lista_hozzafuz(game->fv,new);
        lista_bejar(game->fv,idx);
    }
}

Milliomos read_save(void){
    Milliomos game;
    game.fv = NULL;
    FILE *fp;
    fp = fopen("save.txt","rt");
    if (fp == NULL){
        printf("Nem sikerult a fajlt beolvasni!\n");
        return game;
    }

    char temp[1000];
    fgets(temp, sizeof(temp), fp);
    fscanf(fp,"%d;%d;%d;%d;%d;%d;%ld;%d\n",&game.skerdes,&game.tries,&game.felezes,&game.kozonseg,&game.min,&game.sec,&game.money,&game.diff);
    if (game.skerdes){
        Sorkerdes *first = (Sorkerdes*) malloc(sizeof(Sorkerdes));
        game.sor = first;
        scan_sorkerdes(fp,game.sor);
        scan_save_felvalasz(fp,&game);
    }
    else if (!game.skerdes){
        game.sor = NULL;
        scan_save_felvalasz(fp,&game);
    }
    else
        printf("Hibas a save.txt fajl!\n");
    fclose(fp);
    return game;
}

int hasonlit(void const *vp1, void const *vp2) {
    Score const *sz1 = (Score const *) vp1;
    Score const *sz2 = (Score const *) vp2;
    if (sz1->score > sz2->score) return +1;
    if (sz1->score < sz2->score) return -1;
    return 0;
}

void scoreboard(const Score game){
    FILE *fp;
    fp = fopen("dicsoseglista.txt","rt");
    /*Ha még nem létezne a dicsőséglista fájl*/
    if (fp == NULL){
        fp = fopen("dicsoseglista.txt","wt");
        if (fp == NULL){
            printf("Nem sikerult a fajlt letrehozni!\n");
            return;
        }
        fprintf(fp,"DICSOSEGLISTA v1.0\n");
        fclose(fp);
        fp = fopen("dicsoseglista.txt","rt");
    }
    if (fp == NULL){
        printf("Nem sikerult a fajlt beolvasni!\n");
        return;
    }

    int i=0;
    char temp[1000];
    Score list[10];
    fgets(temp, sizeof(temp), fp);
    /*Beolvasása a fájlban található adatoknak*/
    while (fscanf(fp,"%ld;",&list[i].score) != EOF && i!=10){
        fscanf(fp,"%[^;]",temp);
        list[i].name = dintomb_foglal(temp);
        fscanf(fp,"%c",temp);
        fscanf(fp,"%d;%d;%d\n",&list[i].diff,&list[i].min,&list[i].sec);
        i++;
    }
    /*Ha még nem lenne meg a top 10 ember*/
    if (i < 10){
        printf("\nAdd meg a neved (egy szavas): ");
        scanf(" %s%*[^\n]",temp);
        list[i].name = dintomb_foglal(temp);
        list[i].score = game.score;
        list[i].diff = game.diff;
        list[i].min = game.min;
        list[i].sec = game.sec;
        qsort(list,i+1,sizeof(Score),hasonlit);
        printf("\nFelkerültél a dicsőséglistára!\n");
    }
    /*Ha már van top 10*/
    else if (list[i-1].score < game.score){
        free(list[i-1].name);
        printf("\nAdd meg a neved (egy szavas): ");
        scanf(" %s%*[^\n]",temp);
        list[i-1].name = dintomb_foglal(temp);
        list[i-1].score = game.score;
        list[i-1].diff = game.diff;
        list[i-1].min = game.min;
        list[i-1].sec = game.sec;
        qsort(list,i,sizeof(Score),hasonlit);
        printf("\nFelkerültél a dicsőséglistára!\n");
    }
    fclose(fp);

    fp = fopen("dicsoseglista.txt","wt");
    if (fp == NULL){
        printf("Nem sikerult a fajlt letrehozni!\n");
        return;
    }
    /*Kiírása az új dicsőséglistának*/
    fprintf(fp,"DICSOSEGLISTA v1.0\n");

    if (i > 9){
        i--;
    }

    while (i >= 0){
        fprintf(fp,"%ld;%s;%d;%d;%d\n",list[i].score,list[i].name,list[i].diff,list[i].min,list[i].sec);
        free(list[i].name);
        i--;
    }
    fclose(fp);
}

void scoreboard_kiir(void){
    FILE *fp;
    fp = fopen("dicsoseglista.txt","rt");
    if (fp == NULL){
        printf("\nNincs meg senki benne a dicsoseglistaban!\n");
        return;
    }

    printf("\nDicsoseglista:\n");
    int i=0;
    char temp[1000];
    Score list[10];
    fgets(temp, sizeof(temp), fp);
    while (fscanf(fp,"%ld;",&list[i].score) != EOF && i!=10){
        fscanf(fp,"%[^;]",temp);
        list[i].name = dintomb_foglal(temp);
        fscanf(fp,"%c",temp);
        fscanf(fp,"%d;%d;%d\n",&list[i].diff,&list[i].min,&list[i].sec);
        i++;
    }
    fclose(fp);

    for (int j=0; j < i; j++){
        printf("%d. Nev: %s\t",j+1,list[j].name);
        if (list[j].diff == 1){
            printf(" Nehezsegi szint: Kezdo\t");
        }
        else if (list[j].diff == 2){
            printf(" Nehezsegi szint: Normal\t");
        }
        else if (list[j].diff == 3){
            printf(" Nehezsegi szint: Profi\t");
        }
        printf(" Pont: ");
        feloszt(list[j].score * 1000);
        printf("\t Ido: %0.2d:%0.2d\n",list[j].min,list[j].sec);
        free(list[j].name);
    }
}
