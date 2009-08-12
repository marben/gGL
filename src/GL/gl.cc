#include <SDL/SDL.h>

#include "../OpenGL.h"
#include "gl.h"

using namespace ggl;

static ggl::OpenGL opengl;	// the default global opengl instance
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

void glVertex2f(float x, float y){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y));
}

void glVertex3f(float x, float y, float z){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glVertex4f(float x, float y, float z, float w){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z), static_cast<Real>(w));
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
	// TODO: make some sane mapping between gl defines and ggl enum or whatever...
	switch(mode)
	{
	case ::GL_POINTS:
		opengl.glBegin(ggl::GL_POINTS);
		break;

	case ::GL_LINES:
		opengl.glBegin(ggl::GL_LINES);
		break;

	case GLenum(::GL_POLYGON):
		opengl.glBegin(ggl::GL_POLYGON);
		break;

	case GLenum(::GL_TRIANGLES):
		opengl.glBegin(ggl::GL_TRIANGLES);
		break;

	case GLenum(::GL_QUADS):
		opengl.glBegin(ggl::GL_QUADS);
		break;

	case GLenum(::GL_LINE_STRIP):
		opengl.glBegin(ggl::GL_LINE_STRIP);
		break;

	case GLenum(::GL_LINE_LOOP):
		opengl.glBegin(ggl::GL_LINE_LOOP);
		break;

	case GLenum(::GL_TRIANGLE_STRIP):
		opengl.glBegin(ggl::GL_TRIANGLE_STRIP);
		break;

	case GLenum(::GL_TRIANGLE_FAN):
		opengl.glBegin(ggl::GL_TRIANGLE_FAN);
		break;

	case GLenum(::GL_QUAD_STRIP):
		opengl.glBegin(ggl::GL_QUAD_STRIP);
		break;

	default:
		break;
	}
}

void glShadeModel(GLenum mode)
{
	switch(mode){
	case ::GL_SMOOTH:
		opengl.glShadeModel(ggl::GL_SMOOTH);
		break;

	case ::GL_FLAT:
		opengl.glShadeModel(ggl::GL_FLAT);
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
