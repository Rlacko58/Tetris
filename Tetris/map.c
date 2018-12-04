#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

//Debughoz
#include "debugmalloc.h"


//Memoria terulet lefoglalasa a pályának
PalyaMatrix* MatrixFoglal(Palya const *vp) {
	return (PalyaMatrix*)malloc((vp->sor) * (vp->oszlop) * sizeof(PalyaMatrix));
}

//Ranglista két elemének cserélése
static void RangCsere(Ranglista *a, Ranglista *b) {
	Ranglista sv;
	sv.nev = a->nev; sv.pont = a->pont; sv.time = a->time;
	a->nev = b->nev; a->pont = b->pont; a->time = b->time;
	b->nev = sv.nev; b->pont = sv.pont; b->time = sv.time;
}

void Nevhezir(Palya *vp, int c) {
	int i = 0;
	while (vp->nev[i] != '\0' && i<7) i++;
	if(i<5)	vp->nev[i] = c;
	vp->nev[i + 1] = '\0';
}

void NevbolTorol(Palya *vp) {
	int i = 0;
	while (vp->nev[i] != '\0') i++;
	if(i>0)	vp->nev[i - 1] = '\0';
}

//Ranglista rendezése
void RanglistaRendez(Palya *vp) {
	if (vp->rlista[0].pont == -1) {
		return;
	}
	int i = 1;
	int j = 0;
	int maxhely = 0;
	while (true) {
		if (vp->rlista[i].pont == -1) {
			//printf("%d ", i);
			if (vp->rlista[j].pont == -1) break;
			RangCsere(&vp->rlista[j], &vp->rlista[maxhely]);

			j++; maxhely = j; i = j;

		}
		if (vp->rlista[maxhely].pont < vp->rlista[i].pont) maxhely = i;
		if (vp->rlista[i].pont != -1) i++;
	}
	Ranglistament(vp);
}

void Ranglistament(Palya *vp) {
	FILE *fp;
	fp = fopen("HighScore.txt", "w");
	int i = 0;
	while (vp->rlista[i].pont != -1) {
		Ranglista *sv = &vp->rlista[i];
		fprintf(fp,"%s %d %d %d\n", sv->nev, sv->pont, sv->time.p, sv->time.mp);
		i++;
	}
	fclose(fp);
}

void Ranglistahozad(Palya *vp) {
	int i = 0;
	while (vp->rlista[i].pont != -1) i++;
	vp->rlista[i].nev = vp->nev;
	vp->rlista[i].pont = vp->pont;
	vp->rlista[i].time = vp->time;
	vp->rlista[i + 1].pont = -1;
}

//Ranglista feltöltése fájlból
void Ranglistabeolvas(Palya *vp) {
	typedef enum FInp {
		nev,
		score,
		perc,
		masodperc,
		vegeell
	} FInp;

	FILE *fp;
	fp = fopen("HighScore.txt", "r");
	int c;

	char hnev[6];
	int hscore = 0;
	Ido hido = { 0,0 };

	FInp fmode = nev;
	int i = 0;
	int ni = 0;
	bool mehet = true;
	while (mehet) {
		switch(fmode){
		case nev: 
			c = getc(fp);
			if (c == EOF) { 
				mehet = false;
				if (i == 0) vp->rlista[0].pont = -1;
				break; 
			}
			if (c == ' ') {
				hnev[ni] = '\0';
				ni = 0;
				fmode = score;
			}
			else {
				hnev[ni] = c;
				ni++;
			}
			break;
		case score: 
			c = getc(fp);
			if (c == ' ') fmode = perc;
			else {
				hscore *= 10;
				hscore += c - 48;
			}
			break;
		case perc: 
			c = getc(fp);
			if (c == ' ') fmode = masodperc;
			else {
				hido.p *= 10;
				hido.p += c - 48;
			}
			break;
		case masodperc: 
			c = getc(fp);
			if (c == '\n' || c == EOF) fmode = vegeell;
			else {
				hido.mp *= 10;
				hido.mp += c - 48;
			}
			break;
		case vegeell: 
			vp->rlista[i].nev = (char*)malloc(strlen(hnev)*sizeof(char));
			strcpy(vp->rlista[i].nev, hnev);
			vp->rlista[i].pont = hscore;
			vp->rlista[i].time.p = hido.p;
			vp->rlista[i].time.mp = hido.mp;

			vp->rlista[i + 1].pont = -1;
			hscore = 0; hido = (Ido){ 0,0 };
			i++;
			if (c == EOF) mehet=false;
			else fmode = nev;
			break;
		}
	}
	fclose(fp);
}

//A pálya inicializálása
void MatrixInit(Palya *vp, int const sor, int const oszlop) {
	vp->sor = sor; vp->oszlop = oszlop;
	vp->Nsize = vp->width = vp->height = 0; 
	vp->time.p = vp->time.mp = 0;
	vp->Tarsoly = -1;
	vp->KoviT[0] = rand() % 7;
	vp->KoviT[1] = rand() % 7;
	vp->v = MatrixFoglal(vp);
	vp->level = 1;
	vp->nev = (char*)malloc(7 * sizeof(char));	strcpy(vp->nev, "\0");
	
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
				vp->sum[hp->x + i]++; 
				if (vp->sum[hp->x + i] == vp->oszlop) Eltuntet_sor(vp, hp->x + i);
			}
}
//Ütközés vizsgálat x és y eltolással bp tömbre
bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp, int x, int y) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (bp[IND(i, j, hp->size)]) {
				if (((vp->v[IND(hp->x + i + x, hp->y + j + y, vp->oszlop)].e) && (hp->x + i >= 0)) ||
					hp->y + j + y >= vp->oszlop || hp->y + j + y < 0 ||
					hp->x + i + x > vp->sor-1)
					return true;
			}
	return false;
}

//Alsó tetris távolsága a jelenlegitől
int AltetrisKord(Palya const *vp, Hand const *hp) {
	int x = 1;
	while (!Utkozes(vp, hp, &hp->v[0], x, 0) && x<=vp->sor) {
		x++;
	}
	return x-1;
}

//Következő tetrisre állítás
void KovTetris(Palya *vp, Hand *hp, bool *vege) {
	free(hp->v);
	HandInit(hp, &vp->oszlop, vp->KoviT[0]);
	vp->KoviT[0] = vp->KoviT[1];
	vp->KoviT[1] = rand() % 7;
	if (Utkozes(vp, hp, hp->v, 0, 0)) *vege = true;
}

//Adott sor eltüntetése, majd fölötte lévők lejebb húzása
void Eltuntet_sor(Palya *vp, int sor) {
	vp->ElSorSzam++;
	vp->pont += vp->oszlop*vp->level*vp->ElSorSzam;
	vp->sum[sor] = 0;
	for (int i = sor; i > 0; i--) {
		vp->sum[i] = vp->sum[i-1];
	}
	for (int i = 0; i < vp->oszlop; i++) {
		for (int j = sor; j > 0; j--) {
			vp->v[IND(j, i, vp->oszlop)].e = vp->v[IND(j-1, i, vp->oszlop)].e; 
			vp->v[IND(j - 1, i, vp->oszlop)].e = 0;
			vp->v[IND(j, i, vp->oszlop)].c = vp->v[IND(j-1, i, vp->oszlop)].c;
			vp->v[IND(j - 1, i, vp->oszlop)].c = 0;
		}
	}
}