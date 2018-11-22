#include "megjelenites.h"

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


void Palyakirajzol() {
	int oszlopsz = 10;
	int sorsz = 15;
	GLfloat szelesseg = 0.75;
	GLfloat magassag = 1;
	
	GLfloat kezdoPozx = -0.75;
	GLfloat kezdoPozy = -0.95;
	GLfloat NegyzetM;
	if (oszlopsz > szelesseg) {
		NegyzetM = (magassag / sorsz) * 2 - 0.02;
	}
	else {
		NegyzetM = (szelesseg / oszlopsz) * 2 - 0.02;
	}
	GLfloat Ugras = (szelesseg / oszlopsz)*2;

	for (int i = 0; i < oszlopsz; i++) {
		for (int j = 0; j < sorsz; j++) {
			glPushMatrix();
			glTranslatef(kezdoPozx+Ugras*i, kezdoPozy+Ugras*j, 0.0f);
			glBegin(GL_QUADS);
				glVertex2f(0, 0);
				glVertex2f(0, NegyzetM);
				glVertex2f(NegyzetM, NegyzetM);
				glVertex2f(NegyzetM, 0);
			glEnd();
			glPopMatrix();
		}
	}
	glPushMatrix();
	glTranslatef(-0.95f, -0.75f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, NegyzetM);
	glVertex2f(NegyzetM, NegyzetM);
	glVertex2f(NegyzetM, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.95f, -0.75f+(sorsz-3)*Ugras, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, NegyzetM);
	glVertex2f(NegyzetM, NegyzetM);
	glVertex2f(NegyzetM, 0);
	glEnd();
	glPopMatrix();
}


