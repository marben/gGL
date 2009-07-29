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
typedef int sizei;
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

void glutPostRedisplay();

void glClear(GLbitfield bufferMask);

void glShadeModel(GLenum mode);

void glLoadIdentity();

void glTranslatef(GLfloat x, GLfloat y, GLfloat z);

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

void glutInit(int x, int y);	// TODO: allright, this shouldn't be here and should have different prototype

void glutDisplayFunc(void (*func)(void));

void glutTimerFunc(unsigned int msec, void (*func)(int data), int data);

void glutMainLoop(void);

void glVertex2f(float x, float y);

void glVertex3f(float x, float y, float z);

void glVertex4f(float x, float y, float z, float w);

void glColor3f(GLfloat r, GLfloat g, GLfloat b);

/*
 * glFlush — force execution of GL commands in finite time
 */
void glFlush();

void glBegin(GLenum mode);

void glEnd();

void glClearColor(float red, float green, float blue, float alpha);

#endif /* GL_H_ */
