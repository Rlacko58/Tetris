#ifndef HAND_H
#define HAND_H

#include <stdbool.h>

//Mátrixszerű pointer elérés egyszerűsítése
#define IND(x,y,oszlop) ((x) * oszlop + (y))

//A kézben lévő tetris struktúrája
typedef struct {
	int x, y;	//Pozicioja a Maphez képest
	int color;	//Szine
	int size;	//Mátrix mérete (size*size)
	bool* v;	//Tartalma
} Hand;

//Lefoglalja a tetris mátrixát
bool* HandFoglal(Hand* hp);
//Kézben lévő tetris Inicializáció
void HandInit(Hand* hp, int const *oszlop, int const melyik);
//Elforgatja a kézben lévő tetrist és visszatér annak mátrixával
bool* Forgat_jobbra(Hand const *hp);
bool* Forgat_balra(Hand const *hp);



#endif