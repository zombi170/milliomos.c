#ifndef MILLIOMOS_JATEK_H
#define MILLIOMOS_JATEK_H

Nehezseg valaszt(Milliomos*);  //nehézségi szint kiválasztása
int sorkerdes(Milliomos*);  //sorkérdés választás
void trim(char *kerdes);    //kiszedi a felesleges idézőjeleket a kérdésből
FelValasz *jatek_hozzafuz(FelValasz*, FelValasz*); //a játék listáját állítja össze nehézségi sorrendbe rendezve
FelValasz *generalas(FelValasz**, Nehezseg, int); //legenerálja random a játék során használt kérdéseket
int min(int, int);  //visszaadja a két szám közül a kisebbet
void felezes(Milliomos*);   //felezés segítség
void kozonseg(Milliomos*);  //közönség szavazás segítség
void timer(Milliomos*, long);   //átváltja az eltelt időt percekre és másodpercekre
void feloszt(long sz); //felosztja hármasával a számjegyeket
bool part_sorkerdes(Milliomos*, const long*);    //sorkérdéses része a játéknak
int part_fv(Milliomos*, const long*, int*, const long*);  //felelet-választós része a játéknak
void play(Milliomos*); //játék menetét végigvívő függvény

#endif
