#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lista.h"
#include "debugmalloc.h"

FelValasz *lista_hozzafuz(FelValasz *start, FelValasz *data){
    if (data != NULL){
        FelValasz *uj;

        uj = data;    //rámutat az új listaelemmel a lánc következő elemére
        uj->next = NULL;    //mivel a végére visszük a listának, ezért még nem mutat következő elemre

        if (start == NULL){     //ha még nem létezne a lista kezdeti eleme
            uj->id = 1;
        }
        else{
            FelValasz *move = start;
            while (move->next != NULL)  //elmegy a lista végére
                move = move->next;
            uj->id = move->id + 1;  //megindexeli a listaelemet
            move->next = uj;    //rámutat az új listaelemre
        }
        return uj;
    }
    else
        return NULL;
}

char *dintomb_foglal(char *str){
    char *p = (char*) malloc((strlen(str) + 1) * sizeof(char));
    if (p == NULL){
        printf("Nem sikerult a dinamikus foglalas!\n");
        return NULL;
    }

    strcpy(p,str);
    p[strlen(str)] = '\0';
    return p;
}

void lista_bejar(FelValasz* start, int id){
    FelValasz *move = start;
    while (move->next != NULL){
        move = move->next;
    }
    move->id = id;
}

FelValasz *lista_keres(FelValasz *start, const int idx, const int min, const int max){
    FelValasz *move = start;
    while (move != NULL && move->id != idx){    //keresése az adott elemnek
        move = move->next;
    }
    int r = rand()%(max-min)+1;
    if (r < min){
        r += min;
    }
    while (move != NULL && move->lvl != r){   //keresése az adott nehézségnek
        move = move->next;
    }
    return move;
}

FelValasz *lista_torles(FelValasz *start, const int idx){
    FelValasz *inchworm = NULL;
    FelValasz *move = start;
    while (move != NULL && move->id != idx) {    //keresése az adott elemnek
        inchworm = move;
        move = move->next;
    }
    /*A törlés különböző estei*/
    if (move == NULL){}           //ha nincs ilyen elem akkor nincs teendő
    else if (move == start) {     //ha az első elem a törlendő
        FelValasz *newstart = move->next;
        move->next = NULL;
        felszabadit1(move);
        start = newstart;
    }
    else {                        //egyébként a megtalált elem törlése
        inchworm->next = move->next;
        move->next = NULL;
        felszabadit1(move);
    }
    return start;
}

void lista_rendez_kategoria(FelValasz *start){
    bool csere = true;
    FelValasz *inchworm = NULL, *move = NULL, *kov = NULL, *temp = NULL, *last = NULL;
    int i=0; //azért kell, hogy a nagyon kis százalékú eseteket kiküszöbölje, amikor a csillagok is úgy állnak össze, hogy nem éri meg kategória szerint is rendezni, mert végtelen lesz a ciklus
    while (csere == true && i < 1000){  //addig vizsgálunk, amíg nem kell többet cserélni, mert akkor már jó
        csere = false;
        inchworm = start;
        move = start->next;
        kov = move->next;
        while (kov != NULL) {
            /*Ha ugyanaz a kategória van egymás mellett -> csere*/
            if (strcmp(inchworm->kategoria, move->kategoria) == 0){
                csere = true;
                temp = inchworm->next;
                inchworm->next = move->next;
                move->next = kov->next;
                kov->next = temp;
                temp = move;
                move = kov;
                kov = temp;
            }
            move = move->next;
            kov = kov->next;
            inchworm = inchworm->next;
        }
        /*Utolsó két elem egyezésének lekezelése*/
        while (strcmp(inchworm->kategoria, move->kategoria) == 0){
            csere = true;
            move = start;
            while (move != NULL && move->next != inchworm){
                move = move->next;
            }
            last = start;
            while (last != NULL && last->next != move){
                last = last->next;
            }
            temp = move->next;
            move->next = inchworm->next;
            inchworm->next = last->next;
            last->next = temp;
            temp = move;
            move = inchworm;
            inchworm = temp;
        }
        i++;
    }
}

void lista_indexel(FelValasz *start){
    FelValasz *move;
    int i=1;
    for (move = start; move != NULL; move = move->next){
        move->id = i;
        i++;
    }
}

void felszabadit1(FelValasz *start){
    FelValasz *szabad = start;
    FelValasz *next;
    /*A felelet-választós lista strutúráinak és benne lévő sztringjeinek lefoglalt terület felszabadítása*/
    while (szabad != NULL){
        next = szabad->next;
        free(szabad->kerdes);
        free(szabad->v_A);
        free(szabad->v_B);
        free(szabad->v_C);
        free(szabad->v_D);
        free(szabad->kategoria);
        free(szabad);
        szabad = next;
    }
}

void felszabadit2(Sorkerdes *p){
    /*A sorkérdés struktúra és sztringjeinek lefoglalt terület felszabadítása*/
    if (p != NULL){
        free(p->kerdes);
        free(p->v_A);
        free(p->v_B);
        free(p->v_C);
        free(p->v_D);
        free(p->kategoria);
        free(p);
    }
}
