#ifndef MAP_H
#define MAP_H
#include "hand.h"

#include <stdbool.h>

//Mátrixszerű pointer elérés egyszerűsítése
#define IND(x,y,oszlop) ((x) * oszlop + (y))

//Pálya struktúra
typedef struct {
	bool e;				//Cellában van-e bármi?
	int c;				//Mi a szine?
} PalyaMatrix;

typedef struct {
	int sor, oszlop;	//Pálya mérete
	float Nsize;		//Négyzetek mérete
	float width, height;//Pálya szélesség, magasság
	int *sum;			//Soronkénti összeg
	int level;			//Jelenlegi szint
	PalyaMatrix *v;		//Pálya mátrixa
} Palya;

//Memoria terulet lefoglalasa a pályának
PalyaMatrix* MatrixFoglal(Palya const *vp);

//A pálya inicializálása
void MatrixInit(Palya *vp, int const sor, int const oszlop);

//Tetris átmásolása a pályára
void MatrixbaMasol(Palya *vp, Hand *hp);

//Ütközés vizsgálat
bool Utkozes(Palya const *vp, Hand const *hp, bool const *bp);

//Adott sor eltüntetése, majd fölötte lévők lejebb húzása
void Eltuntet_sor(Palya *vp, int sor);

#endif