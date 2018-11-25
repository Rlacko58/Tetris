#include "megjelenites.h"

#include "stdio.h"

//Debughoz
#include "debugmalloc.h"

//Belső globális változók
static int Frissites_ms = 10; // Képfrissítési időköz milliszekundumokban 

//OpenGL Grafikához szükséges inicializációk
void initGL() {
	//Törléskor használt szin
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fekete
}

// "Timer"
void Idozito(int idokoz) {
	glutPostRedisplay(); //Jelenlegi ablak újrarajzolandónak jelölése
	glutTimerFunc(Frissites_ms, Idozito, 0); // Időzítő indítása megint
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

void KirajzInit(Palya *vp) {		/* Kis matek a megfelelő kirajzolásokhoz */
	float Nmx = 0.1f;				//Maximális négyzet méret
	float Nps = 2.0f / vp->oszlop;	//Négyzet / sor
	float Npo = 2.0f / vp->sor;		//Négyzet / oszlop
	float Nsize = 0.0f;				//Négyzet végső mérete
	if (Nps < Npo)	Nsize = Nps;
	else			Nsize = Npo;

	if (Nsize > Nmx) Nsize = Nmx;
	vp->Nsize = Nsize;
	vp->width = Nsize * vp->oszlop;
	vp->height = Nsize * vp->sor;
}

void Szovegrajzol(char* szoveg, GLfloat x, GLfloat y, float meret){
	glPushMatrix();	
	glTranslatef(x, y, 0);	//Megfelelo poziciora rajzolas
	glScalef(meret / 152.38, meret / 152.38, 0);	//Megfelelo meret beallitasa

	for (char* p = szoveg; *p; p++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p); //Kirajzolas
	}
	glPopMatrix();
}

static void RajzolNegyzet(GLfloat *Nsize, GLfloat x, GLfloat y, RGBA s, bool mode) {
	if (mode) {
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

static void RajzolHaloba(Palya *vp, int sor, int oszlop, RGBA s, bool mode) {
	RajzolNegyzet(&vp->Nsize,
		-1.0 + (2.0 - vp->width) / 2.0 + oszlop * vp->Nsize,
		1.0 - (2.0 - vp->height) / 2.0 - (1+sor) * vp->Nsize,
		s, mode);
}

static RGBA SzinKonverter(int szin) {
	switch (szin) {
	case 1: return (RGBA) { 1.0, 0.0, 0.0, 0.0 }; break;	//red
	case 2: return (RGBA) { 0.0, 1.0, 0.0, 0.0 }; break;	//green
	case 3: return (RGBA) { 0.0, 0.0, 1.0, 0.0 }; break;	//blue
	case 4: return (RGBA) { 1.0, 0.7, 0.0, 0.0 }; break;	//orange
	case 5: return (RGBA) { 1.0, 1.0, 0.0, 0.0 }; break;	//yellow
	case 6: return (RGBA) { 0.4, 0.2, 0.6, 0.0 }; break;	//purple
	case 7: return (RGBA) { 0.0, 0.7, 0.8, 0.0 }; break;	//cyan
	default: return (RGBA) { 0.0, 0.0, 0.0, 0.0};
	}
}

static void RajzolMatrix(Palya *vp) {
	for (int i = 0; i < vp->sor; i++) {
		for (int j = 0; j < vp->oszlop; j++) {
			if (vp->v[IND(i,j,vp->oszlop)].e) {
				RajzolHaloba(vp, i, j, SzinKonverter(vp->v[IND(i,j,vp->oszlop)].c),0);
			}
			else {
				RajzolHaloba(vp, i, j, SzinKonverter(0),0);
			}
		}
	}
}

static void RajzolTetris(Palya *vp, Hand *hp) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (hp->v[IND(i, j, hp->size)])
				RajzolHaloba(vp, hp->x + i, hp->y + j, SzinKonverter(hp->color), 0);
}

static void RajzolAlTetris(Palya *vp, Hand *hp, int x) {
	for (int i = 0; i < hp->size; i++)
		for (int j = 0; j < hp->size; j++)
			if (hp->v[IND(i, j, hp->size)])
				RajzolHaloba(vp, hp->x + i + x, hp->y + j, SzinKonverter(hp->color), 1);
}

int waitclock=10;

void Kirajzol(Palya *vp, Hand *hp) {
	RajzolMatrix(vp);
	RajzolAlTetris(vp, hp, AltetrisKord(vp, hp));
	RajzolTetris(vp, hp);
	
	
	

	glColor4f(1.0, 1.0, 1.0, 1.0);
	Szovegrajzol("HOLD", -1.3, 0.85, 0.09);

	/*	glPushMatrix();
		glTranslatef(-0.95f, -0.75f+(sorsz-3)*Ugras, 0.0f);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, NegyzetM);
		glVertex2f(NegyzetM, NegyzetM);
		glVertex2f(NegyzetM, 0);
		glEnd();
		glPopMatrix();	*/
}


