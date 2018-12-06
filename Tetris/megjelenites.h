#ifndef MEGJELENITES_H
#define MEGJELENITES_H

//Megjelenítéshez, OpenGl tipusokhoz szükséges includok
#include "GL/glut.h"
#include <windows.h>

//Külső fájlok
#include "hand.h"
#include "map.h"

//Szín struktúra
typedef struct {
	float r;	//red
	float g;	//green
	float b;	//blue
	float a;	//alpha
} RGBA;

//OpenGL Grafikához szükséges inicializációk
void initGL();

//Játék belső időzítője
void Idozito(int idokoz);

/* Handler - windows re-size :
	Mikor az ablakot átméretezik, az arányok nem romlanak el. */
void Ujrameretez(GLsizei width, GLsizei height);

//Kis matek a megfelelő kirajzolásokhoz 
void KirajzInit(Palya *vp);

//Játék közbeni Kirajzolás
void JatekRajzol(Palya *vp, Hand *hp);

//Játék vége kirajzolás
void GameOverRajzol(Palya *vp, Hand *hp);

#endif