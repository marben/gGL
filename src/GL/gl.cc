#include "../OpenGL.h"
#include "gl.h"

using namespace ggl;

static ggl::OpenGL opengl;	// the default global opengl instance
static SDL_Display display;	// used by glut so far..
static void (*glutDisplayFunction)(void);
static Image2dRGB colorBuffer;

//static unsigned int frames;	// used to measure fps .. should lock for thread safety...but who cares?


void glLoadIdentity()
{
	opengl.glLoadIdentity();
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glTranslated(x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glRotated(angle, x, y, z);
}

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glRotated(angle, x, y, z);
}

void glutInit(int x, int y)	// TODO: allright, this shouldn't be here and should have different prototype
{
	opengl.init(x, y);
	display.init(x, y);
	colorBuffer.resize(x, y);
	opengl.setColorBuffer(display.getRGBCanvas());
}

void glutDisplayFunc(void (*func)(void))
{
	glutDisplayFunction = func;
}

void glutMainLoop(void)
{
	for(int i=0; i<200; ++i)
		glutDisplayFunction();
}

void glVertex2f(float x, float y){
	opengl.glVertex4d(double(x), double(y));
}

void glVertex3f(float x, float y, float z){
	opengl.glVertex4d(double(x), double(y), double(z));
}

void glVertex4f(float x, float y, float z, float w){
	opengl.glVertex4d(double(x), double(y), double(z), double(w));
}

void glColor3f(GLfloat r, GLfloat g, GLfloat b)
{
	opengl.glColor(r, g, b, 1);
}

/*
 * glFlush — force execution of GL commands in finite time
 */
void glFlush()
{
	// should set GL_INVALID_OPERATION, if called between glBegin() and gEnd()

	display.beginWrite();
		opengl.glFlush();
	display.endWrite();

	display.redraw();
}

void glBegin(GLenum mode){
	switch(mode)
	{
	case GLenum(ggl::GL_POINTS):
		opengl.glBegin(ggl::GL_POINTS);
		break;

	case GLenum(ggl::GL_LINES):
		opengl.glBegin(ggl::GL_LINES);
		break;

	case GLenum(ggl::GL_POLYGON):
		opengl.glBegin(ggl::GL_POLYGON);
		break;

	case GLenum(ggl::GL_TRIANGLES):
		opengl.glBegin(ggl::GL_TRIANGLES);
		break;

	case GLenum(ggl::GL_QUADS):
		opengl.glBegin(ggl::GL_QUADS);
		break;

	case GLenum(ggl::GL_LINE_STRIP):
		opengl.glBegin(ggl::GL_LINE_STRIP);
		break;

	case GLenum(ggl::GL_LINE_LOOP):
		opengl.glBegin(ggl::GL_LINE_LOOP);
		break;

	case GLenum(ggl::GL_TRIANGLE_STRIP):
		opengl.glBegin(ggl::GL_TRIANGLE_STRIP);
		break;

	case GLenum(ggl::GL_TRIANGLE_FAN):
		opengl.glBegin(ggl::GL_TRIANGLE_FAN);
		break;

	case GLenum(ggl::GL_QUAD_STRIP):
		opengl.glBegin(ggl::GL_QUAD_STRIP);
		break;

	default:
		break;
	}
}

void glEnd(){
	opengl.glEnd();
}

void glClearColor(float red, float green, float blue, float alpha){
	opengl.glClearColor(red, green, blue, alpha);
}
