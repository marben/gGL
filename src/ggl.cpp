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

#include "ColorRGB.h"
#include "ColorRGBA.h"
#include "Image2d.h"
#include "Image.h"
#include "SDL_Display.h"
#include "GL/gl.h"
#include "PngWriter.h"
#include "Loader_Obj.h"

using namespace ggl;
using namespace std;

int angle = 0;
int delay = 100;

void printMatrix(float matrix[4])
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
			cout<<matrix[i*4 + j]<<"|";
		cout<<endl;
	}
}

ggl::Object3d cube;

void renderObject(const ggl::Object3d& obj)
{
//	glPushMatrix();
//	glTranslated(obj.x, obj.y, obj.z);
//	glRotated(obj.r, 0, 1, 0);
	glBegin(GL_TRIANGLES);
		for(unsigned int i = 0; i < obj.faces.size(); i++)
		{
			glNormal3f(obj.faces[i].normal.x, obj.faces[i].normal.y, obj.faces[i].normal.z);
			glVertex3f(obj.faces[i].a.x, obj.faces[i].a.y, obj.faces[i].a.z);
			glVertex3f(obj.faces[i].b.x, obj.faces[i].b.y, obj.faces[i].b.z);
			glVertex3f(obj.faces[i].c.x, obj.faces[i].c.y, obj.faces[i].c.z);
		}
	glEnd();
//	glPopMatrix();
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

   //glEnable(GL_CULL_FACE);

   glTranslatef(100, 100, 0);
   glRotatef(angle, 1, 1, 0);
   //glRotatef(13, 1, 1, 0);
   glShadeModel(GL_SMOOTH);
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
		glColor3f(1.0, 0, 0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(0.0, 1.0, 0);
		glVertex3f(0.5,50.5,0.2);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(130.5, 0.5, -0.2);
   glEnd();
   glTranslatef(100, 100, 0);

   glScalef(12, 12, 12);
   renderObject(cube);

   glFlush();

   //GLfloat matrix[16];
   //glGetFloatv(GL_PROJECTION_MATRIX, matrix);
   //printMatrix(matrix);
}

/* rotation angle for the triangle. */
float rtri = 0.0f;

/* rotation angle for the quadrilateral. */
float rquad = 0.0f;

void timerCallback(int data)
{
	angle += 2;
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
	Loader_Obj loader("M1.obj");
	cube = loader.getObject("M1");
	glutInit(640, 480);
	glutDisplayFunc(display2);
	glutTimerFunc(delay, timerCallback, 0);
	glClearColor(0.07, 0.1, 0, 1);
	glutMainLoop();

	return 0;
}
