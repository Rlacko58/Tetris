#ifndef HAND_H
#define HAND_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define IND(x,y,oszlop) ((x) * oszlop + (y))

typedef struct {
	int x, y;	//Pozicioja a Maphez k�pest
	int color;	//Szine
	int size;	//M�trix m�rete
	bool* e;	//Tartalma
} Hand;

bool* HandFoglal(int const *size);
void HandNullaz(Hand* h);

#endif