﻿#include "map.h"

#include <stdlib.h>
#include <stdio.h>

//Debughoz
#include "debugmalloc.h"


//Memoria terulet lefoglalasa a pályának
PalyaMatrix* MatrixFoglal(Palya const *vp) {
	return (PalyaMatrix*)malloc((vp->sor) * (vp->oszlop) * sizeof(PalyaMatrix));
}

//A pálya inicializálása
void MatrixInit(Palya *vp, int const sor, int const oszlop) {
	vp->sor = sor; vp->oszlop = oszlop;
	vp->Nsize = vp->width = vp->height  = 0;
	vp->v = MatrixFoglal(vp);
	vp->level = 1;
	vp->sum = (int*)malloc(vp->sor * sizeof(int));
	for (int i = 0; i < vp->sor; i++) {
		vp->sum[i] = 0;
		for (int j = 0; j < vp->oszlop; j++) {
			vp->v[IND(i, j, vp->oszlop)].e = 0;
			vp->v[IND(i, j, vp->oszlop)].c = 0;
		}
	}
}

//Tetris átmásolása a pályára
void MatrixbaMasol(Palya *vp, Hand *hp) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (hp->v[IND(i, j, hp->size)]) {
				vp->v[IND(hp->x + i, hp->y + j, vp->oszlop)].e = true;
				vp->v[IND(hp->x + i, hp->y + j, vp->oszlop)].c = hp->color;
			}
}
//Ütközés vizsgálat x és y eltolással bp tömbre
bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp, int x, int y) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (bp[IND(i, j, hp->size)]) {
				if ( ((vp->v[IND(hp->x + i + x, hp->y + j + y, vp->oszlop)].e) && (hp->x+i>=0)) ||
					hp->y + j + y >= vp->oszlop || hp->y + j + y < 0 ||
					hp->x + x >= vp->sor)
					return true;
			}
	return false;
}

int AltetrisKord(Palya const *vp, Hand const *hp) {
	int x = 1;
	while (!Utkozes(vp, hp, &hp->v[0], x, 0) && x<=vp->sor) {
		x++;
	}
	return x-1;
}

//Adott sor eltüntetése, majd fölötte lévők lejebb húzása
void Eltuntet_sor(Palya *vp, int sor) {
	vp->sum[sor] = 0;
	for (int i = 0; i < vp->oszlop; i++) {
		for (int j = sor; j > 0; j--) {
			vp->v[IND(j, i, vp->oszlop)].e = vp->v[IND(j-1, i, vp->oszlop)].e; 
			vp->v[IND(j - 1, i, vp->oszlop)].e = 0;
			vp->v[IND(j, i, vp->oszlop)].c = vp->v[IND(j-1, i, vp->oszlop)].c;
			vp->v[IND(j - 1, i, vp->oszlop)].c = 0;
		}
		if (vp->v[IND(i, sor, vp->oszlop)].e) vp->sum[sor]++;
	}
}