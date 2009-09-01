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

void glVertex2f(float x, float y){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y));
}

void glVertex3f(float x, float y, float z){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z));
}

void glVertex4f(float x, float y, float z, float w){
	opengl.glVertex4(static_cast<Real>(x), static_cast<Real>(y), static_cast<Real>(z), static_cast<Real>(w));
}

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	opengl.glViewport(x, y, width, height);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
	opengl.glScale(x, y, z);
}

void glEnable(GLenum cap)
{
	// TODO: checking for being outside begin..end should probably be here...not in every OpenGL::enable function..
	//opengl.enableCulling(true);
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
	default:
		break;
	}
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

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	opengl.gluPerspective(fovy, aspect, zNear, zFar);
}

void glBegin(GLenum mode){
	// TODO: make some sane mapping between gl defines and ggl enum or whatever...
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
