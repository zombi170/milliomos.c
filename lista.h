#ifndef MILLIOMOS_LISTA_H
#define MILLIOMOS_LISTA_H

typedef struct FelValasz{   //felelet-választós kérdések struktúrája
    int id;
    int lvl;
    char *kerdes;
    char *v_A;
    char *v_B;
    char *v_C;
    char *v_D;
    char valasz;
    char *kategoria;
    struct FelValasz *next;
}FelValasz;

typedef struct Sorkerdes{   //sorkérdések struktúrája
    char *kerdes;
    char *v_A;
    char *v_B;
    char *v_C;
    char *v_D;
    char valasz[4+1];
    char *kategoria;
}Sorkerdes;

typedef struct Milliomos{   //játék struktúrája
    int tries;
    int sec;
    int min;
    long money;
    int diff;
    FelValasz *fv;
    Sorkerdes *sor;
    bool skerdes;
    bool felezes;
    bool kozonseg;
}Milliomos;

typedef struct Nehezseg{    //nehézségi szint struktúrája
    int easy;
    int mid;
    int hard;
}Nehezseg;

typedef struct Score{   //dicsőséglista struktúrája
    char *name;
    int sec;
    int min;
    long score;
    int diff;
}Score;

FelValasz *lista_hozzafuz(FelValasz*, FelValasz*);    //hozzáfűz egy felelet-választós kérdést a láncolt listához
char *dintomb_foglal(char*);    //dinamikusan foglal sztringnek memóriaterületet, melyre mutató pointert visszaadja, illetve megtölti a megadott tartalommal
void lista_bejar(FelValasz*, int);     //végigmegy a lista összes elemén és megindexeli az utolsót az adott indexszel
FelValasz *lista_keres(FelValasz*, int, int, int);    //megkeres egy adott indexű és nehézségű elemet a listában
FelValasz *lista_torles(FelValasz*, int); //kikeres egy adott indexű listaelemet és kitörli a listából, mindig visszaadja a lista kezdeti elemét
void lista_rendez_kategoria(FelValasz*);  //rendezi a listát az alapján, hogy egymás után nem következhet ugyanaz a kategória
void lista_indexel(FelValasz*);     //megindexeli a láncolt lista elemeit
void felszabadit1(FelValasz*);    //felszabadítja a felelet választós kérdésekhez dinamikusan lefoglalt memóriaterületeket
void felszabadit2(Sorkerdes*); //felszabadítja a sorkérdésekhez dinamikusan lefoglalt memóriaterületeket

#endif
