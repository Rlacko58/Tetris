#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Megjelenítéshez szükséges includok
#include "GL/glut.h"
#include <windows.h>

//Debughoz
#include "debugmalloc.h"

//Külső fájlok
#include "map.h"
#include "hand.h"
#include "megjelenites.h"

static Palya t;
static Hand h;
static int GravInterv = 0;


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
	glLoadIdentity();               // Reset the model-view matrix
	
	if (GravInterv == 100) {
		GravInterv = 0;
		if (!Utkozes(&t, &h, &h.v[0], 1, 0)) h.x += 1;
		else {
			MatrixbaMasol(&t, &h);
			free(h.v);
			HandInit(&h, &t.oszlop, rand()%7); //DONT FORGET THIS ***44!
		}
	}
			
	Kirajzol(&t, &h);

	GravInterv++;
	

	glutSwapBuffers();   // Double buffered - swap the front and back buffers

	// Change the rotational angle after each display()
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key){
		case 27: // Escape -- Fejlesztéshez egy kis tweak
			glutDestroyWindow(1); 
		break;
		case 32: // Szóköz -- Letesz AlTetris helyére
			h.x += AltetrisKord(&t, &h);
			MatrixbaMasol(&t, &h);
			free(h.v);
			HandInit(&h, &t.oszlop, rand() % 7); //DONT FORGET THIS ***44!
			break;
	}
	glutPostRedisplay();
}

void specialKeys(unsigned char key, int x, int y)
{
	bool* forgatott;
	switch (key) {
		case GLUT_KEY_UP: 
			 forgatott = Forgat_jobbra(&h);
			 if (!Utkozes(&t, &h, &forgatott[0], 0, 0)) {
				 free(h.v);
				 h.v = forgatott;
			 }
			 else {
				 free(forgatott);
			 }
			break;
		case GLUT_KEY_DOWN:
			if (!Utkozes(&t, &h, &h.v[0],1,0)) {
				h.x += 1;
			}
			break;
		case GLUT_KEY_LEFT:
			if (!Utkozes(&t, &h, &h.v[0], 0, -1)) {
				h.y += -1;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (!Utkozes(&t, &h, &h.v[0], 0, 1)) {
				h.y += 1;
			}
			break;
	}
	glutPostRedisplay();
}



int main(int argc, char** argv) {
	srand(time(NULL));
	MatrixInit(&t, 20, 20);
	KirajzInit(&t);

	HandInit(&h, &t.oszlop, 3);

	glutInit(&argc, argv);          // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
	glutInitWindowSize(1000, 700);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(
		(glutGet(GLUT_SCREEN_WIDTH) - 1000) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - 700) / 2); 
	glutCreateWindow("Tetris játék");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(Ujrameretez);       // Register callback handler for window re-size event
	glutTimerFunc(0, Idozito, 0);     // First timer call immediately
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}