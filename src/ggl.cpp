//============================================================================
// Name        : ggl.cpp
// Author      : Martin Benes (GiX)
// Version     :
// Copyright   :
// Description : ggl graphics library
//============================================================================

#include <iostream>
#include <stdexcept>
using namespace std;

#include "PixelRGB.h"
#include "PixelRGBA.h"
#include "Image2d.h"
#include "Image.h"
#include "SDL_Display.h"
#include "GL/gl.h"
#include "PngWriter.h"

//using namespace ggl;
using namespace std;

int angle = 0;
int delay = 20;

void glTest()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(1.0, 0.0, 0.0);

   glLoadIdentity();
   glTranslatef(320, 240, 0);

   glRotatef(45+angle, 1, 0.7, 0.4);
   glShadeModel(GL_SMOOTH);
   glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(10, 10, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(200, 200, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(150, 220, 0);
   glEnd();

   //glShadeModel(GL_FLAT);
   //glRotatef(45+angle, 1, 0.7, 0.4);
   //glRotatef(45, 1, 1, 0);
   glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-50, 50, -50); glColor3f(0, 0, 1); glVertex3f(50, 50, -50);
		glColor3f(1, 0, 0);
		glVertex3f(50, 50, -50); glVertex3f(50, -50, -50);
		//glColor3f(1, 0, 0);
		glVertex3f(50, -50, -50); glVertex3f(-50, -50, -50);
		glVertex3f(-50, -50, -50); glVertex3f(-50, 50, -50);

		glColor3f(0, 1, 0);
		glVertex3f(-50, 50, 50); glVertex3f(50, 50, 50);
		glVertex3f(50, 50, 50); glVertex3f(50, -50, 50);
		glVertex3f(50, -50, 50); glVertex3f(-50, -50, 50);
		glVertex3f(-50, -50, 50); glVertex3f(-50, 50, 50);

	glEnd();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(-50, 50, -50); glVertex3f(-50, 50, 50);
		glVertex3f(50, 50, -50); glVertex3f(50, 50, 50);
		glVertex3f(50, -50, -50); glVertex3f(50, -50, 50);
		glVertex3f(-50, -50, -50);
		glColor3f(1, 0, 1);
		glVertex3f(-50, -50, 50);
   glEnd();
   glFlush();
}

void timerCallback(int data)
{
	angle += 1;
	if(angle >= 360)
		angle -= 360;

	if(angle < 0)
		angle += 360;

	glutPostRedisplay();
	glutTimerFunc(delay, timerCallback, 0);
}

int main()
{
	/*
	PngWriter writer("test.png");

	try{
		writer.write(image);
	}
	catch (const std::exception& error){
		std::cerr<<"Problem saving file:"<<error.what()<<std::endl;
	}
	*/

	glutInit(640, 480);
	glutDisplayFunc(glTest);
	glutTimerFunc(delay, timerCallback, 0);
	glClearColor(0.07, 0.1, 0, 1);
	glutMainLoop();

	return 0;
}
