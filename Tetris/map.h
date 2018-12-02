#ifndef MAP_H
#define MAP_H
#include <stdbool.h>

//Külső fájlok
#include "hand.h"

//Mátrixszerű pointer elérés egyszerűsítése
#define IND(x,y,oszlop) ((x) * oszlop + (y))

//Pálya struktúra
typedef struct {
	bool e;				//Cellában van-e bármi?
	int c;				//Mi a szine?
} PalyaMatrix;

typedef struct {
	int p;				//Perc
	int mp;				//Másodperc
} Ido;

typedef struct Rang{
	char *nev;		//Játékos neve
	int pont;			//Pontszáma
	Ido time;			//Ideje
} Ranglista;

typedef struct {
	int sor, oszlop;	//Pálya mérete
	float Nsize;		//Négyzetek mérete
	float width, height;//Pálya szélesség, magasság
	int *sum;			//Soronkénti összeg
	int level;			//Jelenlegi szint
	Ido time;			//Eltelt idő
	int pont;			//Pontszám
	int ElSorSzam;		//Eltüntetett sorok száma
	PalyaMatrix *v;		//Pálya mátrixa
	int Tarsoly;		//Tarsolyban lévő tetris
	int KoviT[2];		//Kovetkező Tetrisek
	Ranglista rlista[500];			//Ranglista
} Palya;

typedef enum FInp {
	nev,
	score,
	perc,
	masodperc,
	vegeell
} FInp;

//Memoria terulet lefoglalasa a pályának
PalyaMatrix* MatrixFoglal(Palya const *vp);

//A pálya inicializálása
void MatrixInit(Palya *vp, int const sor, int const oszlop);

//Tetris átmásolása a pályára
void MatrixbaMasol(Palya *vp, Hand *hp);

//Ütközés vizsgálat x és y eltolással bp tömbre
bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp, int x, int y);

//Adott sor eltüntetése, majd fölötte lévők lejebb húzása
void Eltuntet_sor(Palya *vp, int sor);

//Alsó tetris távolsága a jelenlegitől
int AltetrisKord(Palya const *vp, Hand const *hp);

//Következő tetrisre állítás
void KovTetris(Palya *vp, Hand *hp, bool *vege);

void Ranglistabeolvas(Palya *vp);

void RanglistaRendez(Palya *vp);

#endif