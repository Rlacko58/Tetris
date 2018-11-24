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

void Szovegrajzol(char* szoveg, GLfloat x, GLfloat y, float meret){
	glPushMatrix();	
	glTranslatef(x, y, 0);	//Megfelelo poziciora rajzolas
	glScalef(meret / 152.38, meret / 152.38, 0);	//Megfelelo meret beallitasa

	for (char* p = szoveg; *p; p++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p); //Kirajzolas
	}
	glPopMatrix();
}

static void RajzolNegyzet(GLfloat x, GLfloat y, RGBA s) {
	glPushMatrix();
	glColor4f(s.r, s.g, s.b, s.a);
	glTranslatef(x, y, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, 0.1);
	glVertex2f(0.1, 0.1);
	glVertex2f(0.1, 0);
	glEnd();
	glPopMatrix();
}

static void RajzolPalya() {
	RajzolNegyzet(-1, 0.9, (RGBA) { 0.0, 1.0, 0.0 , 1.0});
}

void Kirajzol() {

	Szovegrajzol("HOLD", -1.3, 0.85, 0.09);
	RajzolNegyzet(-1, 0.9, (RGBA) { 0.0, 1.0, 0.0, 1.0 });




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


