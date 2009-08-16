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
int delay = 200;

void printMatrix(float matrix[4])
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			cout<<matrix[i*4 + j]<<"|";
		cout<<endl;
	}

}

void glTest()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25, 25, -25, 25, -30, 30);	// should not be here..

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glColor3f(1.0, 0.0, 0.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

  /*
   glTranslatef(320, 240, 0);

   glRotatef(45+angle, 1, 0.7, 0.4);
   glShadeModel(GL_SMOOTH);
   glBegin(GL_TRIANGLES);

	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 200, 0);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(100, 50, -10);

	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0, 0, -20);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 200, -20);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(100, 50, 10);

   glEnd();

   glShadeModel(GL_SMOOTH);
//   glRotatef(45+angle, 1, 0.7, 0.4);
   //glRotatef(45, 1, 1, 0);
   glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-50, 50, -50); glColor3f(0, 0, 1); glVertex3f(50, 50, -50);
		glColor3f(1, 0, 0);
		glVertex3f(50, 50, -50); glVertex3f(50, -50, -50);
		//glColor3f(1, 0, 0);
		glVertex3f(50, -50, -50); glVertex3f(-50, -50, -50);
		glVertex3f(-50, -50, -50); glVertex3f(-50, 50, -50);

		glColor3f(0, 0, 1);
		glVertex3f(-50, 50, 50); glVertex3f(50, 50, 50);
		glVertex3f(50, 50, 50); glVertex3f(50, -50, 50);
		glVertex3f(50, -50, 50); glVertex3f(-50, -50, 50);
		glVertex3f(-50, -50, 50); glVertex3f(-50, 50, 50);

	glEnd();
	glShadeModel(GL_FLAT);
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(-50, 50, -50); glVertex3f(-50, 50, 50);
		glVertex3f(50, 50, -50); glVertex3f(50, 50, 50);
		glVertex3f(50, -50, -50); glVertex3f(50, -50, 50);
		glVertex3f(-50, -50, -50);
		glColor3f(1, 0, 1);
		glVertex3f(-50, -50, 50);
   glEnd();
   */

   glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(-5, 5, -5); glColor3f(0, 0, 1); glVertex3f(5, 5, -5);
		glColor3f(1, 0, 0);
		glVertex3f(5, 5, -5); glVertex3f(5, -5, -5);
		//glColor3f(1, 0, 0);
		glVertex3f(5, -5, -5); glVertex3f(-5, -5, -5);
		glVertex3f(-5, -5, -5); glVertex3f(-5, 5, -5);

		glColor3f(0, 1, 0);
		glVertex3f(-5, 5, 5); glVertex3f(5, 5, 5);
		glVertex3f(5, 5, 5); glVertex3f(5, -5, 5);
		glVertex3f(5, -5, 5); glVertex3f(-5, -5, 5);
		glVertex3f(-5, -5, 5); glVertex3f(-5, 5, 5);

	glEnd();
	glShadeModel(GL_FLAT);
	glBegin(GL_LINES);
		glColor3f(0, 1, 0);
		glVertex3f(-5, 5, -5); glVertex3f(-5, 5, 5);
		glVertex3f(5, 5, -5); glVertex3f(5, 5, 5);
		glVertex3f(5, -5, -5); glVertex3f(5, -5, 5);
		glVertex3f(-5, -5, -5);
		glColor3f(1, 0, 1);
		glVertex3f(-5, -5, 5);
   glEnd();

   glFlush();

   float matrix[16];
   glGetFloatv(GL_PROJECTION_MATRIX, matrix);
   printMatrix(matrix);
}

void display2(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, 640, 480);
	gluOrtho2D(0, 640, 0, 480);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 0.0, 0.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glShadeModel(GL_SMOOTH);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
		glColor3f(1.0, 0, 0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(0.0, 1.0, 0);
		glVertex3f(639.5,239.5,0.2);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.5, 479.5, -0.2);
   glEnd();
   glFlush();

   //GLfloat matrix[16];
   //glGetFloatv(GL_PROJECTION_MATRIX, matrix);
   //printMatrix(matrix);
}

/* rotation angle for the triangle. */
float rtri = 0.0f;

/* rotation angle for the quadrilateral. */
float rquad = 0.0f;
void nehe05()
{
	  glViewport(0, 0, 640, 480);		// Reset The Current Viewport And Perspective Transformation

	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();

	  gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);
	  glMatrixMode(GL_MODELVIEW);

	  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	  glLoadIdentity();				// Reset The View

	  glTranslatef(0.0f,0.0f,-2.0f);		// Move Left 1.5 Units And Into The Screen 6.0

	  glRotatef(rtri,0.0f,1.0f,0.0f);		// Rotate The Pyramid On The Y axis

	  // draw a pyramid (in smooth coloring mode)
	  glShadeModel(GL_SMOOTH);
	  glBegin(GL_TRIANGLES);				// start drawing a pyramid

	  // front face of pyramid
	  glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
	  glVertex3f(-0.4f, 0.4f, 0.0f);		        // Top of triangle (front)
	  glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
	  glVertex3f(0.5f,0.0f, 0.0f);		// left of triangle (front)
	  glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
	  glVertex3f(-0.4f,-0.4f, 0.0f);		        // right of traingle (front)


	  glEnd();					// Done Drawing The Pyramid



	  rtri+=15.0f;					// Increase The Rotation Variable For The Pyramid
	  rquad-=15.0f;					// Decrease The Rotation Variable For The Cube

	  glFlush();

	  // float matrix[16];
	  // glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	  // printMatrix(matrix);
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
	glutDisplayFunc(nehe05);
	glutTimerFunc(delay, timerCallback, 0);
	glClearColor(0.07, 0.1, 0, 1);
	glutMainLoop();

	return 0;
}
