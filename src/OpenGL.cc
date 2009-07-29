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
#include <algorithm>

#define PI 3.1415926535897932384626433832795	// tell me of a better way...
inline double radian(const double degree)
{
	return degree*(static_cast<double>(PI)/180.0);
}

namespace ggl
{

void OpenGL::clearColorBuffer()
{
	_colorBuffer->clear(_glClearColor);
}

void OpenGL::glLoadIdentity()
{
	// TODO: need to distinguish active matrix!!
	_projection.setIdentity();
}

void OpenGL::glTranslate(Real x, Real y, Real z)
{
	// TODO: check, whether we are not between glBegin glEnd, otherwise set some error

	Matrix4d trMatrix;
	trMatrix <<	1, 0, 0, x,
					0, 1, 0, y,
					0, 0, 1, z,
					0, 0, 0, 1;

	_projection *= trMatrix;
}

void OpenGL::glRotate(Real angle, Real x, Real y, Real z)
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

	drawLines_smooth();
	drawLines_flat();
}
void OpenGL::drawLines_smooth()
{

}

void OpenGL::drawLines_flat()
{
	size_t top;
	if((_linesVertexList_flat.size() % 2) != 0)	// check if we have correct number of vertices for our lines
		top = _linesVertexList_flat.size() - 1;
	else
		top = _linesVertexList_flat.size();

	for(size_t i = 0; i < top; ++i)
	{
		const Vertex4& vertex1 = _linesVertexList_flat[i];
		const Vertex4& vertex2 = _linesVertexList_flat[++i];
		const Color& color =	vertex2.color();	// FIXME: allright, we should check, whether colors of both vertices are the same and if not.....
		_colorBuffer->line(round_quick(vertex1.x()), round_quick(vertex1.y()), round_quick(vertex2.x()), round_quick(vertex2.y()), color);
	}
}

const CanvasRGB* OpenGL::glFlush()
{
	assert(_activeVertexList == NONE);

	drawLines();
	drawTriangles();

	_linesVertexList_smooth.clear();
	_linesVertexList_flat.clear();
	_trianglesVertexList.clear();

	return _colorBuffer;
}

void OpenGL::glVertex4(Real x, Real y, Real z, Real w)
{
	assert(_initialized);
	assert(_activeVertexList != NONE);

	//Vertex4d* vertex = new Vertex4d(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor);
	switch (_activeVertexList)
	{
	case GL_LINES:
		if(_shadeModel == GL_SMOOTH)
			_linesVertexList_smooth.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		else
			_linesVertexList_flat.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		break;

	case GL_POLYGON:
		break;

	case GL_TRIANGLES:
		_trianglesVertexList.push_back(Vertex4(_projection * Matrix<double, 4, 1>(x, y, z, w), _activeColor));
		break;

	case NONE:
	default:
		std::cerr<<"glVertex called while no active vertex list selected!"<<std::endl;
		break;
	}
}

void OpenGL::glColor(float r, float g, float b, float alpha)
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

void OpenGL::drawTriangles()
{
	/*
	 * TODO
	 * some info about advanced triangle rasterization:
	 * 1
	 * http://www.devmaster.net/forums/showthread.php?t=1094
	 * http://www.cs.unc.edu/~olano/papers/2dh-tri/
	 *
	 */


	size_t top;
	if((_trianglesVertexList.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
		top = _trianglesVertexList.size() - 1;
	if((_trianglesVertexList.size() % 3) != 0)	// check if we have correct number of vertices for our triangles
			top = _trianglesVertexList.size() - 1;
	else
		top = _trianglesVertexList.size();

	for(size_t i = 0; i < top; i += 3)
		//drawTriangle_wired(_trianglesVertexList[i+0], _trianglesVertexList[i+1], _trianglesVertexList[i+2]);
		drawTriangle_flat(_trianglesVertexList[i+0], _trianglesVertexList[i+1], _trianglesVertexList[i+2], _trianglesVertexList[i+2].color());
}

void OpenGL::drawTriangle_wired(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3)
{
	_colorBuffer->line(round_quick(v1.x()), round_quick(v1.y()), round_quick(v2.x()), round_quick(v2.y()), v1.color());
	_colorBuffer->line(round_quick(v2.x()), round_quick(v2.y()), round_quick(v3.x()), round_quick(v3.y()), v2.color());
	_colorBuffer->line(round_quick(v3.x()), round_quick(v3.y()), round_quick(v1.x()), round_quick(v1.y()), v3.color());
}

void OpenGL::drawTriangle_flat(const Vertex4 & v1, const Vertex4 & v2, const Vertex4 & v3, const Color & color)
{
	// it would make sense to use fixed point arithmetics...but...
	// and incorporate bresenham algorithm, but...who will ever call this function?

	// first we sort vertices by y

	/*
	const Vertex4 *top, *middle, *bottom;

	if(v1.y() > v2.y()) {
		if( v1.y() > v3.y()) {
			top = &v1;
			if( v2.y() > v3.y()) {
				middle = &v2;
				bottom = &v3;
			}
			else {
				middle = &v3;
				bottom = &v2;
			}
		}
		else {
			top = &v3;
			middle = &v1;
			bottom = &v2;
		}
	}
	else {
		if(v2.y() > v3.y()) {
			top = &v2;
			if(v1.y() > v3.y()) {
				middle = &v1;
				bottom = &v3;
			}
			else {
				middle = &v3;
				bottom = &v1;
			}
		}
		else {
			top = &v3;
			middle = &v2;
			bottom = &v1;
		}
	}
	*/
	// might be better to sort using swap
	const Vertex4 *top = &v1;
	const Vertex4 *middle = &v2;
	const Vertex4 *bottom = &v3;

	if(bottom->y() > middle->y())
		std::swap(bottom, middle);
	if(bottom->y() > top->y())
		std::swap(bottom, top);
	if(middle->y() > top->y())
		std::swap(middle, top);


	// lets iterate from bottom to top
	// we count the float x difference for both lines we iterate
	// line 1 should be the longer (from top to bottom)

	// TODO: check, for impossible triangles etc...
	assert(bottom->y() != top->y());	// get rid of this
	Real dx1 = (top->x() - bottom->x()) / (top->y() - bottom->y());

	assert(middle->y() != bottom->y());	// get rid of this
	Real dx2 = (middle->x() - bottom->x()) / (middle->y() - bottom->y());

	Real x1 = bottom->x();
	Real x2 = bottom->x();

	int y = round_quick(bottom->y());
	while(y < middle->y())
	{
		_colorBuffer->hLine(round_quick(x1), y, round_quick(x2), color);
		x1 += dx1;
		x2 += dx2;
		++y;
	}
	// and now for the other 'half' of the triangle
	assert(middle->y() != top->y());	// get rid of this
	dx2 = (top->x() - middle->x()) / (top->y() - middle->y());
	x2 = middle->x();	// this fixes precision problems(visible) with adding float numbers
	while(y <= top->y())
	{
		_colorBuffer->hLine(round_quick(x1), y, round_quick(x2), color);
		x1 += dx1;
		x2 += dx2;
		++y;
	}
}

void OpenGL::glShadeModel(const ShadeModel& model)
{
		if(inBetweenBeginEnd())
			return;	// TODO: should return some error
		_shadeModel = model;
}

OpenGL::OpenGL()
{
	_initialized = false;
	_colorBuffer = NULL;
}

void OpenGL::init(int x, int y)
{
	_glClearColor.clear(0.0);
	_activeVertexList = NONE;
//	_colorBuffer.resize(x, y);
//	_colorBuffer = new Image2dRGB;
//	_colorBuffer->resize(x, y);
	_activeColor = Color(1, 1, 1, 1);
	_projection.setIdentity();
	_shadeModel = GL_SMOOTH;	// default mode according to ogl specification


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
