#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "lista.h"
#include "file.h"
#include "jatek.h"
#include "debugmalloc.h"

int main() {
    srand(time(0));
    char s = 'z';
    Milliomos jatek;
    int hiba;
    bool siker;
    /*Főmenü*/
    while (s != EOF){
        printf("\nLegyen On is Milliomos!\n\n");
        printf("1. (U)j jatek\n");
        printf("2. Jatek (f)olytatasa\n");
        printf("3. (D)icsoseglista\n");
        printf("(K)ilepes\n");
        scanf("\n");
        scanf(" %c%*[^\n]",&s);
        switch (s){
            case '1':
            case 'U':
            case 'u':
                printf("Uj jatek kezdete!\n");

                /*A felelet-választós fájl megnyitása*/
                FILE *fp;
                fp = fopen("felelet_valasztos.csv","rt");
                if (fp == NULL){
                    printf("Nem sikerult a fajlt beolvasni!\n");
                    return -1;
                }

                /*Lista létrehozása felelet-választós kérdésekből*/
                FelValasz *new = read_felvalasz(fp);
                if (new == NULL){
                    printf("Ures a fajl!\n");
                    return -2;
                }
                FelValasz *eleje = NULL;
                eleje = lista_hozzafuz(eleje,new);
                int darab=0;
                while (new != NULL){
                    new = read_felvalasz(fp);
                    lista_hozzafuz(eleje,new);
                    darab++;
                }

                if (darab < 15){
                    printf("Nincs elegendo kerdes a fajlban!\n");
                    return -3;
                }

                Nehezseg difficulty = valaszt(&jatek);    //nehézségi szint kiválasztása

                jatek.fv = generalas(&eleje,difficulty,darab); //random játék generálása

                felszabadit1(eleje);    //felszabadítása a játék felépítéséhez használt dinamikus memóriaterületeknek

                lista_rendez_kategoria(jatek.fv);
                lista_indexel(jatek.fv);

                hiba = sorkerdes(&jatek); //sorkérdés kiválasztása az adott fáljból
                if (hiba == -4){
                    return hiba;
                }

                jatek.felezes = true;
                jatek.kozonseg = true;
                jatek.min = 0;
                jatek.sec = 0;
                jatek.money = 0;

                hiba = save(jatek);   //menti a kész játékot
                if (hiba == 0)
                    printf("Sikeres mentes!\n");
                else{
                    printf("Sikertelen mentes!\n");
                    return hiba;
                }

                play(&jatek);  //játék elkezdése
                break;

            case '2':
            case 'F':
            case 'f':
                printf("\nMegkezdett jatek folytatasa!\n");
                jatek = read_save();    //játék beolvasása a save.txt fájlból
                if (jatek.fv == NULL){
                    printf("Hiba tortent a save.txt fajl beolvasasakor!\n");
                    return -6;
                }

                play(&jatek);  //játék folytatása
                break;

            case '3':
            case 'D':
            case 'd':
                siker = false;
                while (!siker) {
                    printf("1. (K)iir\n");
                    printf("2. (T)orles\n");
                    scanf("\n");
                    scanf(" %c%*[^\n]",&s);
                    switch (s) {
                        case '1':
                        case 'K':
                        case 'k':
                            scoreboard_kiir();
                            siker = true;
                            break;
                        case '2':
                        case 'T':
                        case 't':
                            printf("\nBiztos torolni szeretned a dicsoseglistat?\n");
                            printf("1. (I)gen\n");
                            printf("2. (N)em\n");
                            scanf("\n");
                            scanf(" %c%*[^\n]",&s);
                            switch (s) {
                                case '1':
                                case 'I':
                                case 'i':
                                    remove("dicsoseglista.txt");
                                    printf("Sikeresen torolted a dicsoseglistat!\n");
                                    break;
                                case '2':
                                case 'N':
                                case 'n':
                                    break;
                                default :
                                    printf("Ervenytelen bevitel!\n");
                                    break;
                            }
                            siker = true;
                            break;
                        default :
                            printf("Ervenytelen bevitel!\n");
                            break;
                    }
                }
                break;

            case 'K':
            case 'k':
                printf("\nJatek bezarasa!\n");
                return 0;

            default :
                printf("Ervenytelen bevitel!\n");
                break;
        }
    }
    return 0;
}
