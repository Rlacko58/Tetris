#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
	h.size = 3;
	HandNullaz(&h);
	h.e[IND(2, 1, h.size)] = true;
	h.x = 3; h.y = 1; h.color = 1;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			printf("%d",h.e[IND(i, j, h.size)]);
		printf("\n");
	}

	tomb = MapbaMasol(&tomb[0], &oszlop, &h);
	for (int i = 0; i < sor; i++) {
		for (int j = 0; j < oszlop; j++)
			printf("%d ", tomb[IND(i, j, oszlop)].e);
		printf("\n");
	}
	



	return 0;             
}
