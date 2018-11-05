#ifndef HAND_H
#define HAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//Mátrixszerű pointer elérés egyszerűsítése
#define IND(x,y,oszlop) ((x) * oszlop + (y))

//A kézben lévő tetris struktúrája
typedef struct {
	int x, y;	//Pozicioja a Maphez képest
	int color;	//Szine
	int size;	//Mátrix mérete
	bool* v;	//Tartalma
} Hand;

//Lefoglalja a tetris mátrixát
bool* HandFoglal(Hand const *hp);
//Kézben lévő tetris Inicializáció
void HandInit(Hand* hp, int const size);
//Elforgatja a kézben lévő tetrist és visszatér annak mátrixával
bool* Forgat(Hand const *hp);

#endif