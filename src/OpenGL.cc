/*
 * OpenGL.cc
 *
 *  Created on: Jul 14, 2009
 *      Author: gix
 */

#include "OpenGL.h"
#include <iostream>
#include <cassert>
#include <cmath>

#define PI 3.1415926535897932384626433832795	// tell me of a better way...
inline double radian(const double degree)
{
	return degree*(static_cast<double>(PI)/180.0);
}

namespace ggl
{

void OpenGL::glRotated(double angle, double x, double y, double z)
{
	/*
	 * form man page:
	 *
	  glRotate produces a rotation of angle	degrees	around the
	  vector (x,y,z).  The current matrix (see glMatrixMode) is
	  multiplied by	a rotation matrix with the product replacing
	  the current matrix, as if glMultMatrix were called with the
	  following matrix as its argument:

		  ( xx(1-c)+c	xy(1-c)-zs  xz(1-c)+ys	 0  )
		  |					    |
		  | yx(1-c)+zs	yy(1-c)+c   yz(1-c)-xs	 0  |
		  | xz(1-c)-ys	yz(1-c)+xs  zz(1-c)+c	 0  |
		  |					    |
		  (	 0	     0		 0	 1  )
	  Where	c = cos(angle),	s = sine(angle), and ||( x,y,z )|| = 1
	  (if not, the GL will normalize this vector).
*/

	// TODO: need to distinguish between projection/model/texture matrix
	// TODO: optimize by checking, whether we are rotating around one base axis, which doesn't require whole matrix construction
	// TODO: check the Mesa implementation in m_matrix.c function _math_matrix_rotate(...)   some neat tricks maybe?


	Vector3d vector(x, y, z);
	vector.normalize();

	x = vector.x();
	y = vector.y();
	z = vector.z();

	double angle_rad = radian(angle);

	double c = cos(angle_rad);
	double s = sin(angle_rad);

	Matrix4d matrix;
/*	matrix << 	x*x*(1-c)+c, x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
					y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s, 0,
					x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c, 0,
					0, 0, 0, 1; */


	// TODO: measure, whether this precomputation makes sense..
	double c1 = 1 - c;
	double xyc1 = x * y * c1;
	double xzc1 = x * z * c1;
	double yzc1 = y * z * c1;

	matrix <<	x*x*c1+c, xyc1-z*s, xzc1+y*s, 0,
					xyc1+z*s, y*y*c1+c, yzc1-x*s, 0,
					xzc1-y*s, yzc1+x*s, z*z*c1+c, 0,
					0, 0, 0, 1;

	_projection *= matrix;
}

void OpenGL::drawLines()
{
	size_t top;
	if((_linesVertexList.size() % 2) != 0)	// check if we have correct number of vertices for our lines
		top = _linesVertexList.size() - 1;
	else
		top = _linesVertexList.size();

	for(size_t i = 0; i < top; ++i)
	{
		// TODO should probably round float values to int

		const Vertex4d* vertex1 = _linesVertexList[i];
		const Vertex4d* vertex2 = _linesVertexList[++i];
		const Color& color =	vertex1->color();	// FIXME: allright, we should check, whether colors of both vertices are the same and if not.....
		_outputBuffer.line(vertex1->x(), vertex1->y(), vertex2->x(), vertex2->y(), color);
	}
}

const Image2dRGB& OpenGL::glFlush()
{
	assert(_activeVertexList == NONE);

	drawLines();

	return _outputBuffer;
}

void OpenGL::glVertex4d(double x, double y, double z, double w)
{
	assert(_initialized);
	assert(_activeVertexList != NONE);

	Vertex4d* vertex = new Vertex4d(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor);
	switch (_activeVertexList)
	{
	case GL_LINES:
		_linesVertexList.push_back(vertex);
		break;

	case GL_POLYGON:
		break;

	case NONE:
	default:
		std::cerr<<"glVertex called while no active vertex list selected!"<<std::endl;
		break;
	}
}

void OpenGL::glColor(double r, double g, double b, double alpha)
{
	_activeColor= Color(r, g, b, alpha);
}

void OpenGL::glBegin(ActiveVertexList what)
{
	assert(_initialized);
	assert(what != NONE);
	_activeVertexList = what;
}

void OpenGL::glEnd()
{
	assert(_initialized);
	_activeVertexList = NONE;
}

OpenGL::OpenGL()
{
	_initialized = false;
}

void OpenGL::init(int x, int y)
{
	_glClearColor.clear(0.0);
	_activeVertexList = NONE;
//	_colorBuffer.resize(x, y);
	_outputBuffer.resize(x, y);
	_activeColor = Color(1, 1, 1, 1);
	_projection.setIdentity();

	Matrix4d rotY;
	rotY<< 	0.707106781, 0, 0.707106781, 0,
						0, 1, 0, 0,
						-0.707106781, 0, 0.707106781, 0,
						0, 0, 0, 1;	// rotY 45deg
	Matrix4d rotZ;
	rotZ<<	0.707106781, -0.707106781, 0, 0,
				0.707106781, 0.707106781, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
//	_projection *= rotY;
//	_projection *= rotZ;

//	std::cout<<_projection<<std::endl;

	_initialized = true;
}

void OpenGL::glClearColor(float red, float green, float blue, float alpha)
{
	assert(_initialized);
	_glClearColor = PixelRGBA(red, green, blue, alpha);
}

OpenGL::~OpenGL()
{
	// TODO Auto-generated destructor stub
	// TODO: delete all allocated memory!!
}

}
