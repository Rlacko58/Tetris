#include "map.h"



void Lefoglal(Map** t, int const *sor, int const *oszlop) {
	*t = (Map*)malloc(*sor * *oszlop * sizeof(Map));
}

Map* Lenullaz(Map* t, int const *sor, int const *oszlop) {
	t[IND(3,4,*oszlop)].e=0;
	return t;
}