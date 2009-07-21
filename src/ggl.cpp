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

void glTest()
{
	glutInit(640, 480);

   glRotatef(45, 1, 1, 0);
//   glRotatef(45, 0, 0, 1);
	glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(100, 100, 0); glVertex3f(200, 100, 0);
		glVertex3f(200, 100, 0); glVertex3f(200, 200, 0);
		glVertex3f(200, 200, 0); glVertex3f(100, 200, 0);
		glVertex3f(100, 200, 0); glVertex3f(100, 100, 0);

		glColor3f(0, 1, 0);
		glVertex3f(100, 100, 100); glVertex3f(200, 100, 100);
		glVertex3f(200, 100, 100); glVertex3f(200, 200, 100);
		glVertex3f(200, 200, 100); glVertex3f(100, 200, 100);
		glVertex3f(100, 200, 100); glVertex3f(100, 100, 100);

		glColor3f(0, 0, 1);
		glVertex3f(100, 100, 0); glVertex3f(100, 100, 100);
		glVertex3f(200, 100, 0); glVertex3f(200, 100, 100);
		glVertex3f(200, 200, 0); glVertex3f(200, 200, 100);
		glVertex3f(100, 200, 0); glVertex3f(100, 200, 100);
	glEnd();
	glFlush();
}

int main()
{
//	SDL_Display display;
//	display.init(640, 480);
	Image2dRGBA image(640,480);
	image.clean(Black);
	image.line(10, 10, 400, 200, Red);
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
	glTest();

	sleep(3);

	return 0;
}
