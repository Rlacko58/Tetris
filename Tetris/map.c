#include "map.h"

#include "debugmalloc.h"


//Memoria terulet lefoglalasa a p�ly�nak
PalyaMatrix* MatrixFoglal(Palya const *vp) {
	return (PalyaMatrix*)malloc((vp->sor) * (vp->oszlop) * sizeof(PalyaMatrix));
}

//A p�lya inicializ�l�sa
void MatrixInit(Palya *vp, int const sor, int const oszlop) {
	vp->sor = sor; vp->oszlop = oszlop;
	vp->v = MatrixFoglal(vp);
	vp->sum = (int*)malloc(vp->sor * sizeof(int));
	for (int i = 0; i < vp->sor; i++) {
		vp->sum[i] = 0;
		for (int j = 0; j < vp->oszlop; j++) {
			vp->v[IND(i, j, vp->oszlop)].e = 0;
			vp->v[IND(i, j, vp->oszlop)].c = 0;
		}
	}
}

//Tetris �tm�sol�sa a p�ly�ra
void MatrixbaMasol(Palya *vp, Hand *hp) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (hp->v[IND(i, j, hp->size)]) {
				vp->v[IND(hp->x + i, hp->y + j, vp->oszlop)].e = true;
				vp->v[IND(hp->x + i, hp->y + j, vp->oszlop)].c = hp->color;
			}
}
//�tk�z�s vizsg�lat
bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (bp[IND(i, j, hp->size)]) 
				if (vp->v[IND(hp->x + i, hp->y + j, vp->oszlop)].e)
					return true;
	return false;
}
