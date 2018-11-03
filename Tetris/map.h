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
} PalyaMatrix;

typedef struct {
	int sor, oszlop;
	int *sum;
	PalyaMatrix *v;
} Palya;

//Memoria terulet lefoglalasa a pályának
PalyaMatrix* MatrixFoglal(Palya const *vp);

//A pálya teljes lenullázása
void MatrixInit(Palya *vp, int const sor, int const oszlop);

//Tetris átmásolása a pályára
void MatrixbaMasol(Palya *vp, Hand *hp);

bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp);

#endif