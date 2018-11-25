#include "hand.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//Debughoz
#include "debugmalloc.h"

//A tetrisek lehetséges verziói
static bool I[16] = { 0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 };
static bool T[9] = { 1,1,1,0,1,0,0,0,0 };
static bool L[9] = { 1,1,1,1,0,0,0,0,0 };
static bool J[9] = { 1,1,1,0,0,1,0,0,0 };
static bool S[9] = { 0,1,1,1,1,0,0,0,0 };
static bool Z[9] = { 1,1,0,0,1,1,0,0,0 };
static bool O[4] = { 1,1,1,1 };

//Lefoglalja a tetris mátrixát
bool* HandFoglal(Hand* hp) {
	return (bool*)malloc((hp->size)*(hp->size) * sizeof(bool));
}

//Kézben lévő tetris inicializáció
void HandInit(Hand* hp,int const *oszlop, int const melyik) {
	if (melyik == 0) hp->size = 4;
	else if (melyik <= 5) hp->size = 3;
	else hp->size = 2;

	hp->x = 0;
	hp->y = (int)*oszlop / 2 - ceil(hp->size/2);

	hp->v = HandFoglal(hp);
	switch (melyik) {
		case 0: hp->color = 1; hp->x = -1;  for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = I[i]; break;
		case 1: hp->color = 2;	for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = T[i]; break;
		case 2: hp->color = 3;	for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = L[i]; break;
		case 3: hp->color = 4;	for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = J[i]; break;
		case 4: hp->color = 5;	for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = S[i]; break;
		case 5: hp->color = 6;	for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = Z[i]; break;
		default: hp->color = 7; for (int i = 0; i < hp->size*hp->size; i++) hp->v[i] = O[i]; break;
	}
}

//Elforgatja a kézben lévő tetrist és visszatér annak mátrixával
bool* Forgat_jobbra(Hand const *hp) {  //Jobbra forgat
	bool* sv = HandFoglal(hp);
	if (hp->size == 2) {
		for (int i = 0; i < 4; i++) sv[i] = true;
		return sv;
	}
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++) {
			sv[IND(j, hp->size - 1 - i,hp->size)] = hp->v[IND(i, j, hp->size)];
		}
	return sv;
}
bool* Forgat_balra(Hand const *hp) {  //Balra forgat
	bool* sv = HandFoglal(hp);
	if (hp->size == 2) {
		for (int i = 0; i < 4; i++) sv[i] = true;
		return sv;
	}
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++) {
			sv[IND(j, i, hp->size)] = hp->v[IND(i, hp->size - 1 - j, hp->size)];
		}
	return sv;
}


