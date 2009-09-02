#include <SDL/SDL.h>

#include "../OpenGL.h"
#include "gl.h"

using namespace ggl;
using namespace ggl::ogl;

static ggl::ogl::OpenGL opengl;	// the default global opengl instance
static SDL_Display display;	// used by glut so far..
static void (*glutDisplayFunction)(void);
static Image2dRGB colorBuffer;
static SDL_TimerID	timer_id;	// timer id used by glutTimerFunc

//static unsigned int frames;	// used to measure fps .. should lock for thread safety...but who cares?

void (*_glutTimerFuncPointer) (int data);

Uint32 _glutTimerCallbackFunction(Uint32 interval, void* param)
{
	SDL_RemoveTimer(timer_id);	// should not be necessary
	_glutTimerFuncPointer((int)param);
	return 0;	// the timer is cancelled, if we return 0 -- doesn;t work(it seems)...need to call SDL_RemoveTimer
}

void glMatrixMode(GLenum mode)
{
	switch(mode) {
	case ::GL_PROJECTION:
		opengl.glMatrixMode(ogl::GL_PROJECTION);
		break;
	case ::GL_MODELVIEW:
		opengl.glMatrixMode(ogl::GL_MODELVIEW);
		break;
	case ::GL_TEXTURE:
		opengl.glMatrixMode(ogl::GL_TEXTURE);
		break;
	default:
		break;
	}
}

template<typename T>
static void matrixToArray(const Matrix4d& matrix, T* array)
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			array[i + j*4] = static_cast<T>(matrix(i,j));
}

template<typename T>
static void Getv(GLenum pname, T* params)
{
	switch(pname)
	{
	case GL_MODELVIEW_MATRIX:
		matrixToArray<T>(opengl.getModelViewMatrix(), params);
		return;
	case GL_PROJECTION_MATRIX:
		matrixToArray<T>(opengl.getProjectionMatrix(), params);
		return;
	case GL_TEXTURE_MATRIX:
		matrixToArray<T>(opengl.getTextureMatrix(), params);
		return;
	default:
		break;
	}

}

void glGetFloatv(GLenum pname, GLfloat *params)
{
	Getv<GLfloat>(pname, params);
}

void glGetDoublev(GLenum pname, GLdouble* params)
{
	Getv<GLdouble>(pname, params);
}

void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	opengl.glOrtho(left, right, bottom, top, zNear, zFar);
}

void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
	glOrtho(left, right, bottom, top, -1, 1);
}

void glClear(GLbitfield bufferMask)
{
	if(bufferMask & GL_COLOR_BUFFER_BIT)
		opengl.clearColorBuffer();
	if(bufferMask & GL_DEPTH_BUFFER_BIT)
		opengl.clearZBuffer();
}

void glLoadIdentity()
{
	opengl.glLoadIdentity();
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glTranslate(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glTranslate(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glRotate(static_cast<Real>(angle), static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glRotate(static_cast<Real>(angle), static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glutInit(int x, int y)	// TODO: allright, this shouldn't be here and should have different prototype
{
	opengl.init(x, y);
	display.init(x, y);
	colorBuffer.resize(x, y);
	opengl.setColorBuffer(display.getRGBCanvas());
	SDL_InitSubSystem(SDL_INIT_TIMER);	// whe should already have SDL initialized..
}

void glutTimerFunc(unsigned int msec, void (*func)(int data), int data)
{
	_glutTimerFuncPointer = func;
	timer_id = SDL_AddTimer(msec, _glutTimerCallbackFunction, (void*)data);
}

void glutDisplayFunc(void (*func)(void))
{
	glutDisplayFunction = func;
}

void glutMainLoop(void)
{
	bool done = false;
	SDL_Event event;
	glutPostRedisplay();
	while((!done) && SDL_WaitEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				done = true;
				break;

			case SDL_QUIT:
				done = true;
				break;

			default:
				break;

		}
	}
}

void glVertex2f(GLfloat x, GLfloat y)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y));
}

void glVertex2d(GLdouble x, GLdouble y)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y));
}

void glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z), static_cast<Real>(w));
}

void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z), static_cast<Real>(w));
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	opengl.glViewport(x, y, width, height);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glScale(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glScaled(GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glScale(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glEnable(GLenum cap)
{
	// TODO: checking for being outside begin..end should probably be here...not in every OpenGL::enable function..
	switch(cap) {
	case ::GL_CULL_FACE:
		opengl.enableCulling();
		break;
	case	::GL_NORMALIZE:
		opengl.enableNormalsNormalization();
		break;
	case	::GL_LIGHTING:
		opengl.enableLighting();
		break;
	case	::GL_LIGHT0:
		opengl.enableLight(0);
		break;
	case	::GL_LIGHT1:
		opengl.enableLight(1);
		break;
	case	::GL_LIGHT2:
		opengl.enableLight(2);
		break;
	case	::GL_LIGHT3:
		opengl.enableLight(3);
		break;
	case	::GL_LIGHT4:
		opengl.enableLight(4);
		break;
	case	::GL_LIGHT5:
		opengl.enableLight(5);
		break;
	case	::GL_LIGHT6:
		opengl.enableLight(6);
		break;
	case	::GL_LIGHT7:
		opengl.enableLight(7);
		break;
	case	::GL_LIGHT8:
		opengl.enableLight(8);
		break;
	case	::GL_LIGHT9:
		opengl.enableLight(9);
		break;
	case	::GL_LIGHT10:
		opengl.enableLight(10);
		break;
	case	::GL_LIGHT11:
		opengl.enableLight(11);
		break;
	default:
		break;
	}
}

void glDisable(GLenum cap)
{
	switch(cap) {
	case ::GL_CULL_FACE:
		opengl.disableCulling();
		break;
	case ::GL_NORMALIZE:
		opengl.disableNormalsNormalization();
		break;
	case	::GL_LIGHTING:
		opengl.disableLighting();
		break;
	case	::GL_LIGHT0:
		opengl.disableLight(0);
		break;
	case	::GL_LIGHT1:
		opengl.disableLight(1);
		break;
	case	::GL_LIGHT2:
		opengl.disableLight(2);
		break;
	case	::GL_LIGHT3:
		opengl.disableLight(3);
		break;
	case	::GL_LIGHT4:
		opengl.disableLight(4);
		break;
	case	::GL_LIGHT5:
		opengl.disableLight(5);
		break;
	case	::GL_LIGHT6:
		opengl.disableLight(6);
		break;
	case	::GL_LIGHT7:
		opengl.disableLight(7);
		break;
	case	::GL_LIGHT8:
		opengl.disableLight(8);
		break;
	case	::GL_LIGHT9:
		opengl.disableLight(9);
		break;
	case	::GL_LIGHT10:
		opengl.disableLight(10);
		break;
	case	::GL_LIGHT11:
		opengl.disableLight(11);
		break;
	default:
		break;
	}
}

void glColor3f(GLfloat r, GLfloat g, GLfloat b)
{
	opengl.glColor(r, g, b, 1);
}

void glColor3d(GLdouble r, GLdouble g, GLdouble b)
{
	opengl.glColor(r, g, b, 1);
}

void glNormal3f(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glNormal(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glNormal3d(GLdouble x, GLdouble y, GLdouble z)
{
	opengl.glNormal(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
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

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	opengl.gluPerspective(fovy, aspect, zNear, zFar);
}

void glBegin(GLenum mode){
	switch(mode)
	{
	case ::GL_POINTS:
		opengl.glBegin(ogl::GL_POINTS);
		break;

	case ::GL_LINES:
		opengl.glBegin(ogl::GL_LINES);
		break;

	case GLenum(::GL_POLYGON):
		opengl.glBegin(ogl::GL_POLYGON);
		break;

	case GLenum(::GL_TRIANGLES):
		opengl.glBegin(ogl::GL_TRIANGLES);
		break;

	case GLenum(::GL_QUADS):
		opengl.glBegin(ogl::GL_QUADS);
		break;

	case GLenum(::GL_LINE_STRIP):
		opengl.glBegin(ogl::GL_LINE_STRIP);
		break;

	case GLenum(::GL_LINE_LOOP):
		opengl.glBegin(ogl::GL_LINE_LOOP);
		break;

	case GLenum(::GL_TRIANGLE_STRIP):
		opengl.glBegin(ogl::GL_TRIANGLE_STRIP);
		break;

	case GLenum(::GL_TRIANGLE_FAN):
		opengl.glBegin(ogl::GL_TRIANGLE_FAN);
		break;

	case GLenum(::GL_QUAD_STRIP):
		opengl.glBegin(ogl::GL_QUAD_STRIP);
		break;

	default:
		break;
	}
}

void glShadeModel(GLenum mode)
{
	switch(mode) {
	case ::GL_SMOOTH:
		opengl.glShadeModel(ogl::GL_SMOOTH);
		break;

	case ::GL_FLAT:
		opengl.glShadeModel(ogl::GL_FLAT);
		break;

	default:
		break;
	}
}

void glCullFace(GLenum mode)
{
	switch(mode) {
	case ::GL_BACK:
		opengl.glCullFace(ogl::GL_BACK);
		break;

	case ::GL_FRONT:
		opengl.glCullFace(ogl::GL_FRONT);
		break;

	case ::GL_FRONT_AND_BACK:
		opengl.glCullFace(ogl::GL_FRONT_AND_BACK);
		break;

	default:
		break;
	}
}

void glFrontFace(GLenum mode)
{
	switch(mode) {
	case ::GL_CW:
		opengl.glFrontFace(ogl::GL_CW);
		break;

	case ::GL_CCW:
		opengl.glFrontFace(ogl::GL_CCW);
		break;

	default:
		break;
	}
}

void glutPostRedisplay()
{
	glutDisplayFunction();
}

void glEnd(){
	opengl.glEnd();
}

void glClearColor(float red, float green, float blue, float alpha){
	opengl.glClearColor(red, green, blue, alpha);
}

void glLightfv(GLenum light, GLenum pname, const GLfloat* params)
{
	int light_n;
	switch(light) {
	case ::GL_LIGHT0:
		light_n = 0;
		break;
	case ::GL_LIGHT1:
		light_n = 1;
		break;
	case ::GL_LIGHT2:
		light_n = 2;
		break;
	case ::GL_LIGHT3:
		light_n = 3;
		break;
	case ::GL_LIGHT4:
		light_n = 4;
		break;
	case ::GL_LIGHT5:
		light_n = 5;
		break;
	case ::GL_LIGHT6:
		light_n = 6;
		break;
	case ::GL_LIGHT7:
		light_n = 7;
		break;
	case ::GL_LIGHT8:
		light_n = 8;
		break;
	case ::GL_LIGHT9:
		light_n = 9;
		break;
	case ::GL_LIGHT10:
		light_n = 10;
		break;
	case ::GL_LIGHT11:
		light_n = 11;
		break;
	default:
		return;
	}

}
