#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "map.h"
#include "hand.h"


/*



*/

int main()
{
	Map* t;
	int sor = 10, oszlop = 10;
	Lefoglal(&t, &sor, &oszlop);
	t = Lenullaz(&t, &sor, &oszlop);
	printf("%d", t[IND(3, 4, oszlop)].e);
	return 0;             
}
