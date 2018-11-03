#ifndef MAP_H
#define MAP_H
#include "hand.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define IND(x,y,oszlop) ((x) * oszlop + (y))

typedef struct {
	bool e; //Cellában van-e bármi?
	int c; //Mi a szine?
} Map;

Map* MapFoglal(int const *sor, int const *oszlop);

Map* MapNullaz(Map* tomb, int const *sor, int const *oszlop);

Map* MapbaMasol(Map* tomb, int const * oszlop, Hand* const h);

#endif