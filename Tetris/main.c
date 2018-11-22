#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//Megjelenítéshez szükséges includok
#include "GL/glut.h"
#include <windows.h>

//Debughoz
#include "debugmalloc.h"

//Külső fájlok
#include "map.h"
#include "hand.h"
#include "megjelenites.h"

GLfloat angle = 0.0f;

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
	glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
	glLoadIdentity();               // Reset the model-view matrix

	Palyakirajzol();

	glutSwapBuffers();   // Double buffered - swap the front and back buffers

	// Change the rotational angle after each display()
	angle += 2.0f;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key -- Fejlesztéshez egy kis tweak
		glutDestroyWindow(1);
		return 0;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
	glutInitWindowSize(700, 700);   // Set the window's initial width & height - non-square
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Animation via Idle Function");  // Create window with the given title
	glutDisplayFunc(display);       // Register callback handler for window re-paint event
	glutReshapeFunc(Ujrameretez);       // Register callback handler for window re-size event
	glutTimerFunc(0, Idozito, 0);     // First timer call immediately
	glutKeyboardFunc(keyboard);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing loop
	return 0;
}