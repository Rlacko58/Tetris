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
	HandInit(&h,&t.oszlop, 5);

	printf("\nTetris: \n");
	for (int i = 0; i < h.size; i++) {
		for (int j = 0; j < h.size; j++) {
			printf("%d ", h.v[IND(i, j, h.size)]);
		}
		printf("\n");
	}


	bool* s = Forgat_balra(&h);
	free(h.v);
	h.v = s;

	printf("\nTetris Forgatva balra:\n");
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


	s = Forgat_jobbra(&h);
	free(h.v);
	h.v = s;
	s = Forgat_jobbra(&h);
	free(h.v);
	h.v = s;
	printf("\nTetris Forgatva 2x jobbra:\n");
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
	printf("\n");

	int vsor = 7;
	Eltuntet_sor(&t, &vsor);
	printf("\nMatrix 7.sor eltuntetessel: \n");
	for (int i = 0; i < t.sor; i++) {
		for (int j = 0; j < t.oszlop; j++) {
			printf("%d ", t.v[IND(i, j, t.oszlop)].e);
		}
		printf("\n");
	}

	

	free(t.v);
	free(t.sum);
	free(h.v);

	return 0;
}