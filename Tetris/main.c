#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "debugmalloc.h"

#include "map.h"
#include "hand.h"


/*



*/

int main()
{
	int sor = 10, oszlop = 5;
	Map *tomb = MapFoglal(&sor, &oszlop);
	tomb = MapNullaz(&tomb[0], &sor, &oszlop);
	Hand h;
	h.size = 4;
	HandNullaz(&h);
	h.e[IND(1, 0, h.size)] = true;
	h.e[IND(1, 1, h.size)] = true;
	h.e[IND(1, 2, h.size)] = true;
	h.e[IND(1, 3, h.size)] = true;
	h.x = 3; h.y = 1; h.color = 1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			printf("%d",h.e[IND(i, j, h.size)]);
		printf("\n");
	}

	tomb = MapbaMasol(&tomb[0], &oszlop, &h);
	for (int i = 0; i < sor; i++) {
		for (int j = 0; j < oszlop; j++)
			printf("%d ", tomb[IND(i, j, oszlop)].e);
		printf("\n");
	}
	
	printf("\nForgatas:\n");
	bool* s = Forgat(&h);
	free(h.e);
	h.e = s;
	s = Forgat(&h);
	free(h.e);
	h.e = s;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			printf("%d", h.e[IND(i, j, h.size)]);
		printf("\n");
	}
	tomb = MapNullaz(&tomb[0], &sor, &oszlop);
	tomb = MapbaMasol(&tomb[0], &oszlop, &h);
	for (int i = 0; i < sor; i++) {
		for (int j = 0; j < oszlop; j++)
			printf("%d ", tomb[IND(i, j, oszlop)].e);
		printf("\n");
	}


	free(tomb);
	free(h.e);

	return 0;             
}
