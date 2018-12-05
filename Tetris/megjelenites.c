#include "megjelenites.h"

#include "stdio.h" //Tesztelésekhez konzolra iratás

//Debughoz
#include "debugmalloc.h"

//Tehát a függvénynek csak egy értéket lehet átadni

//OpenGL Grafikához szükséges inicializációk
void initGL() {
	//Törléskor használt szin
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fekete
}

//void glutTimerFunc(unsigned int msecs, void(*func)(int value), value);
// Játék belső időzítője
void Idozito(int idokoz, int Frissites_ms) {
	glutPostRedisplay(); //Jelenlegi ablak újrarajzolandónak jelölése
	glutTimerFunc(idokoz, Idozito, idokoz); // Időzítő indítása megint, (mennyi idő múlva, melyik függvény, mit adjon át)
}

/* Handler - windows re-size :
	Mikor az ablakot átméretezik, az arányok nem romlanak el. */
void Ujrameretez(GLsizei width, GLsizei height) {  // GLsizei (int)
   // Új képarány kiszámítása
	if (height == 0) height = 1;                // 0-val osztás elkerülése
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Viewport teljes ablakra állítása
	glViewport(0, 0, width, height);
	// Képarány beállítása
	glMatrixMode(GL_PROJECTION);  // Megjelenítő mátrixon dolgozás
	glLoadIdentity();	//Alapértelmezett mátrixra állítás (forgatás, stb. nélküli)
	if (width >= height) {
		gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
	}
	else {
		gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
	}
}
// Ez a függvény, az alábbi oldal példája alapján készült:
// http://www3.ntu.edu.sg/home/ehchua/programming/opengl/cg_introduction.html

// Kis matek a megfelelő kirajzolásokhoz
void KirajzInit(Palya *vp) {		
	float Nmx = 0.1f;				
	float Nps = 2.0f / vp->oszlop;	
	float Npo = 2.0f / vp->sor;		
	float Nsize = 0.0f;				
	if (Nps < Npo)	Nsize = Nps;
	else			Nsize = Npo;

	if (Nsize > Nmx) Nsize = Nmx;
	vp->Nsize = Nsize;
	vp->width = Nsize * vp->oszlop;
	vp->height = Nsize * vp->sor;
}

//Szöveg kirajzolása x,y kordinátára adott mérettel
static void Szovegrajzol(char* szoveg, GLfloat x, GLfloat y, float meret){
	glPushMatrix();	
	
	glTranslatef(x, y, 0);	//Megfelelo poziciora rajzolas
	glScalef(meret / 152.38, meret / 152.38, 0);	//Megfelelo meret beallitasa
	for (char* p = szoveg; *p; p++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p); //Kirajzolas
	}
	glPopMatrix();
}

/*	Négyzet rajzolása Nsize mérettel x,y kordinátára s színnel
	Egyik mód a normál tetris rajzolása, másik mikor halványan kell */
static void RajzolNegyzet(GLfloat *Nsize, GLfloat x, GLfloat y, RGBA s, bool mode) {
	if (mode) {	//Halványítás
		s.r /= 3;
		s.g /= 3;
		s.b /= 3;
	}
	glPushMatrix();
	glColor4f(s.r, s.g, s.b, s.a);
	glTranslatef(x, y, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, *Nsize);
		glVertex2f(*Nsize, *Nsize);
		glVertex2f(*Nsize, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glTranslatef(x, y, 0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0, 0);
		glVertex2f(0, *Nsize);
		glVertex2f(*Nsize, *Nsize);
		glVertex2f(*Nsize, 0);
	glEnd();
	glPopMatrix();
	
}

//x, y kordináta szerint egy Nsize méretű négyzetrácsos hálóba rajzolás
static void RajzolHaloba(Palya *vp, float x, float y, int sor, int oszlop, RGBA s, bool mode) {
	RajzolNegyzet(&vp->Nsize,
		x + (2.0 - vp->width) / 2.0 + oszlop * vp->Nsize,
		y - (2.0 - vp->height) / 2.0 - (1+sor) * vp->Nsize,
		s, mode);
}

//Visszatér adott szám alapján a megfelelő színkóddal
static RGBA SzinKonverter(int szin) {
	switch (szin) {
	case 1: return (RGBA) { 1.0, 0.0, 0.0, 0.0 }; break;	//Piros
	case 2: return (RGBA) { 0.0, 1.0, 0.0, 0.0 }; break;	//Zöld
	case 3: return (RGBA) { 0.0, 0.0, 1.0, 0.0 }; break;	//Kék
	case 4: return (RGBA) { 1.0, 0.7, 0.0, 0.0 }; break;	//Narancs
	case 5: return (RGBA) { 1.0, 0.9, 0.0, 0.0 }; break;	//Sárga
	case 6: return (RGBA) { 0.4, 0.2, 0.6, 0.0 }; break;	//lila
	case 7: return (RGBA) { 0.0, 0.7, 0.8, 0.0 }; break;	//cián
	default: return (RGBA) { 0.0, 0.0, 0.0, 0.0};			//Fekete
	}
}

//Pályamátrix kirajzolása
static void RajzolMatrix(Palya *vp) {
	for (int i = 0; i < vp->sor; i++) {
		for (int j = 0; j < vp->oszlop; j++) {
			if (vp->v[IND(i,j,vp->oszlop)].e) {
				RajzolHaloba(vp, -1.0, 1.0, i, j, SzinKonverter(vp->v[IND(i,j,vp->oszlop)].c),0);
			}
			else {
				RajzolHaloba(vp, -1.0, 1.0, i, j, SzinKonverter(0), 0);
			}
		}
	}
}

//Adott kézben lévő tetris kirajzolása
static void RajzolTetris(Palya *vp, Hand *hp, int x, bool mode) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (hp->v[IND(i, j, hp->size)])
				RajzolHaloba(vp, -1.0, 1.0, hp->x + i + x, hp->y + j, SzinKonverter(hp->color), mode);
}


//Adott bool tömbben lévő tetris kirajzolása
static void RajzolVTetris(Palya static *vp, bool* t, int size, float x, float y, int szin) {
	GLfloat nsize = 0.1;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (t[IND(i, j, size)])
				RajzolNegyzet(&nsize, x + j * 0.1, y - (1 + i) * 0.1,
					SzinKonverter(szin), 0);
		}
	}
}

//Idő kirajzolása
static void KirajzolIdo(Ido *t, float x, float y, float size){
	char ido[6] = "";
	ido[0] = (int)(t->p / 10 ) + 48;
	ido[1] = (int)(t->p % 10 ) + 48;
	ido[2] = ':';
	ido[3] = (int)(t->mp / 10) + 48;
	ido[4] = (int)(t->mp % 10) + 48;
	ido[5] = '\0';

	Szovegrajzol(&ido, x, y, size);
}

//Pontszám kirajzolása
static void KirajzolPont(int *ponts, float x, float y, float size) {
	char pont[8] = "";
	int j = 6;
	int svpont = *ponts;
	for (int i = 0; i < 7; i++) {
		pont[j - i] = (int)(svpont % 10) + 48;
		svpont /= 10;
	}
	pont[7] = '\0';
	Szovegrajzol(&pont, x, y, size);
}

//Szint kirajzolása
static void KirajzolSzint(int *szints, float x, float y, float size) {
	char szint[3] = "";
	szint[0] = (int)(*szints / 10) + 48;
	szint[1] = (int)(*szints % 10) + 48;
	szint[2] = '\0';
	Szovegrajzol(&szint, x, y, size);
}

//Eltüntetett sorok számának kirajzolása
static void KirajzolEltSorSzam(Palya *vp, float x, float y, float size) {
	char sorok[5] = "";
	int j = 3;
	int svsor = vp->ElSorSzam;
	for (int i = 0; i < 4; i++) {
		sorok[j - i] = (int)(svsor % 10) + 48;
		svsor /= 10;
	}
	sorok[4] = '\0';
	Szovegrajzol(&sorok, x, y, size);
}

//Tarsolyban lévő tetris kirajzolása
static void RajzolTarsoly(Palya *vp) {
	if (vp->Tarsoly != -1) {
		int size;
		if (vp->Tarsoly == 0) size = 4;
		else if (vp->Tarsoly <= 5) size = 3;
		else size = 2;

		RajzolVTetris(vp, TetroVal(vp->Tarsoly), size, -1.4, 0.78, vp->Tarsoly+1);
	}
}

//Következő tetrisek kirajzolása
static void RajzolKovi(Palya *vp) {
	int size;
	for (int i = 0; i < 2; i++) {
		if (vp->KoviT[i] == 0) size = 4;
		else if (vp->KoviT[i] <= 5) size = 3;
		else size = 2;

		RajzolVTetris(vp, TetroVal(vp->KoviT[i]), size, +1.125-0.055*(size-2), 0.78- i*0.3, vp->KoviT[i] + 1);
	}
}

//Ranglista megjelenítés
static void RajzolRangLista(Palya *vp) {
	for (int i = 0; i < 5; i++) {
		if (vp->rlista[i].pont != -1) {
			char helyezes[2] = "";
			helyezes[0] = i + 1 + 48;
			helyezes[1] = '.';
			Szovegrajzol(&helyezes, 1.015, -0.01 - i * 0.2, 0.07);
			Szovegrajzol(vp->rlista[i].nev, 1.08, -0.01 - i * 0.2, 0.07);
			KirajzolIdo(&vp->rlista[i].time, 1.25, -0.01 - i * 0.2, 0.07);
			KirajzolPont(&vp->rlista[i].pont, 1.13, -0.1 - i * 0.2, 0.07);
		}
		else {
			return;
		}
	}
}

//Játék közbeni Kirajzolás
void JatekRajzol(Palya *vp, Hand *hp) {
	RajzolMatrix(vp);
	RajzolTetris(vp, hp, AltetrisKord(vp, hp), 1);
	RajzolTetris(vp, hp, 0, 0);
	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Kovetkezo", 1.03, 0.85, 0.1);
	RajzolKovi(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Ranglista", 1.03, 0.1, 0.1);
	RajzolRangLista(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Tarsoly", -1.4, 0.85, 0.1);
	RajzolTarsoly(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("|Meret|", -1.4, 0.35, 0.1);
	Ido sv = { vp->sor, vp->oszlop };
	KirajzolIdo(&sv, -1.4, 0.24, 0.1);

	Szovegrajzol("Ido", -1.4, -0.05, 0.1);
	KirajzolIdo(&vp->time, -1.4, -0.16, 0.1);

	Szovegrajzol("Pont", -1.4, -0.30, 0.1);
	KirajzolPont(&vp->pont, -1.4, -0.41, 0.1);


	Szovegrajzol("Szint", -1.4, -0.55, 0.1);
	KirajzolSzint(&vp->level, -1.4, -0.66, 0.1);

	Szovegrajzol("Sorok", -1.4, -0.8, 0.1);
	KirajzolEltSorSzam(vp, -1.4, -0.91, 0.1);	
}


//Játék vége kiiratás
void GameOverRajzol(Palya *vp, Hand *hp) {
	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Jatek vege", -0.3, 0.1, 0.1);
	Szovegrajzol("Neved: ", -0.3, -0.1, 0.1);
	Szovegrajzol(vp->nev, 0.0, -0.1, 0.1);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Kovetkezo", 1.03, 0.85, 0.1);
	RajzolKovi(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Ranglista", 1.03, 0.1, 0.1);
	RajzolRangLista(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("Tarsoly", -1.4, 0.85, 0.1);
	RajzolTarsoly(vp);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("|Meret|", -1.4, 0.35, 0.1);

	Szovegrajzol("Ido", -1.4, -0.05, 0.1);
	KirajzolIdo(&vp->time, -1.4, -0.16, 0.1);

	Szovegrajzol("Pont", -1.4, -0.30, 0.1);
	KirajzolPont(&vp->pont, -1.4, -0.41, 0.1);


	Szovegrajzol("Szint", -1.4, -0.55, 0.1);
	KirajzolSzint(&vp->level, -1.4, -0.66, 0.1);

	Szovegrajzol("Sorok", -1.4, -0.8, 0.1);
	KirajzolEltSorSzam(vp, -1.4, -0.91, 0.1);
}