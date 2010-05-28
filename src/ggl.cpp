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

int width = 640, height = 480;

int nearPlane = 1, farPlane = 1000;

int angle = 100;
int delay = 20;

//GLfloat lightPosition[] = {0.0, 7.0, 0.0, 1.0}; // smerove svetlo - nula na konci (v pripade jednicky by to bylo bodove svetlo)

GLfloat robotAmbient[]={0.4, 0.4, 0.8, 1.0};
GLfloat robotDiffuse[]={0.8, 0.4, 0.4, 1.0};
GLfloat robotSpecular[]={0.0, 1.0, 1.0, 1.0};
GLfloat robotShininess[]={25.0};

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

void setSanePerspective()
{
	glViewport(0, 0, 640, 480);	// nemelo by byt volano porad dokola
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)width/(GLdouble)height,
			nearPlane, farPlane);
}

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
   glDisable(GL_LIGHTING);
   glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
		glColor3f(1.0, 0, 0);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3f(0.0, 1.0, 0);
		glVertex3f(0.5,50.5,0.2);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(130.5, 0.5, -0.2);
   glEnd();

   glTranslatef(20, 20, 0);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glTranslatef(100, 100, 0);
   glBegin(GL_TRIANGLES);
		glColor3f(0.0, 1.0, 0);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, robotDiffuse);
		glVertex3f(0.5, 0.5, 0.0);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, robotAmbient);
		glVertex3f(50.5,50.5,0.2);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, robotSpecular);
		glVertex3f(10.5, 70.5, -0.2);
   glEnd();

   glTranslatef(100, 100, 0);
   glScalef(20, 20, 20);
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

GLfloat glBlack[] = {0.0, 0.0, 0.0, 1.0};
GLfloat glWhite[] = {1.0, 1.0, 1.0, 1.0};
GLfloat glRed[] = {1.0, 0.0, 0.0, 1.0};
GLfloat glGreen[] = {0.0, 1.0, 0.0, 1.0};
GLfloat glBlue[] = {0.0, 0.0, 1.0, 1.0};
GLfloat glTmp[] = {0.5, 0.5, 0.0, 1.0};

GLfloat lightPosition[] = {0, 0, 1, 0.0};

#define X .525731112119133606
#define Z .850650808352039932
static GLfloat vdata[12][3] = {
  {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
  {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
  {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static GLuint tindices[20][3] = {
  {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
  {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
  {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
  {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
};

void normalize(float v[3]) {
  GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  v[0] /= d; v[1] /= d; v[2] /= d;
}

void normCrossProd(float u[3], float v[3], float out[3]) {
  out[0] = u[1]*v[2] - u[2]*v[1];
  out[1] = u[2]*v[0] - u[0]*v[2];
  out[2] = u[0]*v[1] - u[1]*v[0];
  normalize(out);
}

GLfloat d1[3], d2[3], n[3];
void icoNormVec (int i) {
  for (int k = 0; k < 3; k++) {
    d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
    d2[k] = vdata[tindices[i][1]] [k] - vdata[tindices[i][2]] [k];
  }
  normCrossProd(d1, d2, n);
  glNormal3fv(n);
}

void drawIcosahedron()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 20; i++) {
	  icoNormVec(i);
	  glVertex3fv(&vdata[tindices[i][0]] [0]);
	  glVertex3fv(&vdata[tindices[i][1]] [0]);
	  glVertex3fv(&vdata[tindices[i][2]] [0]);
	}
	glEnd();
}

void display5()
{
	glViewport(0,0,640,480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-319.5, 319.5, -239.5, 239.5, -500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glBlack);

	glRotatef(45, 1, 0, 0);
	glBegin(GL_TRIANGLES);
		glMaterialfv(GL_FRONT, GL_AMBIENT, glGreen);
		glMaterialfv(GL_BACK, GL_AMBIENT, glRed);
		glVertex3f(-100, 100, 0);
		glVertex3f(100, 100, 0);
		glVertex3f(0, -100, 0);
	glEnd();

	glFlush();
}

void display4()
{
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, -500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glTranslated(200, 200, 0);
	//glNormal3f(0,0,1);

	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_AMBIENT, glBlack);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glWhite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glBlack);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glRotatef(angle, 1, 1, 0);
	//glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	/*
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(100.0, 0.0, 0.0);
		glVertex3f(0.0, 150.0, 0.0);
	glEnd();
	*/

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glWhite);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glBlack);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	glScalef(100, 100, 100);
	drawIcosahedron();

	glFlush();
}

void displayTriangles()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setSanePerspective();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	GLfloat lightPosition[] = {0.0, 0.0, 2.0, 1.0};
	GLfloat lightColor[] = {1.0, 1.0, 1.0, 1.0};

	GLfloat material1[]={1.0, 0.0, 0.0, 1.0};
	GLfloat material2[]={0.0, 1.0, 0.0, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glBlack);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glBlack);

	glLightfv(GL_LIGHT0, GL_AMBIENT, glBlack);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glBlack);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// just a test

	//glMatrixMode(GL_PROJECTION_MATRIX);
	//glLoadIdentity();

	// end of test

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	/*
	   GLfloat matrix[16];
	   glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	   std::cout<<"modelview matrix:"<<std::endl;
	   printMatrix(matrix);


	   glGetFloatv(GL_PROJECTION_MATRIX, matrix);
	   std::cout<<"projection matrix:"<<std::endl;
	   printMatrix(matrix);
	   */

	glColor3f(1,1,0);
	glBegin(GL_TRIANGLES);
		glVertex3f(-1, -1, -10);
		glVertex3f(1, 1, -10);
		glVertex3f(-1, 1, -10);
	glEnd();


	//glRotatef(angle, 0 ,0, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material2);
	glColor3f(0,0.3,0.77);
	glBegin(GL_TRIANGLES);
		glVertex3f(1, -1, -9);
		glVertex3f(1, 1, -9);
		glVertex3f(-1, 1, -9);
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
	//Loader_Obj loader("M1.obj");
	//cube = loader.getObject("M1");
	glutInit(640, 480);
	glutDisplayFunc(display4);
	glutTimerFunc(delay, timerCallback, 0);
	glClearColor(0.07, 0.1, 0, 1);
	glutMainLoop();

	return 0;
}
