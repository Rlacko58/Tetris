#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

//Megjelenítéshez szükséges dolgok
#include "GL/glut.h"
#include <windows.h>
#include "GL/freeglut.h"

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
static bool vege = false;
//	Azért kellenek ezek a belső változók, mivel a glut a játék 
//	vezérlését végző függvényeknek csak a pointerjeit várja:
//	Pl.:  void glutDisplayFunc(void (*func)(void));
//	Tehát paraméteresen nem lehet átadni neki változókat
//	De az alábbi "globális" változók, csak a fájlon belül
//	vannak használva, kívülre mindent át kell adni.

//Játékhoz változók és feltételek
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
		//Játék vége kirajzolása
		GameOverRajzol(&t, &h);
	}


	glutSwapBuffers();   // kicseréli az elöl lévő buffert a hátul lévővel
}

//A mallocolt memória területek felszabadítása
static void Felszabadit() {
	int i = 0;
	while (t.rlista[i].nev != NULL) free(t.rlista[i++].nev);
	if(t.v!=NULL)	free(t.v);
	if(h.v!=NULL)	free(h.v);
}

void keyboard(unsigned char key, int x, int y)	//Egyszerűbb gombok
{
	if (!vege) {
		switch (key) {
		case 27: // Escape -- Kilépés
			glutLeaveMainLoop();	//Kilépés a glut végtelen ciklusából
			break;
		case 32: // Szóköz -- Leteszi a tetrist alulra
			h.x += AltetrisKord(&t, &h);	//Eltol
			MatrixbaMasol(&t, &h);			//Bemásol

			valthat = true;							//Tarsolyba rakhat-e?

			KovTetris(&t, &h, &vege);				//Következő tetrisre váltás
			break;
		case 'c':	//c -- Tarsolyba rakás
			if (valthat) {					//Tarsolyba tehet / cserélhet
				if (t.Tarsoly != -1) {		//Ha nem üres
					int sv = t.Tarsoly;
					t.Tarsoly = h.melyik;
					free(h.v);
					HandInit(&h, &t.oszlop, sv); //Inicializálja a tarsolyban lévő szám alapján
				}
				else {						//Ha üres
					t.Tarsoly = h.melyik;		//Belerakja a kézben lévő tetris számát
					KovTetris(&t, &h, &vege);
				}
				valthat = false;			//Legközelebbi bemásolásig
			}
			break;
		}
	}
	else {
		if (isalpha(key)) {		//Ha betűt ütött le
			Nevhezir(&t, key);	//Hozzáírja a névhez
		}
		else if (key == 8) {	//Ha törlés gomb
			NevbolTorol(&t);	//Egy karakter töröl
		}
		else if (key == 13) {	//Ha enter
			Ranglistahozaad(&t);
			RanglistaRendez(&t);
			Felszabadit();
			glutLeaveMainLoop();	
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
	glutPostRedisplay();
}

//Uj játék indítása
void UjJatek(int argc, char** argv) {
	int szelesseg, magassag;
	printf("Add meg a Szelesseget es magassagot (pl.: 20 30)\n");
	scanf("%d %d", &szelesseg, &magassag);
	if (szelesseg <= 0 || magassag <= 0) { szelesseg = 20; magassag = 30; }	//Ha nem jót adna meg, alap értékek

	MatrixInit(&t, magassag, szelesseg);		//Kezdő pálya inicializáció
	KirajzInit(&t);								//Kirajzoláshoz szükséges inicializáció
	Ranglistabeolvas(&t);						//Ranglista fájlból beolvasása
	RanglistaRendez(&t);						//Ranglista rendezése

	HandInit(&h, &t.oszlop, 3);		//Első tetris kézbe helyezése

	glutInit(&argc, argv);				// GLUT inicializáció
	glutInitDisplayMode(GLUT_DOUBLE);	// Duplán bufferált mód
	glutInitWindowSize(1000, 700);		// Kezdő szélesség, magasság
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2);	//Középre elhelyezés
	glutCreateWindow("Tetris játék");				// Ablak cím
	glutDisplayFunc(display);			// Újrarajzoláskor lefuttatandó függvény beállítása
	glutReshapeFunc(Ujrameretez);		// Ujraméretezéskor lefuttatandó ...
	glutTimerFunc(0, Idozito, 0);		// Időzítő
	glutKeyboardFunc(keyboard);			// Alap gombok kezelése
	glutSpecialFunc(specialKeys);		// Speciális gombok
	initGL();                       // Extra inicializációhoz szükséges dolgok
	glutMainLoop();                 // Belépés a végtelen ciklusba
}

int main(int argc, char** argv) {
	srand(time(NULL));				//Tényleg random számokat kapjunk
	
	UjJatek(argc, argv);
	return 0;
}