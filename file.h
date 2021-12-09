#ifndef MILLIOMOS_FILE_H
#define MILLIOMOS_FILE_H

FelValasz *read_felvalasz(FILE*);   //felelet-választós kérdések beolvasása az adott fájlból
int counter_sorkerdes(void);    //megszámolja hány soros a sorkérdéseket tartalmazó fájl
void scan_sorkerdes(FILE *fp, Sorkerdes *first); //kiolvas egy sorkérdés típusú sort a fájlból
Sorkerdes *read_sorkerdes (const FelValasz*); //sorkérdések fájlból random kiválaszt egy kérdést és azt kiolvassa
int save(Milliomos);   //menti a játékmenetet egy .txt fájlba
void scan_save_felvalasz(FILE*, Milliomos*);    //a mentésolvasás segédfüggvénye, amely a felelet-választós kérdéseket olvassa be
Milliomos read_save(void); //kiolvassa a mentett játékmenetet a save.txt fájlból
int hasonlit(void const *, void const *);   //összehasonító függvény
void scoreboard(Score); //beírja a dicsőséglistába az elért eredményt, ha felkerülhet a listába
void scoreboard_kiir(void); //kiírja a dicsőséglista elemeit

#endif
