#ifndef MEGJELENITES_H
#define MEGJELENITES_H

//Megjelenítéshez, OpenGl tipusokhoz szükséges includok
#include "GL/glut.h"
#include <windows.h>


#include "hand.h"
#include "map.h"

typedef struct {
	float r;
	float g;
	float b;
	float a;
} RGBA;

void Ujrameretez(GLsizei width, GLsizei height);
void Idozito(int idokoz);
void initGL();
void KirajzInit(Palya *vp);
void Kirajzol(Palya *vp, Hand *hp);

#endif