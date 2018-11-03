#include "map.h"


//Memoria terulet lefoglalasa a pályának
Map* MapFoglal(int const *sor, int const *oszlop) {
	return (Map*)malloc((*sor) * (*oszlop) * sizeof(Map));
}

Map* MapNullaz(Map* tomb, int const *sor, int const *oszlop) {
	for (int i = 0; i < *sor; i++)
		for (int j = 0; j < *oszlop; j++) {
			tomb[IND(i, j, *oszlop)].e = 0;
			tomb[IND(i, j, *oszlop)].c = 0;
		}
	return tomb;
}

Map* MapbaMasol(Map* tomb, int const *oszlop, Hand* const h) {
	for (int i = 0; i < h->size; i++)
		for (int j = 0; j < h->size; j++) {
			if (h->e[IND(i, j, h->size)]) {
				tomb[IND(h->x + i, h->y + j, *oszlop)].e = true;
				tomb[IND(h->x + i, h->y + j, *oszlop)].c = h->color;
			}
		}
	
	return tomb;
}

