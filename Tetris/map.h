#ifndef MAP_H
#define MAP_H
#include "hand.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//Mátrixszerű pointer elérés egyszerűsítése
#define IND(x,y,oszlop) ((x) * oszlop + (y))

//Pálya struktúra
typedef struct {
	bool e; //Cellában van-e bármi?
	int c; //Mi a szine?
} Map;

//Memoria terulet lefoglalasa a pályának
Map* MapFoglal(int const *sor, int const *oszlop);

//A pálya teljes lenullázása
Map* MapNullaz(Map* tomb, int const *sor, int const *oszlop);

//Tetris átmásolása a pályára
Map* MapbaMasol(Map* tomb, int const * oszlop, Hand* const h);

#endif