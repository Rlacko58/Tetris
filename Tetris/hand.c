#include "hand.h"

//Lefoglalja a tetris mátrixát
bool* HandFoglal(int const *size) {
	return (bool*)malloc((*size)*(*size) * sizeof(bool));
}

//Lenullázza a kézben lévő tetrist (Előtte h->size-ot meg kell adni)
void HandNullaz(Hand* h) {
	h->x = 0; h->y = 0; h->color = 0;
	h->e = HandFoglal(&h->size);
	for (int i = 0; i < h->size; i++)
		for (int j = 0; j < h->size; j++) {
			h->e[IND(i, j, h->size)] = false;
		}
}

