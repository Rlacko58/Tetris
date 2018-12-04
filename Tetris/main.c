#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

//Megjelenítéshez szükséges includok
#include "GL/glut.h"
#include <windows.h>

//Debughoz
#include "debugmalloc.h"

//Külső fájlok
#include "map.h"
#include "hand.h"
#include "megjelenites.h"

//Belső globális változók
static Palya t;					//Pálya
static Hand h;					//Kézben lévő tetris
static int GravInterv = 0;		//Gravitáció időköz
static int MpCounter = 0;		//Másodperc számláló
static bool valthat = true;		//Tarsolyba lehet-e tenni?
static int menu = 0;
static bool vege = false;

//Játék függvény
void Jatek() {
	// Gravitáció rendszeres újrameghívása
	if (GravInterv >= (int)100 / t.level) {		//Szinttől függően sűrűbben
		GravInterv = 0;
		if (!Utkozes(&t, &h, &h.v[0], 1, 0)) h.x += 1;
		else {		//Ha ütközés lenne, bemásolja és engedélyezi a tarsolyba rakást
			MatrixbaMasol(&t, &h);
			valthat = true;
			KovTetris(&t, &h, &vege);
		}
	}
	//Másodperc számláló, 10milliszekundomonként hívja meg magát a program
	if (MpCounter == 100) { t.time.mp++; MpCounter = 0; }
	//Idő kijelző perc és mp közti átváltások
	if (t.time.mp == 60) { t.time.p++; t.time.mp = 0; }

	//Adott időközönkénti szint növelés
	if ((t.time.mp + (t.time.p * 60)) > 0 &&
		(t.time.mp + (t.time.p * 60)) % 30 == 0 &&	//30másodpercenként
		MpCounter == 0) t.level++;
}

/* Handler - Ablak újrarajzolás, meghívódik, mikor az ablak
	újrarajzolandónak jelölődik*/
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Törlés
	glMatrixMode(GL_MODELVIEW);     // Modelnézetű mátrixban való dolgozás
	glLoadIdentity();               // Alaphelyzetbe állítás
	if (!vege) {
		Jatek();	//Változók és feltételek

		//Kirajzolás
		JatekRajzol(&t, &h);

		//Változók növelése
		MpCounter++;
		GravInterv++;
	}
	else {
		GameOverRajzol(&t, &h);
	}


	glutSwapBuffers();   // kicseréli az elöl lévő buffert a hátul lévővel
}

void keyboard(unsigned char key, int x, int y)	//Egyszerűbb gombok
{
	if (!vege) {
		switch (key) {
		case 27: // Escape -- Kilépés
			glutDestroyWindow(1);
			break;
		case 32: // Szóköz -- Letesz az alul lévő tetris helyére
			h.x += AltetrisKord(&t, &h);	//Eltol
			MatrixbaMasol(&t, &h);			//Bemásol

			valthat = true;
			KovTetris(&t, &h, &vege);				//Következő tetrisre váltás
			break;
		case 'c':
			if (valthat) {					//Tarsolyba tétel / csere
				if (t.Tarsoly != -1) {		//Ha nem üres
					int sv = t.Tarsoly;
					t.Tarsoly = h.melyik;
					free(h.v);
					HandInit(&h, &t.oszlop, sv);
				}
				else {						//Ha üres
					t.Tarsoly = h.melyik;
					KovTetris(&t, &h, &vege);
				}
				valthat = false;			//Legközelebbi bemásolásig
			}
			break;
		}
	}
	else {
		if (isalpha(key)) {
			Nevhezir(&t, key);
		}
		else if (key == 8) {
			NevbolTorol(&t);
		}
		else if (key == 13) {
			Ranglistahozad(&t);
			RanglistaRendez(&t);
			glutDestroyWindow(1);
		}
	}
	glutPostRedisplay();					//Ablak újrajzolásra jelölése
}

void specialKeys(unsigned char key, int x, int y)	//Speciális gombok
{
	if (!vege) {
		bool* forgatott;	//Segéd mátrix
		switch (key) {
		case GLUT_KEY_UP:	// Forgat
			forgatott = Forgat_jobbra(&h);
			if (!Utkozes(&t, &h, &forgatott[0], 0, 0)) { //Ha nincs ütközés
				free(h.v);
				h.v = forgatott;	//Bemásol
			}
			else {
				free(forgatott);	//Különben nem
			}
			break;
		case GLUT_KEY_DOWN:	// Lemozgat
			if (!Utkozes(&t, &h, &h.v[0], 1, 0)) {
				h.x += 1;
			}
			break;
		case GLUT_KEY_LEFT:	// Mozgat balra
			if (!Utkozes(&t, &h, &h.v[0], 0, -1)) {
				h.y += -1;
			}
			break;
		case GLUT_KEY_RIGHT:// Mozgat jobbra
			if (!Utkozes(&t, &h, &h.v[0], 0, 1)) {
				h.y += 1;
			}
			break;
		}
	}
	else {
		
	}
	glutPostRedisplay();
}



int main(int argc, char** argv) {
	srand(time(NULL));				//Randomizálás biztosításához
	int szelesseg, magassag;
	printf("Add meg a Szelesseget es magassagot (pl.: 10 20)\n");
	scanf("%d %d", &szelesseg, &magassag);
	if (szelesseg <= 0 || magassag <= 0) { szelesseg = 20; magassag = 30; }
	MatrixInit(&t, magassag, szelesseg);			//Kezdő pálya inicializáció
	KirajzInit(&t);					//Kirajzoláshoz szükséges inicializáció
	Ranglistabeolvas(&t);			//Ranglista fájlból beolvasása
	RanglistaRendez(&t);

	HandInit(&h, &t.oszlop, 3);		//Első tetris kézbe helyezése

	glutInit(&argc, argv);				// GLUT inicializáció
	glutInitDisplayMode(GLUT_DOUBLE);	// Duplán bufferált mód
	glutInitWindowSize(1000, 700);		// Kezdő szélesség, magasság
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2);	//Középre elhelyezés
	glutCreateWindow("Tetris játék");				// Ablak cím
	glutDisplayFunc(display);			// Újrarajzoláskor lefuttatandó függvény beállítása
	glutReshapeFunc(Ujrameretez);		// Register callback handler for window re-size event
	glutTimerFunc(0, Idozito, 0);		// First timer call immediately
	glutKeyboardFunc(keyboard);			// Alap gombok kezelése
	glutSpecialFunc(specialKeys);		// Speciális gombok
	initGL();                       // Extra inicializációhoz szükséges dolgok
	glutMainLoop();                 // Belépés a végtelen ciklusba
	return 0;
}