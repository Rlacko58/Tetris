#ifndef RANGLISTA_H
#define RANGLISTA_H

#include "map.h"

//Ranglista feltöltése fájlból
void Ranglistabeolvas(Palya *vp);

//Ranglistához hozzáadás
void Ranglistahozaad(Palya *vp);

//Ranglista rendezése
void RanglistaRendez(Palya *vp);

//Ranglista mentése
void Ranglistament(Palya *vp);

//Játékos nevének módosítása
void Nevhezir(Palya *vp, int c);
void NevbolTorol(Palya *vp);

#endif