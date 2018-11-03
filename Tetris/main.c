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
	Palya t;
	MatrixInit(&t, 10, 5);

	printf("Matrix: \n");
	for (int i = 0; i < t.sor; i++) {
		for (int j = 0; j < t.oszlop; j++) {
			printf("%d ", t.v[IND(i, j, t.oszlop)].e);
		}
		printf("\n");
	}

	Hand h;
	HandInit(&h, 4);
	h.v[IND(1, 0, h.size)] = true;
	h.v[IND(1, 1, h.size)] = true;
	h.v[IND(1, 2, h.size)] = true;
	h.v[IND(1, 3, h.size)] = true;
	h.x = 3; h.y = 1; h.color = 1;

	printf("\nTetris: \n");
	for (int i = 0; i < h.size; i++) {
		for (int j = 0; j < h.size; j++) {
			printf("%d ", h.v[IND(i, j, h.size)]);
		}
		printf("\n");
	}
	
	
	bool* s = Forgat(&h);
	free(h.v);
	h.v = s;

	printf("\nTetris Forgatva:\n");
	for (int i = 0; i < h.size; i++) {
		for (int j = 0; j < h.size; j++) {
			printf("%d ", h.v[IND(i, j, h.size)]);
		}
		printf("\n");
	}

	MatrixbaMasol(&t, &h);

	printf("\nMatrix bemasolt tetris-el: \n");
	for (int i = 0; i < t.sor; i++) {
		for (int j = 0; j < t.oszlop; j++) {
			printf("%d ", t.v[IND(i, j, t.oszlop)].e);
		}
		printf("\n");
	}

	
	s = Forgat(&h);
	free(h.v);
	h.v = s;
	s = Forgat(&h);
	free(h.v);
	h.v = s;
	printf("\nTetris Forgatva megint:\n");
	for (int i = 0; i < h.size; i++) {
		for (int j = 0; j < h.size; j++) {
			printf("%d ", h.v[IND(i, j, h.size)]);
		}
		printf("\n");
	}
	printf("Lenne utkozes bemasolaskor? ");
	if (Utkozes(&t, &h, &h.v[0]))
		printf("Igen");
	else
		printf("Nem");

	free(t.v);
	free(t.sum);
	free(h.v);

	return 0;             
}
