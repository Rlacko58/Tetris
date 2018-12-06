#include "ranglista.h"

#include <stdlib.h>
#include <stdio.h>

//Debughoz
#include "debugmalloc.h"

//Ranglista két elemének cserélése
static void RangCsere(Ranglista *a, Ranglista *b) {
	Ranglista sv;
	sv.nev = a->nev; sv.pont = a->pont; sv.time = a->time;
	a->nev = b->nev; a->pont = b->pont; a->time = b->time;
	b->nev = sv.nev; b->pont = sv.pont; b->time = sv.time;
}

//--- Mentéshez ----//
//Játékos nevéhez karakter hozzáfűzése
void Nevhezir(Palya *vp, int c) {
	int i = 0;
	while (vp->nev[i] != '\0' && i < 7) i++;
	if (i < 5)	vp->nev[i] = c;
	vp->nev[i + 1] = '\0';
}
//Játékos nevéből karakter törlése
void NevbolTorol(Palya *vp) {
	int i = 0;
	while (vp->nev[i] != '\0') i++;
	if (i > 0)	vp->nev[i - 1] = '\0';
}
//------------------//

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
			if (vp->rlista[j].pont == -1) break;
			RangCsere(&vp->rlista[j], &vp->rlista[maxhely]);
			j++; maxhely = j; i = j;
		}
		if (vp->rlista[maxhely].pont < vp->rlista[i].pont) maxhely = i;
		if (vp->rlista[i].pont != -1) i++;
	}
	Ranglistament(vp);
}

//Ranglista mentése
void Ranglistament(Palya *vp) {
	FILE *fp;
	fp = fopen("HighScore.txt", "w");
	int i = 0;
	while (vp->rlista[i].pont != -1) {
		Ranglista *sv = &vp->rlista[i];
		fprintf(fp, "%s %d %d %d\n", sv->nev, sv->pont, sv->time.p, sv->time.mp);
		i++;
	}
	fclose(fp);
}

//Ranglistához hozzáadás
void Ranglistahozaad(Palya *vp) {
	int i = 0;
	while (vp->rlista[i].pont != -1) i++;
	if (vp->nev[0] == '\0') {
		vp->nev[0] = '-'; vp->nev[1] = '\0';
	}
	vp->rlista[i].nev = vp->nev;
	vp->rlista[i].pont = vp->pont;
	vp->rlista[i].time = vp->time;
	vp->rlista[i + 1].pont = -1;
}

//Ranglista feltöltése fájlból
void Ranglistabeolvas(Palya *vp) {
	//Állapotgépes módszerrel megoldott fájlból beolvasás
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
		switch (fmode) {
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
			vp->rlista[i].nev = (char*)malloc((strlen(hnev) + 1) * sizeof(char));
			strcpy(vp->rlista[i].nev, hnev);
			vp->rlista[i].pont = hscore;
			vp->rlista[i].time.p = hido.p;
			vp->rlista[i].time.mp = hido.mp;

			vp->rlista[i + 1].pont = -1;
			hscore = 0; hido = (Ido) { 0, 0 };
			i++;
			if (c == EOF) mehet = false;
			else fmode = nev;
			break;
		}
	}
	fclose(fp);
}