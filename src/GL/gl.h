/*
 * gl.h
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#ifndef GL_H_
#define GL_H_

#include "gl_types.h"
#include "../SDL_Display.h"
#include "../Image.h"

typedef unsigned int GLenum;
typedef bool GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef float GLclampf;	// float clamped in <0;1>
typedef double	GLdouble;
typedef double GLclampd;
typedef unsigned int GLbitfield;

// TODO: do something about these ugly definitions
// maybe ogl standard defines the values? ... would make sense..
#define GL_COLOR_BUFFER_BIT ((GLbitfield) (1 << 0))
#define GL_DEPTH_BUFFER_BIT ((GLbitfield) (1 << 1))

const GLenum GL_SMOOTH = 1;
const GLenum GL_FLAT = 2;
const GLenum GL_TRIANGLES = 3;
const GLenum GL_LINES = 4;
const GLenum GL_POINTS = 5;
const GLenum GL_POLYGON = 6;
const GLenum GL_QUADS = 7;
const GLenum GL_LINE_STRIP = 8;
const GLenum GL_LINE_LOOP = 9;
const GLenum GL_TRIANGLE_STRIP = 10;
const GLenum GL_TRIANGLE_FAN = 11;
const GLenum GL_QUAD_STRIP = 12;
const GLenum GL_MODELVIEW = 13;
const GLenum GL_PROJECTION = 14;
const GLenum GL_TEXTURE = 15;
const GLenum GL_MODELVIEW_MATRIX = 16;
const GLenum GL_PROJECTION_MATRIX = 17;
const GLenum GL_TEXTURE_MATRIX = 18;
const GLenum GL_FRONT = 19;
const GLenum GL_BACK = 20;
const GLenum GL_FRONT_AND_BACK = 21;
const GLenum GL_CW = 22;
const GLenum GL_CCW = 23;
const GLenum GL_CULL_FACE = 24;
const GLenum GL_NORMALIZE = 25;
const GLenum GL_LIGHTING = 26;
const GLenum GL_LIGHT1 = 27;
const GLenum GL_LIGHT2 = 28;
const GLenum GL_LIGHT3 = 29;
const GLenum GL_LIGHT4 = 30;
const GLenum GL_LIGHT5 = 31;
const GLenum GL_LIGHT6 = 32;
const GLenum GL_LIGHT7 = 33;
const GLenum GL_LIGHT8 = 34;
const GLenum GL_LIGHT9 = 35;
const GLenum GL_LIGHT10 = 36;
const GLenum GL_LIGHT11 = 37;
const GLenum GL_LIGHT12 = 38;

void glutPostRedisplay();

void glClear(GLbitfield bufferMask);

void glShadeModel(GLenum mode);

void glLoadIdentity();

void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

void glTranslated(GLdouble x, GLdouble y, GLdouble z);

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

void glutInit(int x, int y);	// TODO: allright, this shouldn't be here and should have different prototype

void glutDisplayFunc(void (*func)(void));

void glutTimerFunc(unsigned int msec, void (*func)(int data), int data);

void glutMainLoop(void);

void glVertex2f(GLfloat x, GLfloat y);

void glVertex2d(GLdouble x, GLdouble y);

void glVertex3f(GLfloat x, GLfloat y, GLfloat z);

void glVertex3d(GLdouble x, GLdouble y, GLdouble z);

void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);

void glColor3f(GLfloat r, GLfloat g, GLfloat b);

void glColor3d(GLdouble r, GLdouble g, GLdouble b);

void glEnable(GLenum cap);

void glDisable(GLenum cap);

void glNormal3f(GLfloat x, GLfloat y, GLfloat z);

void glNormal3d(GLdouble x, GLdouble y, GLdouble z);

/*
 * glFlush — force execution of GL commands in finite time
 */
void glFlush();

void glGetFloatv(GLenum pname, GLfloat *params);

void glGetDoublev(GLenum pname, GLdouble *params);

void glBegin(GLenum mode);

void glMatrixMode(GLenum mode);

void glEnd();

void glClearColor(float red, float green, float blue, float alpha);

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

void glCullFace(GLenum mode);

void glFrontFace(GLenum mode);

void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void glScalef(GLfloat x, GLfloat y, GLfloat z);

void glScaled(GLdouble x, GLdouble y, GLdouble z);

#endif /* GL_H_ */
