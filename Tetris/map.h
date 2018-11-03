#ifndef MAP
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define IND(x,y,oszlop) ((x) * oszlop + (y))

typedef struct {
	bool e;
	int c;
} Map;

void Lefoglal(Map** t, int const *sor, int const *oszlop);
Map* Lenullaz(Map* t, int const *sor, int const *oszlop);

#endif