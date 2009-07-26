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

using namespace ggl;
using namespace std;

int angle = 0;
int delay = 50;

void glTest()
{
	glLoadIdentity();

   glColor3f(1.0, 0.0, 0.0);

   glLoadIdentity();
   glTranslatef(320+angle, 240, 0);
   glRotatef(45+angle, 1, 0.7, 0);
   //glRotatef(45, 1, 1, 0);
   glBegin(GL_LINES);

   glColor3f(1, 0, 0);
	glVertex3f(-50, 50, -50); glVertex3f(50, 50, -50);
	glVertex3f(50, 50, -50); glVertex3f(50, -50, -50);
	glVertex3f(50, -50, -50); glVertex3f(-50, -50, -50);
	glVertex3f(-50, -50, -50); glVertex3f(-50, 50, -50);

	glColor3f(0, 1, 0);
	glVertex3f(-50, 50, 50); glVertex3f(50, 50, 50);
	glVertex3f(50, 50, 50); glVertex3f(50, -50, 50);
	glVertex3f(50, -50, 50); glVertex3f(-50, -50, 50);
	glVertex3f(-50, -50, 50); glVertex3f(-50, 50, 50);

	glColor3f(0, 0, 1);
	glVertex3f(-50, 50, -50); glVertex3f(-50, 50, 50);
	glVertex3f(50, 50, -50); glVertex3f(50, 50, 50);
	glVertex3f(50, -50, -50); glVertex3f(50, -50, 50);
	glVertex3f(-50, -50, -50); glVertex3f(-50, -50, 50);

   glEnd();
   glFlush();
}

void timerCallback(int data)
{
	angle += 1;
	glutTimerFunc(delay, timerCallback, 0);
}

int main()
{
//	SDL_Display display;
//	display.init(640, 480);
//	Image2dRGBA image(640,480);
//	image.clean(Black);
//	image.line(10, 10, 400, 200, Red);
//	display.displayImage(image);
//	display.redraw();
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
	glutMainLoop();
	//glTest();

	//sleep(3);

	return 0;
}
