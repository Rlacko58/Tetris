#include "hand.h"

#include "debugmalloc.h"

//Lefoglalja a tetris mátrixát
bool* HandFoglal(Hand const *hp) {
	return (bool*)malloc((hp->size)*(hp->size) * sizeof(bool));
}

//Lenullázza a kézben lévő tetris Inicializáció
void HandInit(Hand* hp, int const size) {
	hp->x = 0; hp->y = 0; 
	hp->color = 0; hp->size = size;
	hp->v = HandFoglal(hp);
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++) 
			hp->v[IND(i, j, hp->size)] = false;
}

//Elforgatja a kézben lévő tetrist és visszatér annak mátrixával
bool* Forgat(Hand const *hp) {
	bool* sv = HandFoglal(hp);
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++) {
			sv[IND(j, hp->size - 1 - i, hp->size)] = hp->v[IND(i, j, hp->size)];
		}
	return sv;
}

