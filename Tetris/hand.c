#include "hand.h"

bool* HandFoglal(int const *size) {
	return (bool*)malloc((*size)*(*size) * sizeof(bool));
}
void HandNullaz(Hand* h) {
	h->x = 0; h->y = 0; h->color = 0;
	h->e = HandFoglal(&h->size);
	for (int i = 0; i < h->size; i++)
		for (int j = 0; j < h->size; j++) {
			h->e[IND(i, j, h->size)] = false;
		}
}

